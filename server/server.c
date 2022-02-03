#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <netdb.h>
#include <ctype.h>

#include <signal.h>
#include "types.h"
#include "sock.h"

pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;

void *thread_act(void* param)
{

    char rec[500];
    char sen[500];

    char *command,*key,*value;
    char val[200];

    FILE* file;
    char filename[200];

    int fd = *(int *) param;

    pthread_detach(pthread_self());

    while(recv(fd, rec, 500, 0))
    {

        command = strtok(rec, " ");
        key = strtok(NULL, " ");

        sprintf(filename, "%s.txt", key);

        if(strcmp(command, "SET") == 0)
        {
            value = strtok(NULL, " ");

            pthread_rwlock_wrlock(&lock);
            if(access(filename, 0) != 0)
            {
                file = fopen(filename, "w");
                fprintf(file, "%s", value);
                fclose(file);
                pthread_rwlock_unlock(&lock);
                sprintf(sen, "[SUCCESS] Key:%s value:%s is stored!", key, value);
                send(fd, sen, strlen(sen)+1, 0);
            }
            else
            {
                pthread_rwlock_unlock(&lock);
                sprintf(sen, "[ERROR]Key exist!");
                send(fd, sen, strlen(sen)+1, 0);
            }

        }
        else if(strcmp(command, "GET") == 0)
        {
            pthread_rwlock_rdlock(&lock);
            if(access(filename, 0) != 0)
            {
                pthread_rwlock_unlock(&lock);
                sprintf(sen, "[ERROR]Unkonw key!");
                send(fd, sen, strlen(sen)+1, 0);
            }
            else
            {
                file = fopen(filename, "r");
                fread(val, 101, 1, file);
                fclose(file);
                pthread_rwlock_unlock(&lock);
                sprintf(sen, "[SUCCESS] The value : %s", val);
                send(fd, sen, strlen(sen)+1, 0);
            }

        }
        else if(strcmp(command, "DELETE") == 0)
        {
            pthread_rwlock_wrlock(&lock);
            if(access(filename, 0) != 0)
            {
                sprintf(sen, "[ERROR]Unknow key!");
                pthread_rwlock_unlock(&lock);
                send(fd, sen, strlen(sen)+1, 0);
            }
            else
            {
                remove(filename);
                pthread_rwlock_unlock(&lock);
                sprintf(sen,"[SUCCESS] Key : %s is deleted!", key);
                send(fd, sen, strlen(sen)+1, 0);
            }

        }
        else if(strcmp(command, "EXIT") == 0)
            break;
    }
    pthread_exit(0);
}

int main(int argc, char **argv)
{
    char *server_port = 0;
    int opt = 0;
    /* Parsing args */
    while ((opt = getopt(argc, argv, "p:")) != -1)
    {
        switch (opt)
        {
        case 'p':
            server_port = malloc(strlen(optarg) + 1);
            strncpy(server_port, optarg, strlen(optarg));
            break;
        case '?':
            fprintf(stderr, "Unknown option \"-%c\"\n", isprint(optopt) ?
                    optopt : '#');
            return 0;
        }
    }

    if (!server_port)
    {
        fprintf(stderr, "Error! No port number provided!\n");
        exit(1);
    }

    /* Open a listen socket fd */
    int listenfd __attribute__((unused)) = open_listenfd(server_port);
    /* Start coding your server code here! */
    int* fd;
    pthread_t t;

    printf("Have a nice day!\n");
    while(1)
    {
        fd = malloc(sizeof(int));

        if((*fd = accept(listenfd,NULL,NULL))==-1)
        {
            free(fd);
            continue;
        }
        pthread_create(&t, NULL, thread_act, (void *)fd);
    }


    close(listenfd);
    return 0;
}
/*
OLD thread_act ALSO USE LINKED LIST
BUT I FOUND THAT IF I SEND COM KEY VALUE BY PART
AND I DONT KNOW THE TIME RECV OR SEND FIREST
IT WILL MAKE AN UNKNOW ANSWER
SO I QUITED

void *thread_act(void * pa){
    int fd = * (int *)pa;

    char rec[300],com[101],key[101],value[101],s[101];
    pthread_detach(pthread_self());
    while(recv(fd,com,101,0)){
        for(int i=0;i<101;i++){
            putchar(com[i]);
            putchar('\n');
        }
        if(strcmp(com,"SET") == 0){
            //printf("%s/%s/%s\n",com,key,value);
            recv(fd,key,101,0);
            //printf("%s/%s/%s\n",com,key,value);
            recv(fd,value,101,0);
            //pthread_rwlock_lock(&lock);
            if(first == NULL){

                first = malloc(sizeof(node));
                last = first;
                sprintf(s,"[SUCCESS]Key %s value %s is stored.",key,value);
                send(fd,s,101,0);
            }
            else{
                node *now=first;
                while(1){
                    if(strcmp(key,now->k) == 0){
                        send(fd,"[EEORO]Key exist!",101,0);
                        break;
                    }
                    if(now -> next == NULL){

                        last -> next = malloc(sizeof(node));
                        last = last -> next;

                        strcpy(last -> k , key);
                        strcpy(last -> v , value);
                        last -> next = NULL;
                        sprintf(s,"[SUCCESS]Key %s value %s is stored.",key,value);
                        send(fd,s,101,0);
                        break;
                    }
                    now = now -> next;
                }
            }

        }
        else if(strcmp(com,"GET") == 0){
            recv(fd,key,101,0);
            node *now=first;
            while(1){
                if(strcmp(key,now->k) == 0){
                    sprintf(s,"[SUCCESS]value : %s",value);
                    send(fd,s,101,0);
                    break;
                }
                else now = now -> next;
                if(now -> next == NULL){
                    send(fd,"[EEORO]Not exist the key!",101,0);
                    break;
                }
            }
        }
        else if(strcmp(com,"DELETE") == 0){
            recv(fd,key,101,0);
        }
    }
    pthread_exit(0);

}
*/