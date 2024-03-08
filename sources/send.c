#include "../includes/ft_traceroute.h"
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

unsigned short checksum(void *b, int len) {
  unsigned short *buf = b;
  unsigned int sum = 0;
  unsigned short result;

  for (sum = 0; len > 1; len -= 2)
    sum += *buf++;
  if (len == 1)
    sum += *(unsigned char *)buf;
  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);
  result = ~sum;
  return result;
}

void construct_ip_header(struct iphdr *header) {
  header->ihl = 5;
  header->version = 4;
  header->tos = 0;
  header->tot_len = 0;
  header->id = 0;
  header->frag_off = 0;
  header->ttl = data.ttl;
  header->protocol = data.icmp_route == true ? IPPROTO_ICMP : IPPROTO_UDP;
  header->check = 0;
  header->saddr = INADDR_ANY;
  header->daddr = data.networkIp->sin_addr.s_addr;
}

void construct_icmp_header(struct icmphdr *header) {
  header->type = ICMP_ECHO;
  header->code = 0;
  header->un.echo.id = 0;
  header->un.echo.sequence = data.ttl;

  size_t size = data.payloadSize + sizeof(struct icmphdr);
  header->checksum = checksum(header, size);
}

void construct_udp_header(struct udphdr *header, int port_s, int port_d) {
  // printf("WHY UDP WHYYY, port %d\n", port);
  header->source = htons(port_s);
  header->dest = htons(port_d);                                  // port
  header->len = htons(data.payloadSize + sizeof(struct udphdr)); //
  header->check = 0;
}

void *construct_packet(int index) {
  char *packet = (char *)malloc(data.totalSize);
  int offset = data.totalSize - data.payloadSize;

  ft_bzero(packet, data.totalSize);
  ft_memset(packet + offset, 'A', data.payloadSize);
  packet[data.totalSize - 1] = '\0';
  // printf("payload [%s] for a size of %d\n", &packet[offset],
  // data.payloadSize);

  construct_ip_header((struct iphdr *)packet);
  // ft_memcpy(packet, &ipHdr, sizeof(struct iphdr));
  if (data.icmp_route == true) {
    struct icmphdr *addrHdr = (struct icmphdr *)&packet[sizeof(struct iphdr)];
    construct_icmp_header(addrHdr);
  } else {
    struct udphdr *addrHdr = (struct udphdr *)&packet[sizeof(struct iphdr)];
    construct_udp_header(addrHdr, UDP_PORT_S + index, UDP_PORT_D + index);
  }
  return packet;
}

void construct_packets() {
  for (int i = 0; i < 3; i++) {
    data.sendpack[i] = construct_packet(i);
  }
}

void update_packets() {
  for (int i = 0; i < 3; i++) {
    data.sendpack[i]->ipHeader.ttl = data.ttl;
    if (data.icmp_route == false) {
      int new_port_d = UDP_PORT_D + i + (data.ttl - 1) * 3;
      int new_port_s = UDP_PORT_S + i + (data.ttl - 1) * 3;
      data.sendpack[i]->udpHdr.source = htons(new_port_s);
      data.sendpack[i]->udpHdr.dest = htons(new_port_d);
    }
  }
}

void send_packet(int i) {
  // dprintf(1, "in send packet ttl du packet direct = %d\n",
  //         data.sendpack[i]->ipHeader.ttl);

  int bytesSent =
      sendto(data.sockFd, data.sendpack[i], data.totalSize, 0,
             (struct sockaddr *)data.networkIp, sizeof(struct sockaddr_in));
  if (bytesSent < 0) {
    // dprintf(1, "bytes not sent for packet %d and ttl %d\n", i, data.ttl);
    perror("bytes not sent");
  } else {
    // printf("%d sent\n", i);
  }
  gettimeofday(&data.sendTime[i], NULL);
  // dprintf(1, "packet sent\n");
  // print_packet((char *)data.sendpack[i], data.totalSize);
}
