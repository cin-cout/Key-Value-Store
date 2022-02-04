# Key-value Store

## Directories
- /server ->	server program related sources
- /client ->	client program related sources
- /common ->	common inclusions
- /util ->	common utilization
- /build ->	target build directory

## Building the Project
Code out your `/server/server.c` and `/client/client.c`, then
```shell
$ make
```
Test your `/build/server` and `build/client`.

## Implementations
### Please briefly describe your multi-threading design pattern

- Using a while loop in main function to keep listen and 
waiting for the connect.If there is a connect ,the server
will accept it and create a thread.

- In the thread I used the pthread_detach() to delete the
memory of thread itself and using pthread_rwlock_lock() 
before any time need to appliy the data(by file),which
solve the problem of race condition.

### Please briefly describe your data structure implementation

- At first i try to use lined list to store the data.It is 
not difficult but using file is better than it a lot due to
its convenient function.
- Using fileopen() and fprint() to achieve "SET".
- Using access() and fread() to achieve "GET".
- Using access() and remove() to achieve "Delete".

## References
* [POSIX thread man pages](https://man7.org/linux/man-pages/man7/pthreads.7.html)
* [socket man pages](https://linux.die.net/man/7/socket)

