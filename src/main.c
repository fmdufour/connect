#include "main.h"

int main(int argv, char **argc){
	int modo_exec;
	
	if (DEBUG){	
		for (int i=0; i < argv; i++){
			printf("arg %d: %s\n", i, argc[i]);
		}
	}
	

	if(argv < 2 || !(modo_exec = modo_exec_valido(argc)) ){
		printf("Informe o modo de execução (-client | -server)\n");
		return 1;
	}


	if(modo_exec == CLIENT){
		start_client();
	}
	else if(modo_exec == SERVER){
		start_server();
	}	

	return 0;
}