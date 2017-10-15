/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define MAXBUFF 1024
# define   FIM    "fim da transmissao"

int gameOver = 0;
int tela = 0;

char * Introducao();
char * Tutorial();

char * Teste();

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

server(socketfd)
int socketfd;
{
    char buffer[MAXBUFF], nome[30];
    char *aux;
    int newsocketfd, n, clilen, menuN;
    struct sockaddr_in cli_addr;

    //Prepara para receber conexoes dos clientes
    listen(socketfd,5);

    clilen = sizeof(cli_addr);

    newsocketfd = accept(socketfd, (struct sockaddr *) &cli_addr, &clilen);

    if (newsocketfd < 0)
      error("Funcao server: erro no accept");

    do {
    if (tela == 0) {
        bzero(nome, MAXBUFF);
        n = read(newsocketfd, nome, MAXBUFF);

        if (n < 0)
          error("Funcao server: erro de ler do socket");

        printf("\nO jogador %s se conectou!\n\n\n", nome);
        tela++;
    }


    if (tela == 1) {
        bzero(buffer, MAXBUFF);
        aux = Introducao();
        strcpy(buffer, aux);
        tela++;
        n = write(newsocketfd, buffer, strlen(buffer));
    }

    if (tela == 2) {
        bzero(buffer, MAXBUFF);
        while (tela == 2) {
            n = read(newsocketfd, buffer, MAXBUFF);
            //printf("Opcao do Menu retornado pelo cliente: %s", buffer);
            if(n > 0) {
                switch (buffer[0]) {
                  case '1':
                      printf("Iniciar Jogo");
                  break;

                  case '2':
                      bzero(buffer, MAXBUFF);
                      aux = Tutorial();
                      strcpy(buffer, aux);
                      n = write(newsocketfd, buffer, strlen(buffer));
                  break;

                  default:
                      printf("Comando errado");
                  break;
                }
            }
        }
    }
  } while (gameOver != 1);

    //Envia aviso de fim de transmissao
    if (gameOver != 0) {
      n = sizeof(FIM);
      if (write(newsocketfd, FIM, n) != n) {
          printf("Funcao server: erro no envio do fim de transmissao pelo socket");
          close(socketfd);
          close(newsocketfd);
          exit(0);
      }
    }

    if (n < 0)
      error("Funcao server: erro ao escrever no socket");

      // Introducao();
      /*
      bzero(buffer, MAXBUFF);
      char *aux = Teste();
      strcpy(buffer, aux);
      n = write(newsocketfd, buffer, strlen(buffer));
      */
    return 0;
}

void error(char *msg)
{
    perror(msg);
    exit(1);
}

char * Introducao() {
  // Sem \n no printf não exibe msg no servidor
  printf("Introdução Enviada\n");
  char * aux = malloc(MAXBUFF);
  strncpy(aux, "\t \t Encontre a Chave\n\n \
  \t Você foi preso, mas um amigo político\n \
  \t mandou esconder a chave na sua cela,\n \
  \t tente encontrar!\n\n \
  \t\t [1] Iniciar\n \
  \t\t [2] Tutorial\n \
  \t\t [3] Sair\n\n", MAXBUFF);
  return aux;
}

char * Tutorial() {
  // Sem \n no printf não exibe msg no servidor
  printf("Tutorial Enviado\n");
  char * aux = malloc(MAXBUFF);
  strncpy(aux, "\n\n\t \t Comandos validos:\n\n \
  \t Qualquer duvida do que fazer digite 'ajuda',\n \
  \t você pode usar o comando 'olhar' para saber o que está vendo,\n \
  \t para olhar algo de perto digite 'examinar' e o 'objeto',\n\n \
  \t por exemplo 'examinar pia', após examinar algo você\n \
  \t pode, escolher se executa ou não uma ação com 'sim' ou 'nao',\n \
  \t e se finalmente encontrar a chave 'pegar chave'!\n\n", MAXBUFF);
  return aux;
}

char * Sair() {
    char * aux = malloc(MAXBUFF);
    strncpy(aux, "1", MAXBUFF);
    return aux;
}

char * Teste() {
    char * aux = malloc(MAXBUFF);
    strncpy(aux, "É bem possível", MAXBUFF);
    return aux;
}

/*
void * Introducao() {
  printf("              Get Out Adventure Game\n");
  printf("              por Alexandre Peluchi\n\n");
  printf("A Terra de repente inexplicavelmente mudou sua órbita elíptica\n");
  printf("e assim, começou a seguir um caminho que, gradualmente, \n");
  printf("de pouco a pouco, dia a dia, se movia para longe do sol.\n\n");
}
*/
