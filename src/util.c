#include "main.h"

void printHelp(){
	printf("Digite os parametros no seguinte formato:\n");
	printf("Digite os parametros no seguinte formato:\n");
	printf("Digite os parametros no seguinte formato:\n");
	printf("Digite os parametros no seguinte formato:\n");
	printf("Digite os parametros no seguinte formato:\n");
	printf("Digite os parametros no seguinte formato:\n");
	printf("Digite os parametros no seguinte formato:\n");
	
}

int modo_exec_valido(char **argc){
	if(strcmp(argc[1], "-client") == 0)
		return CLIENT;
	if(strcmp(argc[1], "-server") == 0)
		return SERVER;

	return False;
}