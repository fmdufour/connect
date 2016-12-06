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
#define LLS 0
#define LCD 1
#define  CD 2
#define  LS 3
#define PUT 4
#define GET 5
#define INVALIDO 6
#define LIST 7
#define ALL 8
#define EXIT 9



typedef struct _pacote {
	unsigned char init;
	unsigned char size;
	unsigned char seq;
	unsigned char type;
	unsigned char *data;
	unsigned char par;
} *pacote;

typedef struct _comando{
	int tipo;
	int opcao;
	char *arq;
} comando;




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