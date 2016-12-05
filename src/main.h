#include <stdio.h>
#include <string.h>

#define DEBUG 1
#define CLIENT 1
#define SERVER 2
#define True 1
#define False 0


typedef struct _pacote {
	unsigned char init;
	unsigned char size;
	unsigned char seq;
	unsigned char type;
	unsigned char *data;
	unsigned char par;
} *pacote;

void print_help();
void print_options_server();
void print_options_client();
int modo_exec_valido(char **argc);
void start_server();
void start_client();
