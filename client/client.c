#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ctype.h>
#include <stdlib.h>

#include "sock.h"

void sendtosev(char out[], int fd)
{
    char rec[500];
    memset(rec, 0, 500);
    send(fd, out, strlen(out)+1, 0);
    recv(fd, rec, 500, 0);
    printf("%s\n", rec);
    return;
}

int main(int argc, char **argv)
{
    int opt;
    char *server_host_name = NULL, *server_port = NULL;

    /* Parsing args */
    while ((opt = getopt(argc, argv, "h:p:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            server_host_name = malloc(strlen(optarg) + 1);
            strncpy(server_host_name, optarg, strlen(optarg));
            break;
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

    if (!server_host_name)
    {
        fprintf(stderr, "Error!, No host name provided!\n");
        exit(1);
    }

    if (!server_port)
    {
        fprintf(stderr, "Error!, No port number provided!\n");
        exit(1);
    }

    /* Open a client socket fd */
    int clientfd __attribute__((unused)) = open_clientfd(server_host_name, server_port);

    /* Start your coding client code here! */
    char input[500],sen[500];

    char *command,*key,*value,*rest;

    while(1)
    {

        printf("-------------------->");
        scanf("%[^\n]%*c", input);
        strcpy(sen,input);
        command = strtok(input, " ");

        if(strcmp(command, "SET") == 0)
        {
            key = strtok(NULL, " ");
            value = strtok(NULL, " ");
            rest = strtok(NULL, " ");

            if(key == NULL)
                printf("[ERROR]Unknow Command\n");
            else
            {
                if(value == NULL)
                    printf("[ERROR]Unknow Command\n");
                else
                {
                    if(rest == NULL)
                        sendtosev(sen, clientfd);
                    else
                        printf("[ERROR]Unknow Command\n");
                }
            }
        }
        else if(strcmp(command, "GET") == 0)
        {
            key = strtok(NULL, " ");
            rest = strtok(NULL, " ");

            if(key == NULL)
                printf("[ERROR]Unknow Command\n");
            else
            {
                if(rest == NULL)
                    sendtosev(sen, clientfd);
                else
                    printf("[ERROR]Unknow Command\n");
            }
        }
        else if(strcmp(command, "DELETE") == 0)
        {
            key = strtok(NULL, " ");
            rest = strtok(NULL, " ");
            if(key == NULL)
                printf("[ERROR]Unknow Command\n");
            else
            {
                if(rest == NULL)
                    sendtosev(sen, clientfd);
                else
                    printf("[ERROR]Unknow Command\n");
            }
        }
        else if(strcmp(command, "EXIT") == 0)
        {
            key = strtok(NULL," ");
            if(key != NULL)
                printf("[ERROR]Unknow Command\n");
            else
            {
                send(clientfd, sen, strlen(sen)+1, 0);
                break;
            }
        }
        else
            printf("[ERROR]Unknow Command\n");
    }

    shutdown(clientfd, SHUT_RDWR);
    close(clientfd);
    return 0;
}
/*

USING LINKED LIST TO GET COM KEY VALUE
THIS TAKE ME LOTS OF TIME
AT LAST I FOUND strtok...
I AM SUCH A IDIOT...

typedef struct input input;

struct input{
    char c;
    input *next;
};

    char com[101];
    char key[101];
    char value[101];
    char rev[101];
    char s[300];


while(1){
    input* first;
    input* now = malloc(sizeof(input));
    first = now;
    while (now -> c  = getc(stdin)){
      if(now -> c == '\n'){
          now -> next = NULL;
          break;
      }
      now -> next = malloc(sizeof(input));
      now = now -> next;
    }

    int a=0,s=0;
    now = first;
    while(1){
        if(now -> c == ' '){s++;}
        if(now -> next == NULL){break;}
        else now = now -> next;
    }

    now = first;
    while(1){
        if(now -> c == ' ' || now -> c == '\n'){break;}
        now = now -> next;
        a++;
    }

    //com = malloc((a+1)*sizeof(char));
    now = first;
    for(int i=0;i<101;i++){
        if(i==a){com[i]='\0';}
        else if(i==100){com[i]='\0';}
        else if(i<a){
        com[i] = now -> c;
        now = now -> next;
        }
        else{com[i]=60;}
    }

    if(strcmp(com,"SET") == 0){

        if(s!=2){printf("unknow command\n");}
        else{
            now = now -> next;
            input* setnow = now;
            a=0;
            while(1){
                if(now -> c == ' '){break;}
                now = now -> next;
                a++;
            }
            //key = malloc((a+1)*sizeof(char));

            for(int i=0;i<101;i++){
                if(i==a){key[i]='\0';}
                else if(i==100){key[i]='\0';}
                else if(i<a){
                    key[i] = setnow -> c;
                    setnow = setnow -> next;
                }
                else {key[i]=60;}
            }

            a=0;
            now = now -> next;
            setnow = now;
            while(1){
                if(now -> c == '\n'){break;}
                now = now -> next;
                a++;
            }
            //value = malloc((a+1)*sizeof(char));
            for(int i=0;i<101;i++){
                if(i==a){value[i]='\0';}
                else if(i==100){value[i]='\0';}
                else if(i<a){
                    value[i] = setnow -> c;
                    setnow = setnow -> next;
                }
                else {value[i]=60;}
            }
            printf("%s/%s/%s/\n",com,key,value);
            send(clientfd,com,101,0);
            send(clientfd,key,101,0);
            send(clientfd,value,101,0);
            memset(rev,0,101);
            recv(clientfd,rev,101,0);
            printf("%s\n",rev);
        }
    }

    else if(strcmp(com,"GET") == 0){

        if(s!=1){printf("unknow command\n");}
        else{
            now = now -> next;
            input* setnow = now;
            a=0;
            while(1){
                if(now -> c == '\n'){break;}
                now = now -> next;
                a++;
            }
            //key = malloc((a+1)*sizeof(char));

            for(int i=0;i<=a;i++){
                if(i==a){key[i]='\0';}
                else{
                    key[i] = setnow -> c;
                    setnow = setnow -> next;
                }
            }
            send(clientfd,com,101,0);
            send(clientfd,key,101,0);
            memset(rev,0,101);
            recv(clientfd,rev,101,0);
            printf("%s\n",rev);
        }
    }

    else if(strcmp(com,"DELETE") == 0){

        if(s!=1){printf("unknow command\n");}
        else{
            now = now -> next;
            input* setnow = now;
            a=0;
            while(1){
                if(now -> c == '\n'){break;}
                now = now -> next;
                a++;
            }
            //key = malloc((a+1)*sizeof(char));

            for(int i=0;i<=a;i++){
                if(i==a){key[i]='\0';}
                else{
                    key[i] = setnow -> c;
                    setnow = setnow -> next;
                }
            }
            send(clientfd,key,101,0);
        }
    }

    else {printf("unknow command\n");}
    printf("%s/%s/%s/\n",com,key,value);
*/