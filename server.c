/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXBUFF 1024

void error(char *msg);
int server(int);

int main(int argc, char *argv[])
{
    int socketfd, serv_len;

    struct sockaddr_in serv_addr;

    if ( (socketfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0){
      printf("servidor: erro na criacao do socket");
      exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(0);

    // Pega uma porta aleatória
    serv_len = sizeof(serv_addr);
    if (bind(socketfd, (struct sockaddr *) &serv_addr, serv_len)<0) {
        printf("servidor: erro no bind local");
        close(socketfd);
        exit(0);
    }

    // Mostra a porta alocada dinamicamente
    if (getsockname(socketfd, (struct sockaddr *)&serv_addr, &serv_len) < 0) {
        printf("Servidor: erro na chamada da funcao getsockname");
        close(socketfd);
        exit(0);
    }

    printf("\nPorta alocada para o Servidor: %d\n", ntohs(serv_addr.sin_port));

    //Chama a funcao servidor para esperar uma requisicao de servico

    server(socketfd);

    //fecha o socket apos uso

    close (socketfd);

    exit(0);
}

void error(char *msg)
{
    perror(msg);
    exit(1);
}

server(socketfd)
int socketfd;
{
    char buffer[MAXBUFF];
    int newsocketfd, n, clilen;
    struct sockaddr_in cli_addr;

    //Prepara para receber conexoes dos clientes
    listen(socketfd,5);

    clilen = sizeof(cli_addr);

    newsocketfd = accept(socketfd, (struct sockaddr *) &cli_addr, &clilen);

    if (newsocketfd < 0)
      error("Funcao server: erro no accept");

    bzero(buffer,MAXBUFF);
    n = read(newsocketfd,buffer,MAXBUFF);

    if (n < 0)
      error("Funcao server: erro de ler do socket");

    printf("O jogador %s se conectou!\n",buffer);

    //n = write(newsocketfd,"I got your message",18);

    if (n < 0)
      error("Funcao server: erro ao escrever no socket");

    return 0;
}
