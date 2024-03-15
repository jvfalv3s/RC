#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<netdb.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>

#define SERVER_PORT 9000
#define BUF_SIZE 1024

void process_cliennt(int fd);
void erro(char *msg);

int main(){
    int fd, client;
    struct sockaddr_in addr, client_addr;
    int client_addr_size;
    int client_number = 0;

    bzero((void *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(SERVER_PORT);

    if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        erro("na funcao socket");
    if(bind(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0)
        erro("na funcao bind");
    if(listen(fd, 5) < 0)
        erro("na funcao listen");
    client_addr_size = sizeof(client_addr);

    while(1){
        //clean child processes, avoiding zombies
        //must use WNOHANG or would block whenever a child process was still working
        while(waitpid(-1, NULL, WNOHANG) > 0);
        //wait for new connection
        client = accept(fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_size);
        if(client > 0){
            client_number++;
            printf("Cliente conectado no endereço IP: %s, Porto: %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            printf("Cliente numero %d conectado\n", client_number);
            if(fork() == 0){
                close(fd);
                process_cliennt(client, client_addr, client_number);
                close(client);
                exit(0);
            }
            close(client);
        }
    }

    return 0;
}

void process_client(int client_fd){
    int nread = 0;
    char buffer[BUF_SIZE];
    char message[BUF_SIZE];

    //constroi a mensagem a ser enviada para o cliente
    sprintf(message, "Endereco IP: %s\nPorto: %d\nNumero de clientes: %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), getpid());
    write(client_fd, message, strlen(message));

    do {
        nread = read(client_fd, buffer, BUF_SIZE-1);
        buffer[nread] = '\0';
        printf("%s", buffer);
        fflush(stdout);
    } while (nread > 0);
    close(client_fd);
}

void erro(char *msg){
    printf("Erro: %s\n", msg);
    exit(-1);
}


//funcionalidades a acrescentar : O servidor escreve na consola o endereço IP e o porto do cliente que lhe está a ligar; dá também um número a cada cliente novo que liga;

// O servidor devolve ao cliente uma mensagem de texto com o endereço IPv4, o porto do qual o cliente está a ligar e o número de clientes que já estabeleceram ligação.