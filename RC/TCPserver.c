#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

#define SERVER_PORT 9000
#define BUF_SIZE 1024

void process_client(int fd,char ***matrix, int num_linhas);
void erro(char *msg);
bool check(char ***matrix, char *dominio, int num_linhas);
char* devolveIP(char ***matrix, char *dominio, int num_linhas);

int main() {
    FILE *f;
    char line[256];
    char *filename = "ficheiro.txt";
    int fd, client;
    struct sockaddr_in addr, client_addr;
    int client_addr_size;
    
    bzero((void *)&addr, sizeof(addr));
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(SERVER_PORT);
    
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        erro("na funcao socket");
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        erro("na funcao bind");
    if (listen(fd, 5) < 0)
        erro("na funcao listen");
    client_addr_size = sizeof(client_addr);
    
    int num_linhas = 0;
    //Abrir o ficheiro
    f = fopen(filename, "r");
    if (f == NULL)
    {
        erro("na leitura do ficheiro");
    }
    //Descobrir o nº de linhas
    while (fgets(line, sizeof(line), f) != NULL)
    {
        num_linhas = num_linhas + 1;
    }
    printf("Numero de linhas: %d\n", num_linhas);
    
    //Vamos criar uma matriz [num_linhas x 2(dominio,ip)]
    char **matrix[num_linhas];
    int indice = 0;
    
    //Vamos precorrer de novo o ficheiro e sacar info para a matriz
    fseek(f, 0, SEEK_SET); // Reset file pointer to the beginning
    while (fgets(line, sizeof(line), f) != NULL){
        if (indice >= num_linhas){
            break;
  	    }
    	matrix[indice] = malloc(2 * sizeof(char*)); // Allocate memory para dominio e IP
    	// Allocate memory para (dominio)
    	matrix[indice][0] = malloc(strlen(line) + 1);
    	strcpy(matrix[indice][0], strtok(line, " "));
    
    	// Allocate memory para  (IP)
    	matrix[indice][1] = malloc(strlen(line) + 1);
    	strcpy(matrix[indice][1], strtok(NULL, " \n"));
    	indice = indice + 1;
    }
    
    while (1){
        //clean finished child processes, avoiding zombies
        //must use WNOHANG or would block whenever a child process was working
        while (waitpid(-1, NULL, WNOHANG) > 0);
        //wait for new connection
        if(listen(fd,5)== -1){
        	erro("Erro no Listening!");
        	}
        
        client = accept(fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_size);
        if (client > 0)
        {
            if (fork() == 0)
            {
                close(fd);
                process_client(client,matrix, num_linhas);
                exit(0);
            }
            close(client);
        }
    }
    return 0;
}

void process_client(int client_fd, char***matrix, int num_linhas){
    int nread = 0;
    char buffer[BUF_SIZE];
    const char *mensagem = "Bem-vindo ao servidor de nomes do DEI. Indique o nome de dominio\n";
    //Envia mensagem de bem-vindo
    printf("Welcome message sent\n");
    printf("%d",client_fd);
    write(client_fd,mensagem,strlen(mensagem));
    fflush(stdout);//flush(stdout);

    do{
        nread = read(client_fd, buffer, BUF_SIZE - 1);
        buffer[nread] = '\0';
        
        //Condição para sair
        if (strcmp(buffer, "SAIR") == 0){
            printf("Ate logo!\n");
            break;
        	}
        if(check(matrix, buffer, num_linhas) == false){
        	printf("O nome de dominio %s não tem associado o endereço ",buffer);
        	}
		if (check(matrix, buffer, num_linhas) == true){
		
			char* ip;
			ip =(char *)malloc(100 * sizeof(char));
			if(ip == NULL){
				erro("Alocação de memória falhada!");
				}
			ip = devolveIP(matrix, buffer,num_linhas);
			printf("O nome de dominio %s tem associado o endereço %s",buffer,ip);
			free(ip);
		}
        }while(nread>0);
    close(client_fd);
    fflush(stdout);   
}

void erro(char *msg)
{
    printf("Erro: %s\n", msg);
    exit(-1);
}

//Vamos verificar se o dominio requirido está na matriz
bool check(char ***matrix, char *dominio, int num_linhas)
{
    for (int i = 0; i < num_linhas; i++)
    {
        if (strcmp(dominio, matrix[i][0]) == 0)
        {
            return true;
        }
    }
    return false;
}
char* devolveIP(char ***matrix, char *dominio, int num_linhas){
	
	for (int i = 0; i < num_linhas; i++){
		if (strcmp(matrix[i][0], dominio)==0){
			printf("Foi encontrado no ficheiro o dominio %s\n",matrix[i][0]);
			printf("Endereço associado é %s\n",matrix[i][1]);
			return strdup(matrix[i][1]);
			
			}
		}
	return NULL;
	}
	