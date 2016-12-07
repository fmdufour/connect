#include "main.h"



void cd(int socket, comando *cmd){
	printf("Executando comando CD -> Remoto\n");

	pacote *p = monta_pacote(CD, (unsigned char*)cmd->arq, sizeof(cmd->arq));

	printf("Montou pacote\n");

	envia_pacote(socket, p);

	printf("CD: pacote enviado\n");
}

void lcd(comando *cmd) {	
	if ((chdir ((char*) cmd->arq)) == 0) {		
	} else {
			printf ("Diretorio %s nao existe\n", cmd->arq);
	}

}

void lls(comando *cmd) {
	DIR *d;
	struct dirent *ent;
	int i = 0;
	int j = 0;
	char *perm;
	struct stat statbuf;

  	d = opendir(".");

  	if (d){
	    while ((ent = readdir(d)) != NULL){
    		//se nao estiver marcado o all no ls retira os arquivos ocultos
    		if(!(cmd->opcao == ALL) &&
    			(!strcmp(ent->d_name, "..")
    			|| ent->d_name[0] == '.')) 
    				continue;    		    	

    		if(stat(ent->d_name,&statbuf) < 0){				
				printf("erro ao recuperar atributos arquivo\n");    			
				return;
    		}

    		if(cmd->opcao == LIST || cmd->opcao == ALL){
    			printf("%s %d %s %s ", 
	      			get_permissoes_arq(statbuf) ,
	      			get_links(statbuf), 
	      			get_owner(statbuf),
	      			get_group(statbuf));
	      			
    			    //adiciona espacos para formatar o tamanho do arquivo
		    	while(j++ < 11 - strlen(get_file_size(statbuf)))
		    		printf(" ");

		    	printf(" %s %s %s\n",
		    		get_file_size(statbuf),
	      			get_date(statbuf),
	      			ent->d_name);

		    	j = 0;
    		}
    		else{
    			printf("%s    ", ent->d_name);
    			i++;      			
    		}
    	}
    	//coloca uma quebra de linha caso já nao tenha colocado
    	if (cmd->opcao < LIST) printf("\n");
    	closedir(d);
  	}  	
}





comando* get_comando(){
	comando *cmd;
	char *entrada;	
	char *com;	

	entrada = malloc(sizeof(char) * 1024);

	gets(entrada);

	cmd = malloc(sizeof(comando));
		
	com = strtok(entrada, " ");	

	com = trimwhitespace(com);	

	if(!strcmp(com, "lls"))
		cmd->tipo = LLS;		
	else if(!strcmp(com, "lcd"))
		cmd->tipo = LCD;
	else if(!strcmp(com, "cd")){
		cmd->tipo = CD;		
	}
	else if(!strcmp(com, "ls")){
		cmd->tipo = LS;
	}
	else if(!strcmp(com, "put"))
		cmd->tipo = PUT;
	else if(!strcmp(com, "get"))
		cmd->tipo = GET;
	else if(!strcmp(com, "exit"))
		cmd->tipo = EXIT;
	else{		
		cmd->tipo = INVALIDO;		
		return cmd;
	}		
	cmd->opcao = 0;
	cmd->arq = NULL;
	com = strtok(NULL, " ");
	
	if(com){
		com = trimwhitespace(com);							

		if(!strcmp(com, "-l"))
			cmd->opcao = LIST;
		else if(!strcmp(com, "-la"))
			cmd->opcao = ALL;
		else if((cmd->tipo == GET || cmd->tipo == PUT 
				|| cmd->tipo == LCD || cmd->tipo== CD))
			cmd->arq = com;						
	}

	//se pediu get ou put e não informou o arquivo
	if((cmd->tipo == GET || cmd->tipo == PUT 
		|| cmd->tipo == LCD || cmd->tipo== CD) && cmd->arq == NULL)
		cmd->tipo = INVALIDO;			

	return cmd;
}