//modelo de servidor como escrito na ficha 3

#include <sys/types.h>
#include <sys/socket.h>

/*Cria um novo socket*/

int socket(int domain, int type, int protocol);

int bind(int fd, const struct sockaddr *address, socklen_t address_len);

/*Estrutura que contém um endereço de internet*/
struct sockaddr {
    short sin_family; // AF_INET
    unsigned short sin_port; //porto a associar
    struct in_addr sin_addr; // INADDR_ANY = qualquer endereço do host

    char sin_zero; //padding, deixar em branco
};

/*Estrutura que contém um endereço de internet*/
struct in_addr {
    unsigned long s_addr;
};

/*Coloca um socket à escuta de ligações*/
int listen(int fd, int backlog);

/*Aceita uma ligação num socket*/
int accept(int fd, const struct sockaddr *address, socklen_t *address_len);

/* Inicia uma ligação num socket*/
int connect(int fd, const struct sockaddr *address, socklen_t address_len);

/* Converte nome para endereço*/
struct hostent *gethostbyname(const char *name);

struct hostent {
    char *h_name; //nome oficial do host
    char **h_aliases; //lista de aliases
    int h_addrtype; //tipo de endereço (ex.: AF_INET6)
    int h_length; //comprimento do endereço
    char **h_addr_list; //lista de endereços terminada com null 
};

/* lst addr, net byte order*/
#define h_addr h_addr_list[0]

#include <arpa/inet.h>

uint32_t htonl(uint32_t hostlong);
uint16_t htons(uint16_t hostshort);
uint32_t ntohl(uint32_t netlong);
uint16_t ntohs(uint16_t netshort);

/* Converte em endereço IPv4 para uma string com formato xxx.xxx.xxx.xxx = n(network) to p(presentation)*/
const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);

/* Converte uma string com o endereço IP num valor para a estrutura struct in_addr ou struct in6_addr => p(presentation) to n(netework) */
int inet_pton(int af, const char *src, void *dst);
