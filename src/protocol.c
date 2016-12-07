#include "main.h"

int sequencia = 0;

pacote* monta_pacote(int tipo, unsigned char *dados, int tamanho){
	pacote *p;

	p = malloc(sizeof(pacote));

	p->marca = MARCA;
	p->tam = tamanho;
	p->seq = (sequencia % MAX_SEQ) + 1;
	p->tipo = tipo;
	p->dados = dados;
	//p->crc = get_crc(p->dados);	
	return p;
}

unsigned char* bufferiza_pacote(pacote* p){
	unsigned char *buffer;
	int i;

	buffer = malloc(sizeof(unsigned char) * p->tam + (8*4));

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

int envia_pacote(int socket, pacote *p){
	int i = 0;

	printf("antes bufferizar pacote\n");
	unsigned char *buff = bufferiza_pacote(p);

	printf("indo enviar pacote\n");
	if ((send (socket, p, MAX_PACOTE, 0)) == -1) {
		printf("Deu tretaaa\n");
		perror("send");
		//printf("erro: send: %s\n", strerror(errno));
		//usleep (random * PACKET_TIME);
		//continue;
	}
	if(DEBUG)
		printf("Pacote enviado %d\n", p->seq);	

}


pacote* recebe_pacote(int socket){
	pacote p;
	unsigned char *buffer = malloc (MAX_PACOTE);
   
    while (1) {

		if ((recv (socket, buffer, MAX_PACOTE, 0)) < 0) {
			printf("Erro RECV -> recebe_pacote()\n");
			continue;
		}

		printf("Pacote Recebido");
		//p = desbufferiza_pacote(buffer);

	/*		if (p->marca != MARCA)
			continue;

	if (p->seq != _seq)
			continue;

        if (packet_parity(p) != p->par)
            continue;

		if (p->type == TYPE_ACK) {
			packet_seq ();
			return p;
		}

		if (DEBUG) {
			printf("debug: mensagem recebida\t");
			debug_packet (p);
		}

		if (p->type != TYPE_ACK && p->type != TYPE_NACK) {
			packet q = packet_mount (_seq, TYPE_ACK, NULL, 0);
			if (packet_send (socket, q))
				return p;
		}

		*/

	}
}












