#include "csapp.h"
#include <stdio.h>
/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr =
    "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 "
    "Firefox/10.0.3\r\n";

void doit(int fd);
void forward_request(char *serverhostname, char *serverport);
void read_requesthdrs(rio_t *rp, char *serverhostname, char *serverport);
int parse_uri(char *uri, char *filename);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg,
                 char *longmsg);

int main(int argc, char **argv) {
  int listenfd, connfd;
  char client_hostname[MAXLINE], client_port[MAXLINE];
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;

  /* Check command line args */
  if (argc != 2) {

    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }
  listenfd = Open_listenfd(argv[1]);
  while (1) {
    clientlen = sizeof(clientaddr);
    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
    Getnameinfo((SA *)&clientaddr, clientlen, client_hostname, MAXLINE,
                client_port, MAXLINE, 0);
    printf("Accepted connection from (%s, %s)\n", client_port, client_port);
    doit(connfd);  // line:netp:tiny:doit
    Close(connfd); // line:netp:tiny:close
  }
  //   printf("%s", user_agent_hdr);
  return 0;
}
void doit(int fd) {

  struct stat sbuf;
  char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
  char filename[MAXLINE], cgiargs[MAXLINE];
  char serverhostname[MAXLINE], serverport[MAXLINE];
  rio_t rio;

  Rio_readinitb(&rio, fd);
  if (!Rio_readlineb(&rio, buf, MAXLINE)) // line:netp:doit:readrequest
    return;
  printf("%s", buf);
  sscanf(buf, "%s %s %s", method, uri, version);
  if (strcasecmp(method, "GET")) {
    clienterror(fd, method, "501", "Not Implemented",
                "Proxy does not implement this method");
    return;
  } // line:netp:doit:endrequesterr
  printf("[DEBUG] before start parsing the request headers\n");
  read_requesthdrs(&rio, serverhostname, serverport);
  // TODO: get the filename
  parse_uri(uri, filename);

  // TODO: forward the request to the server and receive the response

  // TODO: return the server response to the client

  // TODO: clean up and close the connection
}

void forward_request(char *serverhostname, char *serverport) {
  int clientfd;
  rio_t rio_server;
  clientfd = Open_clientfd(serverhostname, serverport);
  Rio_readinitb(&rio_server, clientfd);
}

/*
 * read_requesthdrs - read HTTP request headers
 */
/* $begin read_requesthdrs */
void read_requesthdrs(rio_t *rp, char *serverhostname, char *serverport) {
  char buf[MAXLINE];

  //   Rio_readlineb(rp, buf, MAXLINE);
  //   printf("89 [DEBUG] request buf: %s\n", buf);
  while (strcmp(buf, "\r\n")) {
    Rio_readlineb(rp, buf, MAXLINE);
    printf("%s", buf);
    if (strstr(buf, "Host")) {
      if (strchr(buf + 6, ':')) {
        printf("[DEBUG] found the port number");
        sscanf(buf, "%*[^:]%*2c%[^:]%*c%s", serverhostname, serverport);
      } else {
        printf("[DEBUG] not found the port number\n");
        sscanf(buf, "%*[^:]%*2c%s", serverhostname);
        // Use the default HTTP port 80
        serverport = "80";
      }
      printf("[DEBUG] received the request, the hostname is %s, the port is : "
             "%s\n",
             serverhostname, serverport);
    }
    return;
  }
}

/*
 * parse_uri - parse URI into filename and CGI args
 *             return 0 if dynamic content, 1 if static
 */
/* $begin parse_uri */
int parse_uri(char *uri, char *filename) {
  char *ptr;

  if (uri[strlen(uri) - 1] == '/')
    strcat(filename, "index.html");
}
/* $end parse_uri */

/*
 * clienterror - returns an error message to the client
 */
/* $begin clienterror */
void clienterror(int fd, char *cause, char *errnum, char *shortmsg,
                 char *longmsg) {
  char buf[MAXLINE];

  /* Print the HTTP response headers */
  sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "Content-type: text/html\r\n\r\n");
  Rio_writen(fd, buf, strlen(buf));

  /* Print the HTTP response body */
  sprintf(buf, "<html><title>Tiny Error</title>");
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "<body bgcolor="
               "ffffff"
               ">\r\n");
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "%s: %s\r\n", errnum, shortmsg);
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "<p>%s: %s\r\n", longmsg, cause);
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "<hr><em>The Tiny Web server</em>\r\n");
  Rio_writen(fd, buf, strlen(buf));
}
/* $end clienterror */