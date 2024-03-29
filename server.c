/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define MAXBUFF 1024
#define NUMLOCAIS 6
# define   FIM    "fim da transmissao"

int gameOver = 0;
int tela = 0;

char * Introducao();
char * TutorialHelp();
char * Sair();

char * Iniciar();
char * Olhar();

void Initialize();

char * Teste();

void error(char *msg);
int server(int);

typedef enum {
  Cama1,
  Cama2,
  CamaBaixo,
  Grade,
  Privada,
  Pia
} Local;

char * Mover(Local loc);

typedef enum {
  Parede,
  Cama,
  Broche,
  Corredor,
  PrivadaObj,
  Espelho,
  PiaObj
} Objetos;

char * Examinar(Objetos obj);

char *DescricaoLocal[NUMLOCAIS] = {
  "na cama da beliche.",
  "na cama de cima da beliche.",
  "debaixo da cama.",
  "na grade olhando pra fora.",
  "de frente para a privada.",
  "de frente para a pia."
};

int turnos;
int localAtual;
Local localChave = 2;
int chave = 0;

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
                          bzero(buffer, MAXBUFF);
                          aux = Iniciar();
                          strcpy(buffer, aux);
                          tela++;
                          n = write(newsocketfd, buffer, strlen(buffer));
                      break;

                      case '2':
                          bzero(buffer, MAXBUFF);
                          aux = TutorialHelp();
                          strcpy(buffer, aux);
                          n = write(newsocketfd, buffer, strlen(buffer));
                      break;

                      case '3':
                          printf("Sair, finalizando transmissao\n");
                          gameOver = 1;
                          tela++;
                      break;

                      default:
                          printf("Comando errado\n");
                      break;
                    }
                }
            }
        }

        // Aqui executa o jogo
        if (tela == 3) {
            bzero(buffer, MAXBUFF);
            while (tela == 3) {
                n = read(newsocketfd, buffer, MAXBUFF);
                if (n > 0) {
                    if (tolower(buffer[0] == 'a')) {
                        bzero(buffer, MAXBUFF);
                        aux = TutorialHelp();
                        strcpy(buffer, aux);
                        n = write(newsocketfd, buffer, strlen(buffer));
                    } else if (tolower(buffer[0] == 's')) {
                        printf("Sair, finalizando transmissao\n");
                        gameOver = 1;
                        tela++;
                    } else if (tolower(buffer[0] == 'o')) {
                        bzero(buffer, MAXBUFF);
                        aux = Olhar();
                        strcpy(buffer, aux);
                        n = write(newsocketfd, buffer, strlen(buffer));
                    } else if ((tolower(buffer[0] == 'e')) && (tolower(buffer[9] == 'p')) && (tolower(buffer[10] == 'a'))) { // Ex. Parede
                        bzero(buffer, MAXBUFF);
                        aux = Examinar(Parede);
                        strcpy(buffer, aux);
                        n = write(newsocketfd, buffer, strlen(buffer));
                    }else if ((tolower(buffer[0] == 'e')) && (tolower(buffer[9] == 'c')) && (tolower(buffer[10] == 'a'))) { // Ex. Cama
                        bzero(buffer, MAXBUFF);
                        aux = Examinar(Cama);
                        strcpy(buffer, aux);
                        n = write(newsocketfd, buffer, strlen(buffer));
                    }else if ((tolower(buffer[0] == 'e')) && (tolower(buffer[9] == 'b')) && (tolower(buffer[10] == 'r'))) { // Ex. Broche
                        bzero(buffer, MAXBUFF);
                        aux = Examinar(Broche);
                        strcpy(buffer, aux);
                        n = write(newsocketfd, buffer, strlen(buffer));
                    }else if ((tolower(buffer[0] == 'e')) && (tolower(buffer[9] == 'c')) && (tolower(buffer[10] == 'o'))) { // Ex. Corredor
                        bzero(buffer, MAXBUFF);
                        aux = Examinar(Corredor);
                        strcpy(buffer, aux);
                        n = write(newsocketfd, buffer, strlen(buffer));
                    }else if ((tolower(buffer[0] == 'e')) && (tolower(buffer[9] == 'p')) && (tolower(buffer[10] == 'r'))) { // Ex. Privada
                        bzero(buffer, MAXBUFF);
                        aux = Examinar(PrivadaObj);
                        strcpy(buffer, aux);
                        n = write(newsocketfd, buffer, strlen(buffer));
                    }else if ((tolower(buffer[0] == 'e')) && (tolower(buffer[9] == 'e')) && (tolower(buffer[10] == 's'))) { // Ex. Espelho
                        bzero(buffer, MAXBUFF);
                        aux = Examinar(Espelho);
                        strcpy(buffer, aux);
                        n = write(newsocketfd, buffer, strlen(buffer));
                    }else if ((tolower(buffer[0] == 'e')) && (tolower(buffer[9] == 'p')) && (tolower(buffer[10] == 'i'))) { // Ex. Pia
                        bzero(buffer, MAXBUFF);
                        aux = Examinar(PiaObj);
                        strcpy(buffer, aux);
                        n = write(newsocketfd, buffer, strlen(buffer));
                    }else if ((tolower(buffer[0] == 'm')) && (tolower(buffer[6] == 'c')) && (tolower(buffer[7] == 'a'))) { //Mover
                        bzero(buffer, MAXBUFF);
                        aux = Mover(Cama1);
                        strcpy(buffer, aux);
                        n = write(newsocketfd, buffer, strlen(buffer));
                    } else if ((tolower(buffer[0] == 'm')) && (tolower(buffer[6] == 'e')) && (tolower(buffer[7] == 'm'))) {
                        bzero(buffer, MAXBUFF);
                        aux = Mover(CamaBaixo);
                        strcpy(buffer, aux);
                        n = write(newsocketfd, buffer, strlen(buffer));
                    } else if ((tolower(buffer[0] == 'm')) && (tolower(buffer[6] == 'g')) && (tolower(buffer[7] == 'r'))) {
                        bzero(buffer, MAXBUFF);
                        aux = Mover(Grade);
                        strcpy(buffer, aux);
                        n = write(newsocketfd, buffer, strlen(buffer));
                    } else if ((tolower(buffer[0] == 'm')) && (tolower(buffer[6] == 'p')) && (tolower(buffer[7] == 'r'))) {
                        bzero(buffer, MAXBUFF);
                        aux = Mover(Privada);
                        strcpy(buffer, aux);
                        n = write(newsocketfd, buffer, strlen(buffer));
                    } else if ((tolower(buffer[0] == 'm')) && (tolower(buffer[6] == 'p')) && (tolower(buffer[7] == 'i'))) {
                        bzero(buffer, MAXBUFF);
                        aux = Mover(Pia);
                        strcpy(buffer, aux);
                        n = write(newsocketfd, buffer, strlen(buffer));
                    } else { // Para não dar erro se digitar algo errado
                        bzero(buffer, MAXBUFF);
                        strncpy(aux, "\n\t Comando errado tente novamente\n\n", MAXBUFF);
                        strcpy(buffer, aux);
                        n = write(newsocketfd, buffer, strlen(buffer));
                    }
                }
            }
            //printf("Jogo\n");
            sleep(1);
        }
    } while (gameOver != 1);

    //Envia aviso de fim de transmissao
    if (gameOver != 0) {
        n = sizeof(FIM);
        if (write(newsocketfd, FIM, n) != n) {
            error("Funcao server: erro no envio do fim de transmissao pelo socket");
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

char * TutorialHelp() {
  // Sem \n no printf não exibe msg no servidor
  printf("Tutorial / Help Enviado\n");
  char * aux = malloc(MAXBUFF);
  strncpy(aux, "2.\n\n\t \t Comandos validos:\n\n \
  \t Qualquer duvida do que fazer digite 'ajuda',\n \
  \t você pode usar o comando 'olhar' para saber \n \
  \t o que está vendo, para olhar algo de perto \n \
  \t digite 'examinar' e o 'objeto', por exemplo \n \
  \t 'examinar pia', após examinar algo acontece, \n \
  \t você pode se mover pela cela com 'mover' \n \
  \t ex. 'mover pia', para sair do jogo 'sair'!\n\n", MAXBUFF);
  return aux;
}

char * Sair() {
    char * aux = malloc(MAXBUFF);
    strncpy(aux, "1", MAXBUFF);
    return aux;
    return aux;
}

char * Teste() {
    char * aux = malloc(MAXBUFF);
    strncpy(aux, "É bem possível", MAXBUFF);
    return aux;
}

void Initialize() {
  localAtual = Cama1;
}

char * Iniciar() {
  // Sem \n no printf não exibe msg no servidor
  printf("Iniciar jogo Enviado\n");
  char * aux = malloc(MAXBUFF);
  strncpy(aux, "1.\n\n\t \t \t Jogo Iniciado\n\n \
  \t Após a visita de um camarada, você soube que mandaram\n \
  \t esconder todas as chaves do presídio na sua cela,\n \
  \t mas como os créditos do celular do camarada acabaram,\n \
  \t o telefonema com o líder político foi interrompida, \n \
  \t cabe a você encontrar as chaves e escapar da prisão.\n\n", MAXBUFF);
  return aux;
}

char * Olhar() {
    char * aux = malloc(MAXBUFF);

    if (localAtual == Cama1) {
        printf("Enviado o Local Atual - Cama, descricao do que vê e onde pode ir.\n");
        strncpy(aux, "\n\t Você está sentado na cama da beliche, \n \
        olhando para uma parede cheia de coisas, \n \
        a sua esquerda está a grade da cela, a sua \n \
        direita há uma pia e uma privada, seu \n \
        companheiro de cela, está, na cama de cima da  \n \
        beliche dormindo.\n \n", MAXBUFF);
        return aux;
    }

    if (localAtual == CamaBaixo) {
        printf("Enviado o Local Atual - Embaixo da Cama, descricao do que vê e onde pode ir.\n");
        strncpy(aux, "\n\t Você está embaixo da beliche, é \n \
        muito apertado você mal consegue se mover, \n \
        a baixo está o 'banheiro' e acima as grades,\n \
        você vê algo estranho no canto, um broche. \n \n", MAXBUFF);
        return aux;
    }

    if (localAtual == Grade) {
        printf("Enviado o Local Atual - Grade, descricao do que vê e onde pode ir.\n");
        strncpy(aux, "\n\t Você está debruçado nas grades, \n \
        dá para ver o corredor, todos estão nas celas. \n \n", MAXBUFF);
        return aux;
    }

    if (localAtual == Privada) {
        printf("Enviado o Local Atual - Privada, descricao do que vê e onde pode ir.\n");
        strncpy(aux, "\n\t Você está em frente a privada, \n \
        a um odor horrível saindo pela privada.\n \n", MAXBUFF);
        return aux;
    }

    if (localAtual == Pia) {
        printf("Enviado o Local Atual - Pia, descricao do que vê e onde pode ir.\n");
        strncpy(aux, "\n\t Você está de frente para a pia, \n \
        há um espelho a sua frente e algo saindo pelo cano.\n \n", MAXBUFF);
        return aux;
    }
}
/*
Parede
Cama
Broche
Corredor
Privada
Espelho
Pia
*/
char * Examinar(Objetos obj) {
    char * aux = malloc(MAXBUFF);

    // Cama
    if (localAtual == Cama1 && obj == Parede) {
      strncpy(aux, "\t Fotos de família e de famosos.\n\n", MAXBUFF);
      return aux;
    }
    if (localAtual == Cama1 && obj == Cama) {
      strncpy(aux, "\t A cama é tão dura quanto concreto.\n\n", MAXBUFF);
      return aux;
    }

    // Embaixo da Cama
    if (localAtual == CamaBaixo && obj == Broche) {
      strncpy(aux, "\t Examinar Parede funcionou\n\n", MAXBUFF);
      return aux;
    }

    // Grade
    if (localAtual == Grade && obj == Corredor) {
      strncpy(aux, "\t Muitos guardas patrulhando a área.\n\n", MAXBUFF);
      return aux;
    }

    // Privada
    if (localAtual == Privada && obj == PrivadaObj) {
      strncpy(aux, "\t Meu companheiro está preparando saquê.\n\n", MAXBUFF);
      return aux;
    }

    // Pia
    if (localAtual == Pia && obj == Espelho) {
      strncpy(aux, "\t Como você é feio, o tempo e a balança não ajudaram.\n\n", MAXBUFF);
      return aux;
    }
    if (localAtual == Pia && obj == PiaObj) {
      strncpy(aux, "\t Pia muito suja, enferrujada e vermes saindo pelo cano.\n\n", MAXBUFF);
      return aux;
    }
}

char * Mover(Local loc) {
    char * aux = malloc(MAXBUFF);

    if(localAtual != loc) {
        localAtual = loc;

        switch (loc) {
            case Cama1:
                printf("Moveu Cama1\n");
                strncpy(aux, "\tVocê está na sua cama.\n\n", MAXBUFF);
                return aux;
            break;

            case CamaBaixo:
                printf("Moveu CamaBaixo\n");
                strncpy(aux, "\tVocê está na embaixo da cama.\n\n", MAXBUFF);
                return aux;
            break;

            case Grade:
                printf("Moveu Grade\n");
                strncpy(aux, "\tVocê está de frente para a grade.\n\n", MAXBUFF);
                return aux;
            break;

            case Privada:
                printf("Moveu Privada\n");
                strncpy(aux, "\tVocê está de frente para a privada.\n\n", MAXBUFF);
                return aux;
            break;

            case Pia:
                printf("Moveu Pia\n");
                strncpy(aux, "\tVocê está de frente para a pia.\n\n", MAXBUFF);
                return aux;
            break;
          }
      } else {
            printf("Não se moveu\n");
            strncpy(aux, "\tVocê já está aqui.\n\n", MAXBUFF);
            return aux;
      }
    /*
    if (localAtual == Pia) {
      strncpy(aux, "Moveu para pia\n\n", MAXBUFF);
      return aux;
    }
    */
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
