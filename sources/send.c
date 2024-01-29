#include "../includes/ft_traceroute.h"
#include <netinet/in.h>
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

uint16_t checksum(const void *data, size_t len) {
  uint32_t sum = 0;
  for (size_t i = 0; i < len; ++i) {
    sum += *(uint16_t *)((char *)data + i);
  }
  while (sum >> 16) {
    sum = (sum & 0xffff) + (sum >> 16);
  }
  return ~sum;
}

void construct_ip_header(struct iphdr *header) {
  header->ihl = 5;
  header->version = 4;
  header->tos = 0;
  header->tot_len = htons(data.totalSize);
  header->id = getuid();
  header->frag_off = 0;
  header->ttl = data.ttl;
  header->protocol = IPPROTO_UDP;
  header->check = 0;
  header->saddr = INADDR_ANY;
  header->daddr = data.networkIp->sin_addr.s_addr;
  // checksum(&header-> sizeof(ipHdr));
}

void construct_udp_header(struct udphdr *header) {
  header->source = INADDR_ANY;
  header->dest = htons(60000);
  header->len = htons(sizeof(struct udphdr));
  header->check = 0;
}

void *construct_packet(int index) {
  struct iphdr ipHdr;
  struct udphdr udpHdr;

  construct_ip_header(&ipHdr);
  construct_udp_header(&udpHdr);

  // checksum(&udpHdr, sizeof(udpHdr));

  char payload[] = "01234567";

  char *packet =
      (char *)malloc(sizeof(ipHdr) + sizeof(udpHdr) + data.payloadSize);
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
    int bytesSent =
        sendto(data.sockFd, data.sendpack[i], data.totalSize, 0,
               (struct sockaddr *)data.networkIp, sizeof(struct sockaddr_in));
    if (bytesSent < 0) {
      dprintf(1, "bytes not sent for packet %d and ttl %d\n", i, data.ttl);
      perror("bytes not sent");
    } else {
      printf("bytes sent = %d\n", bytesSent);
      print_memory(data.sendpack[i], bytesSent, 16);
    }
  }
}
