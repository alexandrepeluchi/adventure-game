#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

#define MAXBUFF 1024
#define FIM "fim da transmissao"

int client(int);
void error(char *msg);

int tela = 0;

int main(int argc, char *argv[])
{
    int socketfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    if (argc < 3) {
       fprintf(stderr,"Usage: %s ipaddress portnumber\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);

    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0)
        error("Função client: erro ao abrir socket");

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"Função client: erro, host errado\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);

    serv_addr.sin_port = htons(portno);
    if (connect(socketfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("Função client: erro ao conectar");

    client(socketfd);

    //Fecha o socket apos uso
    close (socketfd);

    /*
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)
         error("Função cliente: erro ao escrever no socket");

    bzero(buffer,MAXBUFF);
    n = read(sockfd,buffer,MAXBUFF);

    if (n < 0)
         error("Função cliente: erro ao ler do socket");
    printf("%s\n",buffer);
    */
    return 0;
}

client(socketfd)
int      socketfd;
{
    char   buffer[MAXBUFF];
    int    n, flag;

    if (tela == 0) {
        printf("Por favor digite seu nome: ");
        bzero(buffer, MAXBUFF);
        fgets(buffer, MAXBUFF,stdin);
        tela++;
        puts("");
    }

    n = write(socketfd, buffer, strlen(buffer));
    if (n < 0)
         error("Função cliente: erro ao escrever no socket");

    bzero(buffer, MAXBUFF);

    do {
        if(tela == 1) {
            if ((n = read(socketfd, buffer, MAXBUFF)) < 0) {
                printf("Funcao client: erro no recebimento de dados do server");
                close(socketfd);
                exit(0);
            }
            //printf("Nome: %s\n", buffer);

            // Escreve o Menu
            if (strcmp(buffer,FIM) != 0)            //Se nao e' fim de transmissao
            //printf("Escreve intro");                                      //Escreve os dados para a saida padrao
            if (write(1, buffer, n) != n)  {    //fd 1 = stdout
                 printf("Funcao client:  erro na escrita para o video");
                 close(socketfd);
                 exit(0);
            }
            tela++;
        } else if (tela == 2) {
            // MENU DO JOGO
            printf("\tOpção Menu: ");
            bzero(buffer, MAXBUFF);
            fgets(buffer, MAXBUFF, stdin);
            n = write(socketfd, buffer, strlen(buffer));

            if ((n = read(socketfd, buffer, MAXBUFF)) < 0) {
                printf("Funcao client: erro no recebimento de dados do server");
                close(socketfd);
                exit(0);
            } else {
                // Exibe as infos do tutorial ou inicia o game e muda tela
                if (buffer[0] == '1') {
                     printf("%s", buffer);
                     tela++;
                } else {
                    printf("%s", buffer);
                }
            }
        } else if (tela == 3) {
            // JOGO INICIADO
            printf("\t? ");
            bzero(buffer, MAXBUFF);
            fgets(buffer, MAXBUFF, stdin);
            n = write(socketfd, buffer, strlen(buffer));

            if ((n = read(socketfd, buffer, MAXBUFF)) < 0) {
                printf("Funcao client: erro no recebimento de dados do server");
                close(socketfd);
                exit(0);
            } else {
                // Exibe as infos do jogo
                printf("%s", buffer);
            }
        }
    } while (strcmp(buffer, FIM) != 0);
}

void error(char *msg)
{
    perror(msg);
    exit(0);
}
