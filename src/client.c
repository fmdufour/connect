#include "main.h"


void start_client(){
	char cwd[1024];
	comando *cmd;
	int socket;


	socket = socket_create(DEVICE);

	printf("Client conectado...\n");
	
	print_options();
	      		
	while(1){
		get_diretorio_atual(cwd);

		printf("%s$ ", cwd);
		
		cmd = get_comando();

		switch(cmd->tipo){
			case LS:
				ls(socket, cmd);
				break;
			case CD:
				cd(socket, cmd);
				break;
			case LLS:
				lls(cmd);
				break;
			case LCD:
				lcd(cmd);
				break;
			case GET:
				get(socket, cmd);
				break;
			case PUT:
				put(socket, cmd->arq);				
				break;
			case INVALIDO:
				printf("Comando invalido\n");
				break;
		}

	}
}