#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

#define BUFLEN 512 //Tamanho do buffer
#define PORT 9876 //Porto para recepção das mensagens

void erro(char *s){
    perror(s);
    exit(1);
}

//Funções para lidar com usuarios

void addUser(int num){
    printf("Adicionando usuario %d\n", num);
}

void delUser(int num){
    printf("Removendo usuario %d\n", num);
}

void listUsers(){
    printf("Listando usuarios\n");
}

void quitServer(){
    printf("Encerrando servidor\n");
}

void login(int num){
    printf("Logando usuario %d\n", num);
}

int main(void){
    
    struct sockaddr_in si_minha, si_outra;
    int s, recv_len;
    socklen_t slen = sizeof(si_outra);
    char buf[BUFLEN];

    //Cria um socket para recepção de pacotes UDP
    if((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1){
        erro("Erro na criação do socket");
    }

    //Preenchimento da socket address structure
    si_minha.sin_family = AF_INET;
    si_minha.sin_port = htons(PORT);
    si_minha.sin_addr.s_addr = htonl(INADDR_ANY);

    //Associa o socket a informação de endereço
    if(bind(s, (struct sockaddr *)&si_minha, sizeof(si_minha))==-1){
        erro("Erro no bind");
    }
    //Espera recepção de mensagem (a chamada é bloqueante)
    if((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&si_outra, &slen))==-1){
        erro("Erro no recvfrom");
    }
    //Para ignorar o restante conteudo (anterior do buffer)
    buf[recv_len] = '\0';

    printf("Recebi uma mensagem do sistema com o dendreço %s e o porto %d\n", inet_ntoa(si_outra.sin_addr), ntohs(si_outra.sin_port));
    printf("Conteudo da mensagem: %s\n", buf);

    //Convertendo a mensagem para inteiro
    int num = atoi(buf);

    //Chamando as funções de conversão e imprimindo os resultados
    switch(num){
        case 1:
            addUser(num);
            break;
        case 2:
            delUser(num);
            break;
        case 3:
            listUsers();
            break;
        case 4:
            quitServer();
            break;
        case 5:
            login(num);
            break;
        default:
            printf("Comando inválido\n");
            break;
    }


    
    //Fecha o socket e termina o programa
    close(s);
    return 0;
}