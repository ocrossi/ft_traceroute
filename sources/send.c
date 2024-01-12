#include "../includes/ft_traceroute.h"

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

void construct_headers(t_packetData *packet, int index) {
	struct iphdr *ipHdr = &(packet->ipHeader);
	struct icmphdr *icmpHdr = &(packet->icmpHeader);

	ipHdr->ihl = 5;
	ipHdr->version = 4;
	ipHdr->tos = 0;
	ipHdr->tot_len = htons(data.totalSize);
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
	icmpHdr->un.echo.sequence = (data.ttl == 1) ? data.ttl + index : (data.ttl - 1) * 3 + index ;
	icmpHdr->checksum = 0;
	
	ssize_t sizepack = data.payloadSize + sizeof(struct icmphdr); // problem here header invalid, think its size is bad smh
  printf("size packet in construct header ? %d\n", sizepack);
	icmpHdr->checksum = checksum(&(packet->icmpHeader), sizepack);
}

void construct_packets() {
	for (int i = 0; i < 3; i++) {
    char payload[] = "cmon checksum";
		t_packetData *packet = (t_packetData *)malloc(data.totalSize);
		construct_headers(packet, i);
		data.sendpack[i] = packet;
    ft_memset(data.sendpack[i]->payload, 0, data.payloadSize);
		ft_strcpy(data.sendpack[i]->payload, payload);
		// printf("itoa? %s\n", ft_itoa(i));
		// char *payload = ft_strnew(6);
		// strcpy(payload, "AAAA");
		// strcpy(payload+4, ft_itoa(i));
		// strcpy(data.sendpack[i]->payload, payload);
		// printf("===========================================\n");
		// printf("payload = %s\n", data.sendpack[i]->payload);
		// printf("===========================================\n");
		// printf("full packet\n");
		//   print_memory(data.sendpack[i], data.totalSize, 16);
		// printf("===========================================\n");
  }
}

void send_packets() {
	for (int i = 0; i < 3; i++) {
		gettimeofday(&data.sendTime[i], NULL);

		int bytesSent = sendto(data.sockFd, data.sendpack[i], data.totalSize, 0,
			 (struct sockaddr *)data.networkIp, sizeof(struct sockaddr_in));
		if (bytesSent < 0) {
			dprintf(1, "bytes not sent for packet %d and ttl %d\n", i, data.ttl);
			perror("bytes not sent");
		}
		else {
			// printf("bytes sent = %d\n", bytesSent);
		}
	}
}
