#include "../includes/ft_traceroute.h"

t_data data;

void set_signals() {
	signal(SIGALRM, &print_interrupt);
	signal(SIGINT, &print_interrupt);
}

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

unsigned short checksum(void *b, int len)
{
	unsigned short *buf = b;
	unsigned int sum=0;
	unsigned short result;

	for (sum = 0; len > 1; len -= 2)
		sum += *buf++;
	if (len == 1)
		sum += *(unsigned char*)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}

void construct_headers(t_packetData *packet) {
	struct iphdr *ipHdr = &(packet->ipHeader);
	struct icmphdr *icmpHdr = &(packet->icmpHeader);

	ipHdr->ihl = 5;
	ipHdr->version = 4;
	ipHdr->tos = 0;
	ipHdr->tot_len = htons(data.size);
	ipHdr->id = getpid();
	ipHdr->frag_off = htons(0x4000);
	ipHdr->ttl = data.ttl;
	ipHdr->protocol = IPPROTO_ICMP;
	ipHdr->check = 0;
	ipHdr->saddr = INADDR_ANY;
	ipHdr->daddr = data.networkIp->sin_addr.s_addr;

	icmpHdr->type = ICMP_ECHO;
	icmpHdr->code = 0;
	icmpHdr->un.echo.id = getpid();
	// icmpHdr->un.echo.sequence = stats.pingNb;
	icmpHdr->un.echo.sequence = 1;
	icmpHdr->checksum = 0;

	size_t size_pack = data.size + sizeof(struct icmphdr);
	icmpHdr->checksum = checksum(&(packet->icmpHeader), size_pack);
}


void construct_packets() {
	for (int i = 0; i < 3; i++) {
		t_packetData *packet = (t_packetData *)malloc(sizeof(t_packetData) + data.size);
		construct_headers(packet);
		data.sendpack[i] = packet;
		data.sendpack[i]->payload = ft_strnew(data.size);
		printf("itoa? %s\n", ft_itoa(i));
		char *payload = ft_strnew(6);
		strcpy(payload, "AAAA");
		strcpy(payload+4, ft_itoa(i));
		strcpy(packet->payload, payload);
		printf("payload = %s\n", payload);
	}
}

void send_packets() {
	ssize_t sizepack = sizeof(struct icmphdr) + data.size;
	for (int i = 0; i < 3; i++) {
		gettimeofday(&data.sendTime[i], NULL);

		int bytesSent = sendto(data.sockFd, data.sendpack[i], sizepack, 0,
			 (struct sockaddr *)data.networkIp, sizeof(struct sockaddr_in));
		if (bytesSent < 0) {
			dprintf(1, "bytes not sent for packet %d and ttl %d\n", i, data.ttl);
			perror("bytes not sent");
		}
		else {
			printf("bytes sent = %d\n", bytesSent);
		}
	}
}

void init_data(void) {
	data.ttl = 1;
	data.test = "init";
	create_socket();
}

bool recieve_packet(int index) {
	char recieve[data.size];
	struct iovec retMsgData;
	struct msghdr retMsg;

	ft_memset(recieve, 0, data.size);
	ft_memset(&retMsgData, 0, sizeof(retMsgData));
	ft_memset(&retMsg, 0, sizeof(retMsg));

	retMsgData.iov_base = &recieve;
	retMsgData.iov_len = data.size;

	retMsg.msg_iov = &retMsgData;
	retMsg.msg_iovlen = 1;

	ssize_t bytesRecieved = recvmsg(data.sockFd, &retMsg, 0);
	if (bytesRecieved < 0) {
		return false;
	} else {
		printf("===========================================\n");
		printf("we've recieved %lu\n", bytesRecieved);
		print_memory(recieve, bytesRecieved, 16);
		printf("===========================================\n");
	}
	data.retpack[index] = (t_packetData *)malloc(bytesRecieved);
	ft_memcpy(data.retpack[index], recieve, bytesRecieved);
	// data->recievedBytesArray = (char *)malloc(	bytesRecieved);
	// ft_memcpy(data->recievedBytesArray, recieve, bytesRecieved);
	gettimeofday(&data.recieveTime[index], NULL);
	// data->retPrintSize = bytesRecieved;
	return true;
}

void recieve_packets() {
	for (int i = 0; i < 3; i++) {
		recieve_packet(i);
	}
}


int main(int argc, char **argv)
{
	manage_args(argc, argv);
	parse_dest(argc, argv);
	set_signals();
	init_data();
	construct_packets();

	while (data.ttl < 2) {
		send_packets();
		recieve_packets();
		data.ttl++;
	}
	// send packets
	// parse return 
	// display
	// incr ttl to max 30
	

	return EXIT_SUCCESS;
}
