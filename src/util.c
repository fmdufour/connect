#include "main.h"


void print_options(){
	printf("----------------------\n");
	printf("Comandos disponiveis:\n");
	printf("ls [-l | -la]\n");
	printf("cd <pasta>\n");
	printf("lls [-l | -ls]\n");
	printf("lcd <pasta>\n");
	printf("get <arquivo>\n");
	printf("put <arquivo>\n");
	printf("----------------------\n");
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

	if(str == NULL)  // blank?
		return "";

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

char * get_permissoes_arq(struct stat statbuf){
    char *perm;

    perm = malloc(sizeof(char)*11);

    perm[0] = (S_ISDIR(statbuf.st_mode)) ? 'd' : '-';
    perm[1] = statbuf.st_mode & S_IRUSR ? 'r' : '-';
    perm[2] = statbuf.st_mode & S_IWUSR ? 'w' : '-';
    perm[3] = statbuf.st_mode & S_IXUSR ? 'x' : '-';
    perm[4] = statbuf.st_mode & S_IRGRP ? 'r' : '-';
    perm[5] = statbuf.st_mode & S_IWGRP ? 'w' : '-';
    perm[6] = statbuf.st_mode & S_IXGRP ? 'x' : '-';
    perm[7] = statbuf.st_mode & S_IROTH ? 'r' : '-';
    perm[8] = statbuf.st_mode & S_IWOTH ? 'w' : '-';
    perm[9] = statbuf.st_mode & S_IXOTH ? 'x' : '-';
	perm[10] = '\0';

	return perm;
}

int get_links(struct stat statbuf){
	return statbuf.st_nlink;
}

char* get_owner(struct stat statbuf){
	struct passwd  *pwd;

	if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
	    return (char*)pwd->pw_name;
	else
	    return (char*)statbuf.st_uid;
}

char* get_group(struct stat statbuf){
	struct group *grp;

	if ((grp = getgrgid(statbuf.st_gid)) != NULL)
        return grp->gr_name;
    else
        return statbuf.st_gid;

}

char* get_file_size(struct stat statbuf){
	int i;
	char *tam = malloc(sizeof(char)*11);

	sprintf(tam, "%d", statbuf.st_size);

	return tam;
}

char* get_date(struct stat statbuf){
	struct tm      *tm;
	int i = 0;
	char datestring[255];

	char *data = malloc(sizeof(char) * 100);

    tm = localtime(&statbuf.st_mtime);

    /* Get localized date string. */
    strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);

    for(i = 4; i <=15;i++)
    	data[i-4] = datestring[i];

    data[i-4] = '\0';

    //sprintf(data, "%s %s", datestring, dp->d_name);

    return data;
}

void free_packet(struct pacote *p){
	free(p->dados);
	free(p);
}
