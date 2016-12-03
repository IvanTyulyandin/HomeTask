#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<signal.h>
#include<fcntl.h>

#define BYTES 1024

char *ROOT;
int listenfd;

void startServer(char *port) {
  struct addrinfo hints, *res, *p;
  memset (&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if (getaddrinfo(NULL, port, &hints, &res) != 0)
      {
          perror("getaddrinfo() error");
          exit(1);
      }
  for (p = res; p != NULL; p = p -> ai_next)
  {
      listenfd = socket(p -> ai_family, p -> ai_socktype, 0);
      if (listenfd == -1) continue;
      if (bind(listenfd, p -> ai_addr, p -> ai_addrlen) == 0) break;
  }
  if (p == NULL)
  {
      perror ("socket() or bind()");
      exit(1);
  }
  freeaddrinfo(res);

  if ( listen (listenfd, 1000) != 0 )
  {
      perror("listen() error");
      exit(1);
  }
}

void  respond(int clientfd) {
  char mesg[10000], *checkStr, *query, data_to_send[BYTES], path[10000];
  int rcvd, fd, bytes_read;

  memset((void*)mesg, (int)'\0', 10000);

  rcvd = recv(clientfd, mesg, 10000, 0);

  printf("%s\n", mesg);

  checkStr = strtok(mesg, " ");
  if (strcmp(checkStr, "GET") != 0) {
    perror("not GET query");
    exit(1);
  }
  query = strtok(NULL, " ");
  checkStr = strtok(NULL, " \n\0");
  if ((strcmp(checkStr, "HTTP/1.0") != 0) && (strcmp(checkStr, "HTTP/1.1") != 0)) {
    write(clientfd, "HTTP/1.0 400 Bad Request\n", 25);
  }
  strcpy(path, ROOT);
  strcpy(&path[strlen(ROOT)], query);
  printf("file %s\n", path);
  if ( (fd = open(path, O_RDONLY))!= -1 ) {
    send(clientfd, "HTTP/1.0 200 OK\n\n", 17, 0);
    while ((bytes_read = read(fd, data_to_send, BYTES)) > 0 ) {
      write (clientfd, data_to_send, bytes_read);
    }
  }
  else {
    write(clientfd, "HTTP/1.0 404 Not Found\n", 23);
  }
  shutdown (clientfd, SHUT_RDWR);
  close(clientfd);
}

int main(int argc, char const *argv[]) {
  int clientfd;
  struct sockaddr_in clientaddr;
  socklen_t addrlen;
  char c;
  char PORT[6];
  ROOT = getenv("PWD");
  strcpy(PORT,"10000");

  printf("Server started at port no. %s%s%s with root directory as %s%s%s\n","\033[92m",PORT,"\033[0m","\033[92m",ROOT,"\033[0m");

  startServer(PORT);

  while (1) {
    addrlen = sizeof(clientaddr);
    clientfd = accept (listenfd, (struct sockaddr *) &clientaddr, &addrlen);
    if (clientfd < 0) {
      perror("accept() error");
      exit(1);
    }
    else {
      respond(clientfd);
    }
  }
  return 0;
}
