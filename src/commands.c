#include "main.h"



void cd(int socket, comando *cmd){

	pacote *p = monta_pacote(CD, (unsigned char*)cmd->arq, strlen(cmd->arq));

	if(!envia_pacote(socket, p))
		return;

	free(p);

	if((p = recebe_pacote(socket, 0)) == NULL)
		return;

	if(p->tipo == OK){
		printf("CD realizado para %s\n", cmd->arq);
	}
	else if(p->tipo == ERR){
		if(p->dados[0] == '1')
			printf("A pasta %s nao existe no servidor\n", cmd->arq);
	}

	free(p);
}

void lcd(comando *cmd) {
	if ((chdir ((char*) cmd->arq)) == 0) {
	} else {
			printf ("Diretorio %s nao existe\n", cmd->arq);
	}

}

char *get_ls_string(DIR *d, int lista, int todos){
	struct stat statbuf;
	struct dirent *ent;
	int j = 0;

	//starta buff com \0 para sprintf identificar como string
	int tamTotal = 1;
	char *string_ls;
	string_ls = malloc(sizeof(char)*1);
	string_ls[0] = '\0';

  	if (d){
	    while ((ent = readdir(d)) != NULL){
    		//se nao estiver marcado o all no ls retira os arquivos ocultos
    		if(!(todos) &&
    			(!strcmp(ent->d_name, "..")
    			|| ent->d_name[0] == '.'))
    				continue;

    		if(stat(ent->d_name,&statbuf) < 0){
				printf("erro ao recuperar atributos arquivo\n");
				return;
    		}

    		if(lista || todos){
    			tamTotal += strlen(get_permissoes_arq(statbuf));
    			//adiciona 1 para o tamanho de links get_links()
    			tamTotal += 1;
    			tamTotal += strlen(get_owner(statbuf));
    			tamTotal += strlen(get_group(statbuf));
    			//adiciona tamanho dos espacos brancos
    			tamTotal += 4;

    			string_ls = realloc(string_ls, sizeof(char) * tamTotal);

    			sprintf(string_ls,"%s%s %d %s %s ",
    				string_ls,
	      			get_permissoes_arq(statbuf) ,
	      			get_links(statbuf),
	      			get_owner(statbuf),
	      			get_group(statbuf));

    			    //adiciona espacos para formatar o tamanho do arquivo
		    	while(j++ < 11 - strlen(get_file_size(statbuf))){
		    		tamTotal += 1;
		    		string_ls = realloc(string_ls, sizeof(char) * tamTotal);
		    		sprintf(string_ls,"%s ",string_ls);

		    	}
		    	//add tamanho dos espacos e\n
		    	tamTotal += 4;
		    	tamTotal += strlen(get_file_size(statbuf));
		    	tamTotal += strlen(get_date(statbuf));
		    	tamTotal += strlen(ent->d_name);

		    	string_ls = realloc(string_ls, sizeof(char) * tamTotal);

		    	sprintf(string_ls, "%s %s %s %s\n",
		    		string_ls,
		    		get_file_size(statbuf),
	      			get_date(statbuf),
	      			ent->d_name);

		    	j = 0;
    		}
    		else{
    			tamTotal += strlen(ent->d_name);
    			tamTotal += 4;
    			string_ls = realloc(string_ls, sizeof(char) * tamTotal);
    			sprintf(string_ls,"%s%s    ",string_ls, ent->d_name);
    		}
    	}
    	//coloca uma quebra de linha caso já nao tenha colocado
    	if (!lista && !todos){
    		tamTotal +=1;
	    	string_ls = realloc(string_ls, sizeof(char) * tamTotal);

    		sprintf(string_ls,"%s\n",string_ls);
    	}

   		return string_ls;
	}
}

void lls(comando *cmd) {
	DIR *d;
	int j = 0;

  	d = opendir(".");

  	if(d){
  		int todos = cmd->opcao == ALL ? 1 : 0;
  		int lista = cmd->opcao == LIST ? 1 : 0;

  		printf("%s", get_ls_string(d, lista, todos));

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
		|| cmd->tipo == LCD || cmd->tipo== CD)
			&& cmd->arq == NULL)
		cmd->tipo = INVALIDO;

	return cmd;
}

void server_ls(int socket, unsigned char* dados){
	int lista = 0;
	int todos = 0;
	int quebra = 0;
	int partes = 0;
	int tam_total = 0;
	int i = 0;
	int j = 0;
	char *parte;
	char *resul;
	DIR *d;
	pacote *p;

	//testa se ls é ls -l
	if(dados[0] == 'l'){
		lista = 1;
	}
	else if(dados[0] == 'a'){
		todos = 1;
	}

	d = opendir(".");

	if(d){

		resul = get_ls_string(d, lista, todos);

		tam_total = strlen(resul);

		if(tam_total > MAX_DADOS){
			quebra = 0;
			tam_total = strlen(resul);
			partes = tam_total / MAX_DADOS + (tam_total % MAX_DADOS == 0 ? 0 : 1);

			for(j = 0; j < partes; j++){

				i = 0;

				parte = malloc(MAX_DADOS + 1);

				//quebra de 63 em 63 para envio
				while(quebra < tam_total && i < MAX_DADOS){
					parte[i] = resul[quebra];
					quebra++;
					i++;
				}

	      p = monta_pacote(MOSTRA, parte, strlen(parte));

				if(!envia_pacote(socket, p))
					return;

				free_packet(p);
			}

		}else{
			p = monta_pacote(MOSTRA, resul, strlen(resul));

			if(!envia_pacote(socket, p))
				return;

			//free_packet(p);
		}
		p = monta_pacote(FIM_TXT, NULL, 0);

		if(!envia_pacote(socket, p))
			return;

		//free_packet(p);

		closedir(d);
	}

}

void server_cd(int socket, unsigned char* dados){
	pacote *p;
	char *cd_erro = malloc(sizeof(char));

	printf("cd para -> %s\n", dados);

	p = monta_pacote(0, cd_erro, 1);

	if ((chdir ((char*) dados)) == 0) {
		cd_erro[0] = ' ';
		p->tipo = OK;

	} else {
		cd_erro[0] = '1';
		p->tipo = ERR;

	}

	if(!envia_pacote(socket, p))
		return;
}


void ls(int socket, comando *cmd){
	pacote *p;
	char *opcao = malloc(sizeof(char));

	if(cmd->opcao == LIST)
		opcao[0] = 'l';
	else if(cmd->opcao == ALL)
		opcao[0] = 'a';
	else
		opcao[0] = ' ';

	p = monta_pacote(LS, opcao, strlen(opcao));

	if(!envia_pacote(socket, p))
		return;

	free(p);

	if((p = recebe_pacote(socket, 0)) == NULL)
		return;

	do{
		printf("%s", p->dados);
		free_packet(p);

		if((p = recebe_pacote(socket, 0)) == NULL)
			return;
	}while(p->tipo == MOSTRA);


	if(!p->tipo == FIM_TXT)
		printf("erro no final do LS\n");

}


void put(int socket, unsigned char *nome_arq){

	long int tam_arq = 0;
	FILE *fp;
	pacote *p;
	int tam_parte = 0;
	int partes = 0;
	char *parte;
	int j = 0;

	parte = malloc(MAX_DADOS);

  if ((fp = fopen (nome_arq, "r")) == NULL) {
    printf("Arquivo %s nao existe\n",nome_arq);
    return;
  }

	fseek (fp, 0, SEEK_END);
	tam_arq = ftell(fp);

	p = monta_pacote(PUT, (unsigned char*) nome_arq, strlen (nome_arq));

	if(!envia_pacote(socket, p))
		return;

	rewind (fp);

	//free_packet(p);

	while (1) {
		//pega o que resta do arquivo
		tam_parte = tam_arq - ftell(fp);

		if (tam_parte <= 0)
			break;

		if (tam_parte > MAX_DADOS)
			tam_parte = MAX_DADOS;

		fread (parte, 1, tam_parte, fp);

		p = monta_pacote(DADOS, parte, tam_parte);

		envia_pacote(socket, p);

	}

	fclose (fp);

	free(parte);

	p = monta_pacote(FIM_TXT, &tam_arq,1);

	if(!envia_pacote(socket, p))
		return;
}

void get(int socket, unsigned char* nome_arq){
	pacote *p;
	p = monta_pacote(GET,
		(unsigned char*) nome_arq, strlen(nome_arq));

	if(!envia_pacote(socket, p))
		return;

	recebe_arquivo(socket, nome_arq);
}

void recebe_arquivo(int socket, unsigned char *nome_arq) {

	long int tam_arq;
	pacote *p;

	FILE* fp;


  if ((fp = fopen ((char*) nome_arq, "w")) == NULL) {
      printf ("Nao eh possivel criar o arquivo %s\n",nome_arq);
      return;
  }


	while (1) {
		if((p = recebe_pacote(socket, 0)) == NULL)
			return;

		if (p->tipo == DADOS)
			fwrite (p->dados, 1, p->tam, fp);
		else if (p->tipo == FIM_TXT) {
			break;
		}

		free_packet(p);
	}

  fseek (fp, 0, SEEK_END);

	printf ("Arquivo %s transferido com sucesso\n", nome_arq);

	fclose (fp);

}
