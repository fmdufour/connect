#include "main.h"
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

	p = recebe_pacote(socket);
	
}

