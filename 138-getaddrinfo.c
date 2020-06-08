/* 138-getaddrinfo.c --- 继续学习 getaddrinfo */
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

int
main(void)
{
    struct addrinfo hints = {
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
        .ai_flags = AI_PASSIVE | AI_ADDRCONFIG,
    };
    struct addrinfo* result;
    int error = getaddrinfo("localhost", NULL, &hints, &result);
    if (error) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
        exit(EXIT_FAILURE);
    }
    for (struct addrinfo* rp = result; rp; rp = rp->ai_next) {
        char buf[256];
        inet_ntop(rp->ai_family, rp->ai_addr, buf, sizeof buf);
        printf("IP Address: '%s' ", buf);
        
        char host[100];
        char service[100];
        int error = getnameinfo(rp->ai_addr, rp->ai_addrlen,
                                host, sizeof host,
                                service, sizeof service,
                                0);
        if (error) {
            fprintf(stderr, "getnameinfo: %s\n", gai_strerror(error));
            exit(EXIT_FAILURE);
        }
        printf("'%s', '%s'\n", host, service);
    }

    int fd = 0;
    struct addrinfo* rp;
    for (rp = result; rp; rp = rp->ai_next) {
        int sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1) {
            perror("socket");
            continue;
        }

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == -1) {
            perror("bind");
            exit(EXIT_FAILURE);
        } else {
            fd = sfd;
            break;
        }

    }

    if (fd) {
        struct sockaddr addr = {0};
        socklen_t len = rp->ai_addrlen;
        int error = getsockname(fd, &addr, &len);
        if (error) {
            fprintf(stderr, "getsockname: %s\n", gai_strerror(error));
            exit(EXIT_FAILURE);
        }

        char host[100];
        char service[20];
        error = getnameinfo(&addr, len, host, sizeof host, service, sizeof service, 0);
        if (error) {
            fprintf(stderr, "getnameinfo: %s\n", gai_strerror(error));
            exit(EXIT_FAILURE);
        }
        printf("host: %s, port: %s\n", host, service);
        
        if (listen(fd, 5) == -1) {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        while (1) {
            struct sockaddr caddr = {0};
            int cfd = accept(fd, &caddr, &len);
            if (cfd == -1) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            error = getnameinfo(&addr, len, host, sizeof host, service, sizeof service, 0);
            if (error) {
                fprintf(stderr, "getnameinfo: %s\n", gai_strerror(error));
                exit(EXIT_FAILURE);
            }
            printf("Connection from %s:%s\n", host, service);
            
            char buf[256];
            ssize_t numRead;
            while((numRead = read(cfd, buf, sizeof buf)) > 0) {
                write(STDOUT_FILENO, buf, numRead);
            }
            close(cfd);
        }
    }
}
