#include "main.h"



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

  	d = opendir(".");

  	if (d)
  	{
	    while ((ent = readdir(d)) != NULL)
    	{
    		if(!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
    			continue;


    		i++;
      		printf("%s %d %s    ", get_permissoes_arq(ent->d_name),get_links(ent->d_name), ent->d_name);

      		if(i % 4 == 0) printf("\n");
    	}
    	//coloca uma quebra de linha caso já nao tenha colocado
    	if (i % 4 != 0) printf("\n");

    	closedir(d);
  	}  	


}





comando* get_comando(){
	comando *cmd;
	char entrada[1024];	
	char *com;	
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
		return cmd;
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