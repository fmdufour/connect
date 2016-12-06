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

void print_options(){
	printf("Comandos disponiveis:\n");
	printf("Comandos disponiveis:\n");
	printf("Comandos disponiveis:\n");
	printf("Comandos disponiveis:\n");

}

int modo_exec_valido(char **argc){
	if(strcmp(argc[1], "-client") == 0)
		return CLIENT;
	if(strcmp(argc[1], "-server") == 0)
		return SERVER;

	return False;
}

void get_diretorio_atual(char* cwd){
	if (getcwd(cwd, 1024) == NULL){
		perror("getcwd() error");	
	}       	   	
}


char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}

char * get_permissoes_arq(char *nome_arq){
    struct stat fileStat;
    char perm[11];

    if(stat(nome_arq,&fileStat) < 0)    
	    return "erro perm ";        

    perm[0] = (S_ISDIR(fileStat.st_mode)) ? 'd' : '-';
    perm[1] = fileStat.st_mode & S_IRUSR ? 'r' : '-';
    perm[2] = fileStat.st_mode & S_IWUSR ? 'w' : '-';
    perm[3] = fileStat.st_mode & S_IXUSR ? 'x' : '-';
    perm[4] = fileStat.st_mode & S_IRGRP ? 'r' : '-';
    perm[5] = fileStat.st_mode & S_IWGRP ? 'w' : '-';
    perm[6] = fileStat.st_mode & S_IXGRP ? 'x' : '-';
    perm[7] = fileStat.st_mode & S_IROTH ? 'r' : '-';
    perm[8] = fileStat.st_mode & S_IWOTH ? 'w' : '-';
    perm[9] = fileStat.st_mode & S_IXOTH ? 'x' : '-';
	perm[10] = '\0';

	return perm;
}

int get_links(char *nome_arq){
	struct stat statbuf;

	if(stat(nome_arq,&statbuf) < 0)
		return 'x';

	return statbuf.st_nlink;
}


