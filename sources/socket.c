#include "../includes/ft_traceroute.h"

void create_socket() {
	int sockFd;
	int protocol = IPPROTO_ICMP;

	struct timeval tval;
	ft_memset(&tval, 0, sizeof(tval));
	tval.tv_sec = 1;
	tval.tv_usec = 0;

	sockFd = socket(AF_INET, SOCK_RAW, protocol);
	// !! needs sudo privileges to create raw socket
	if (sockFd < 0) {
		perror("socket");
		exit(1);
	}
	protocol = IPPROTO_IP;
	setsockopt(sockFd, protocol, IP_HDRINCL, (int[1]){1}, sizeof(int));
	setsockopt(sockFd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tval, sizeof(tval));

	data.sockFd = sockFd;
}

