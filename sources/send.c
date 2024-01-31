#include "../includes/ft_traceroute.h"
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <unistd.h>

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
  header->tot_len = data.totalSize;
  header->id = getuid();
  header->frag_off = 0;
  header->ttl = data.ttl;
  header->protocol = IPPROTO_ICMP;
  header->check = 0;
  header->saddr = INADDR_ANY;
  header->daddr = data.networkIp->sin_addr.s_addr;
  // checksum(&header-> sizeof(ipHdr));
}

void construct_icmp_header(struct icmphdr *header) {
  header->type = ICMP_ECHO;
  header->code = 0;
  header->un.echo.id = getpid();
  header->un.echo.sequence = data.ttl;
  header->checksum = checksum(header, data.totalSize);
}

void *construct_packet(int index) {
  struct iphdr ipHdr;
  struct icmphdr icmpHdr;

  construct_ip_header(&ipHdr);
  construct_icmp_header(&icmpHdr);

  char payload[] = "01234567";
  char *packet =
      (char *)malloc(sizeof(ipHdr) + sizeof(icmpHdr) + data.payloadSize);
  ft_memcpy(packet, &ipHdr, sizeof(ipHdr));
  ft_memcpy(packet + sizeof(ipHdr), &icmpHdr, sizeof(icmpHdr));
  ft_memcpy(packet + sizeof(ipHdr) + sizeof(icmpHdr), &payload,
            sizeof(payload));

  return packet;
}

void construct_packets() {
  for (int i = 0; i < 3; i++) {
    if (data.ttl != 1) {
      data.sendpack[i]->ipHeader.ttl = data.ttl;
    } else {
      data.sendpack[i] = construct_packet(i);
    }
  }
}

void send_packets() {
  for (int i = 0; i < 3; i++) {
    int bytesSent =
        sendto(data.sockFd, data.sendpack[i], data.totalSize, 0,
               (struct sockaddr *)data.networkIp, sizeof(struct sockaddr_in));
    if (bytesSent < 0) {
      dprintf(1, "bytes not sent for packet %d and ttl %d\n", i, data.ttl);
      perror("bytes not sent");
    } // else {
    //   printf("bytes sent = %d\n", bytesSent);
    //   // print_packet(data.sendpack[i], bytesSent);
    // }
    gettimeofday(&data.sendTime[i], NULL);
  }
}
