#include "../includes/ft_traceroute.h"
#include <netinet/ip.h>
#include <netinet/udp.h>
//
// unsigned short checksum(void *b, int len)
// {
// 	unsigned short *buf = b;
// 	unsigned int sum=0;
// 	unsigned short result;
//
// 	for (sum = 0; len > 1; len -= 2)
// 		sum += *buf++;
// 	if (len == 1)
// 		sum += *(unsigned char*)buf;
// 	sum = (sum >> 16) + (sum & 0xFFFF);
// 	sum += (sum >> 16);
// 	result = ~sum;
// 	return result;
// }

uint16_t checksum(const void* data, size_t len) {
  uint32_t sum = 0;
  for (size_t i = 0; i < len; ++i) {
    sum += *(uint16_t*)((char*)data + i);
  }
  while (sum >> 16) {
    sum = (sum & 0xffff) + (sum >> 16);
  }
  return ~sum;
}

void* construct_packet(int index) {
	struct iphdr ipHdr;
	ipHdr.ihl = 5;
	ipHdr.version = 4;
	ipHdr.tos = 0;
	ipHdr.tot_len = htons(data.totalSize);
	ipHdr.id = getpid();
	ipHdr.frag_off = 0;
	ipHdr.ttl = data.ttl;
	ipHdr.protocol = IPPROTO_UDP;
	ipHdr.check = 0;
	ipHdr.saddr = INADDR_ANY;
	ipHdr.daddr = data.networkIp->sin_addr.s_addr;
	checksum(&ipHdr, sizeof(ipHdr));

	struct udphdr udpHdr;
	udpHdr.source = htons(8080);
	udpHdr.dest = htons(53);
	udpHdr.len = data.payloadSize + sizeof(udpHdr);
	udpHdr.check = 0;
	checksum(&udpHdr, sizeof(udpHdr));

	char payload[] = "01234567";

	char *packet = (char *)malloc(sizeof(ipHdr) + sizeof(udpHdr) + data.payloadSize);
	ft_memcpy(packet, &ipHdr, sizeof(ipHdr));
	ft_memcpy(packet + sizeof(ipHdr), &udpHdr, sizeof(udpHdr));
	ft_memcpy(packet + sizeof(ipHdr) + sizeof(udpHdr), &payload, sizeof(payload));

	return packet;
}

void construct_packets() {
	for (int i = 0; i < 3; i++) {
		data.sendpack[i] = construct_packet(i);
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
			printf("bytes sent = %d\n", bytesSent);
			print_memory(data.sendpack[i], bytesSent, 16);
		}
	}
}
