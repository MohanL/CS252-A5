//
//  echo.c
//  Server
//
//  Created by Mohan Liu on 4/22/15.
//  Copyright (c) 2015 Mohan Liu. All rights reserved.
//

#include "echo.h"
#include "rio.h"
#define	MAXLINE	 8192  /* Max text line length */
#define MAXBUF   8192  /* Max I/O buffer size */
#define LISTENQ  1024  /* Second argument to listen() */

void echo2(int connfd)
{
    size_t n;
    char buf[MAXLINE];
    rio_t rio;
    
    rio_readinitb(&rio,connfd);
    while((n = rio_readlineb(&rio,buf,MAXLINE))!=0)
    {
        printf("server received %zu bytes\n",n);
        rio_writen(connfd, buf,n);
    }
}