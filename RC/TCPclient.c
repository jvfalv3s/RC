`/**********************************************************************
* CLIENTE liga ao servidor (definido em argv[1]) no porto especificado
* (em argv[2]), escrevendo a palavra predefinida (em argv[3]).
* USO: >cliente <enderecoServidor>  <porto>  <Palavra>
**********************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

void erro(char *msg);

int main(int argc, char *argv[]) {
    char buffer[1024];
    char endServer[100];
    int fd;
    struct sockaddr_in addr;
    struct hostent *hostPtr;
    
    if (argc != 3) {
        printf("cliente <host> <port> \n");
        exit(-1);
    }
    //Copia o ip -> endServer
    strcpy(endServer, argv[1]);
    printf("Server IP: %s\n",endServer);
    
    //Converte nome para endereço
    if ((hostPtr = gethostbyname(endServer)) == 0)
        erro("Não consegui obter endereço");
    
    bzero((void *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;//(para IPV4)
    addr.sin_addr.s_addr = ((struct in_addr *)(hostPtr->h_addr))->s_addr;
    
    //Host to Network Short
    addr.sin_port = htons((short) atoi(argv[2]));
    
    if ((fd = socket(AF_INET,SOCK_STREAM,0)) == -1)
        erro("socket");
    if (connect(fd,(struct sockaddr *)&addr,sizeof (addr)) < 0)
        erro("Connect");
        
    //Espera para o user escrever algo
    while(1){
    printf("Waiting for input\n");
    fgets(buffer,sizeof(buffer),stdin);
    size_t len = strlen(buffer);
    if (buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    write(fd, buffer,strlen(buffer));
    if(strcmp(buffer,"SAIR")== 0){
        break;
        }
    }
    close(fd);
    exit(0);
    }

void erro(char *msg) {
    printf("Erro: %s\n", msg);
    exit(-1);
}