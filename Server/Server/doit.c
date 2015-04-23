//
//  doit.c
//  Server
//
//  Created by Mohan Liu on 4/23/15.
//  Copyright (c) 2015 Mohan Liu. All rights reserved.
//

#include "doit.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "rio.h"
#include <string.h>
#include "parse_uri.h"
#include "read.h"
#include "serve.h"
#include "clienterror.h"
#define MAXLINE 100
#define MAXBUF 100
void doit(int fd)
{
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;
    
    // read request line and headers
    rio_readinitb(&rio,fd);
    rio_readlineb(&rio, buf, MAXLINE);
    sscanf(buf,"%s %s %s", method, uri, version);
    if(strcasecmp(method, "GET"))
    {
        clienterror(fd, method, "501", "NOT IMPLEMENTED", "tiny does not implement this method ");
        return;
    }
    read_requesthdrs(&rio);
    
    // parse uri from GET requests
    is_static = parse_uri(uri, filename, cgiargs);
    if(stat(filename,&sbuf) < 0)
    {
        clienterror(fd, filename, "404", "NOT FOUND", "tiny coult not find this file");
        return;
    }
    
    if(is_static){ // serve static content
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) {
            clienterror(fd, filename, "403", "FORBIDDEN", "Tiny could not read the file");
            return;
        }
        serve_static(fd, filename, (int)sbuf.st_size);
    }
    else{
        if(! (S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode))
        {
            clienterror(fd, filename, "403", "FORBIDDEN", "tiny could not run the CGI program");
            return;
        }
        serve_dynamic(fd,filename,cgiargs);
    }
}