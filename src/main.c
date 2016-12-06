#include "main.h"

int main(int argc, char **argv){
	int modo_exec;
	int i;
	
	if (DEBUG){	
		for (i =0; i < argc; i++){
			printf("arg %d: %s\n", i, argv[i]);
		}
	}
	

	if(argc < 2 || !(modo_exec = modo_exec_valido(argv)) ){
		printf("!! Informe o modo de execução (-client | -server) !!\n");
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