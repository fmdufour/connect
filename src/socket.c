# include "main.h"

int socket_create (const char *device) {

	int sock = 0;
	int device_id = 0;
    struct ifreq ifr;
    struct sockaddr_ll sll;

	if((sock = socket(PF_PACKET, SOCK_RAW, 0)) == -1) {		
	    perror("socket");
	    exit(1);
	}

	memset(&ifr, 0, sizeof(struct ifreq));
	memcpy(ifr.ifr_name, device, sizeof(device));
	if(ioctl(sock, SIOCGIFINDEX, &ifr)==-1) {
	    perror("ioctl");
	    exit(1);
	}
	device_id = ifr.ifr_ifindex;

	memset(&sll, 0, sizeof(sll));
	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = device_id;
	sll.sll_protocol = htons(ETH_P_ALL);
	if(bind(sock, (struct sockaddr*)&sll, sizeof(sll)) == -1) {
	    perror("bind");
	    exit(1);
	}


	struct packet_mreq mr;
	memset(&mr, 0, sizeof(mr));
	mr.mr_ifindex = device_id;
	mr.mr_type = PACKET_MR_PROMISC;
	if (setsockopt(sock, SOL_PACKET, PACKET_ADD_MEMBERSHIP,
		&mr, sizeof(mr)) == -1) {
			perror("setsockopt");
			exit(1);
	}

	return sock;
}