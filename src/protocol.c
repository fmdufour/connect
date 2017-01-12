#include "main.h"

unsigned int sequencia = 0;

pacote* monta_pacote(int tipo, unsigned char *dados, int tamanho){
	pacote *p;

	p = (pacote*)malloc(sizeof(pacote));

	p->marca = MARCA;
	p->tam = tamanho;
	p->seq = sequencia;
	p->tipo = tipo;
	p->dados = dados;
	//p->crc = get_crc(p->dados);
	return p;
}

unsigned char* bufferiza_pacote(pacote* p){
	unsigned char *buffer;
	int i;

	buffer = (unsigned char*)malloc(sizeof(unsigned char) * (p->tam + 4));

	buffer[0] = p->marca;
	buffer[1] = p->tam << 2;
	buffer[1] |= p->seq >> 3;
	buffer[2] = p->seq << 5;
	buffer[2] |= p->tipo;

	for(int i=0; i < p->tam; i++){
		buffer[i+3] = p->dados[i];
	}
	//CRC
	//buffer[i+4] = "0xFF";
	return buffer;
}

pacote* desbufferiza_pacote(unsigned char *buffer){
	int i;
	pacote *p = (pacote*)malloc(sizeof(pacote));
	p->dados = -1;
	p->marca = buffer[0];
	p->tam = buffer[1] >> 2;
	p->seq = buffer[1] << 3 & 0x1F;
	p->seq |= buffer[2] >> 5;
	p->tipo = buffer[2] & 0x1F;

	p->dados = (unsigned char*)malloc(p->tam);
	for(int i=0; i < p->tam; i++){
		p->dados[i] = buffer[i+3];
	}

	//CRC
	//buffer[i+4] = "0xFF";
	return p;
}

void print_pacote(pacote *p){
	printf("Marca %s\n", int2bin(p->marca, 8));
	printf("Tamanho %s\n", int2bin(p->tam, 8));
	printf("Sequencia %s\n", int2bin(p->seq, 8));
	printf("Tipo %s\n", int2bin(p->tipo, 8));
	printf("----\n");
}

int envia_pacote(int socket, pacote *p){
	int i = 0;

	pacote *resp;

	unsigned char *buff = bufferiza_pacote(p);

	if ((send (socket, buff, MAX_PACOTE, 0)) == -1) {
		perror("send");
	}

	if (p->tipo == ACK)
		sequencia = (sequencia%MAX_SEQ) + 1;

	if (p->tipo == ACK || p->tipo == NACK)
		return 1;

	if ((resp = recebe_pacote(socket, 0)) == NULL) {
		//Timeout resposta
		return False;
	}

	if (resp->tipo == NACK) {
		printf("erro: packet_recv: nack retornado\n");
	}

	return True;
}


pacote* recebe_pacote(int socket, int server){
	pacote *p;
	unsigned char *buffer = malloc (MAX_PACOTE * sizeof(char));
	int tentativas = 0;

	while (1) {
		//printf("Esperando pacote...\n");
		if ((recv (socket, buffer, MAX_PACOTE, 0)) < 0) {
			if (server){
				//se for o servidor deve ignorar o timeout de espera
				continue;
			}

			printf("%da Tentativa de envio Falhou\n", ++tentativas);

			if(tentativas == TENTATIVAS_TIMEOUT){
				printf("Envio falhou após %d tentativas...\n", tentativas);
				return NULL;
			}
			continue;
		}
			//printf("Pacote Recebido\n");
		p = desbufferiza_pacote(buffer);
			//print_pacote(p);
		if (p->marca != MARCA){
	  	free_packet(p);
			continue;
	  }
		if (p->seq != sequencia){
			free_packet(p);
			continue;
		}
	        //if (packet_parity(p) != p->par)
        //    continue;
		if (p->tipo == ACK) {
			sequencia = (sequencia%MAX_SEQ) +1;
			return p;
		}
		if (p->tipo != ACK && p->tipo != NACK) {
			//envia pacote para sinalizar aceitacao
			pacote *resp = monta_pacote(ACK, NULL, 0);
			if (envia_pacote(socket, resp))
				return p;
		}
	}
}
