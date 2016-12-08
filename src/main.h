#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <linux/if.h>
#include <netinet/in.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdint.h>


#define DEBUG 1
#define CLIENT 1
#define SERVER 2
#define True 1
#define False 0

#define DEVICE "lo"


//Comandos
#define LLS 55
#define LCD 56
#define INVALIDO 57
#define LIST 58
#define ALL 59
#define EXIT 60

//tipos pacote
#define ACK  0
#define NACK 1
#define OK   2
#define CD   10
#define LS   11
#define PUT  12
#define PUTM 13
#define GET 14
#define NOME_ARQ 22
#define DADOS 5
#define ERR  31
#define MOSTRA 20
#define FIM_TXT  7
#define FIM_TXT_GRP 8
//marca inicio mensagem
#define MARCA 126
#define MAX_DADOS 63
#define MAX_SEQ 31
#define MAX_PACOTE 67

typedef struct _comando{
	int tipo;
	int opcao;
	char *arq;
} comando;

typedef struct pacote{
	unsigned char marca;
	unsigned char tam;
	unsigned char seq;
	unsigned char tipo;
	unsigned char *dados;
	unsigned char crc;
}pacote;

comando* get_comando();
void lcd(comando *cmd);
void lls(comando *cmd);

void start_server();
void start_client();


//UTIL
void get_diretorio_atual(char* cwd);
char* trimwhitespace(char *str);
char* get_permissoes_arq(struct stat statbuf);
int get_links(struct stat statbuf);
char* get_owner(struct stat statbuf);
char* get_group(struct stat statbuf);
char* get_file_size(struct stat statbuf);
char* get_date(struct stat statbuf);

void print_help();
void print_options();
int modo_exec_valido(char **argc);

//SOCKET
int socket_create (const char *device);

//PROTOCOL
//pacote *monta_pacote(int tipo, char *dados, int tamanho);
pacote* recebe_pacote(int socket);
void recebe_arquivo(int socket, unsigned char * nome_arq);
