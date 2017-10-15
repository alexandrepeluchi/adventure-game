#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAXBUFF 1024

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[MAXBUFF];
    if (argc < 3) {
       fprintf(stderr,"Usage: %s ipaddress portnumber\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Função cliente: erro ao abrir socket");

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"Função cliente: erro, host errado\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);

    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("Função cliente: erro ao conectar");

    printf("Por favor digite seu nome: ");
    bzero(buffer,MAXBUFF);
    fgets(buffer,MAXBUFF,stdin);

    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)
         error("Função cliente: erro ao escrever no socket");

    bzero(buffer,MAXBUFF);
    n = read(sockfd,buffer,MAXBUFF);

    if (n < 0)
         error("Função cliente: erro ao ler do socket");
    printf("%s\n",buffer);

    return 0;
}
