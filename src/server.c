#include "main.h"x
#include <limits.h>

char * int2bin(unsigned i, int tam)
{
    size_t bits = tam;

    char * str = malloc(bits + 1);
    if(!str) return NULL;
    str[bits] = 0;

    // type punning because signed shift is implementation-defined
    unsigned u = *(unsigned *)&i;
    for(; bits--; u >>= 1)
        str[bits] = u & 1 ? '1' : '0';

    return str;
}


void start_server(){
	pacote *p;
	int socket;

	printf("Maquina conectada em modo server\n");

	socket = socket_create(DEVICE);
    while(1){

        p = recebe_pacote(socket, 1);

        switch(p->tipo){
            case LS:
                server_ls(socket, p->dados);
                break;
            case CD:
                server_cd(socket, p->dados);
                break;
            case PUT:
                printf("Recebendo arquivo %s ...\n", p->dados);
                recebe_arquivo(socket, p->dados);
                break;
            case GET:
                printf("Enviando arquivo %s ...\n", p->dados);
                put(socket, p->dados);
                break;

        }
    }
}
