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

uint16_t checksum_udp(uint8_t *data, unsigned int size) {
  int i;
  int sum = 0;
  uint16_t *p = (uint16_t *)data;

  for (i = 0; i < size; i += 2) {
    sum += *(p++);
  }

  uint16_t carry = sum >> 16;
  uint16_t tmp = 0x0000ffff & sum;
  uint16_t res = ~(tmp + carry);

  return res;
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
  // header->protocol = IPPROTO_ICMP;
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

void construct_udp_header(void *packet, int port) {
  f_ipHdr ipHdrInfo;
  struct udphdr *header = packet + sizeof(struct iphdr);
  struct iphdr *headerip = packet;
  header->source = INADDR_ANY;
  header->dest = htons(port);                                    // port
  header->len = htons(data.payloadSize + sizeof(struct udphdr)); //

  ipHdrInfo.source_addr = headerip->saddr;
  ipHdrInfo.dest_addr = headerip->daddr;
  ipHdrInfo.zeros = 0;
  ipHdrInfo.protocol = IPPROTO_UDP;
  ipHdrInfo.length = header->len;

  int size_fake_packet =
      data.payloadSize + sizeof(struct udphdr) + sizeof(f_ipHdr);

  char *pseudo_packet = (char *)malloc(size_fake_packet);

  ft_memcpy(pseudo_packet, &ipHdrInfo, sizeof(f_ipHdr));
  ft_memcpy(pseudo_packet + sizeof(f_ipHdr), header,
            size_fake_packet - sizeof(f_ipHdr));

  // size_t size = data.totalSize - sizeof(struct udphdr);
  header->check = checksum_udp((uint8_t *)pseudo_packet, size_fake_packet);
  // header->check = 0;
}

void construct_udp_header2(struct udphdr *header, int port) {
  printf("WHU UDP WHYYY\n");
  header->source = INADDR_ANY;
  header->dest = htons(port);                                    // port
  header->len = htons(data.payloadSize + sizeof(struct udphdr)); //
  header->check = 0;
}

void *construct_packet(int index) {
  char *packet = (char *)malloc(data.totalSize);
  int offset = data.totalSize - data.payloadSize;

  ft_bzero(packet, data.totalSize);
  ft_memset(packet + offset, 'A', data.payloadSize);
  packet[data.totalSize - 1] = '\0';
  printf("payload [%s] for a size of %d\n", &packet[offset], data.payloadSize);

  construct_ip_header((struct iphdr *)packet);
  // ft_memcpy(packet, &ipHdr, sizeof(struct iphdr));
  if (data.icmp_route == true) {
    struct icmphdr *addrHdr = (struct icmphdr *)&packet[sizeof(struct iphdr)];
    construct_icmp_header(addrHdr);
  } else {
    struct udphdr *addrHdr = (struct udphdr *)&packet[sizeof(struct iphdr)];
    construct_udp_header2(addrHdr, UDP_PORT + index);
    // construct_udp_header(packet, UDP_PORT + index);
  }
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

void send_packet(int i) {
  // dprintf(1, "in send packet ttl du packet direct = %d\n",
  //         data.sendpack[i]->ipHeader.ttl);

  int bytesSent =
      sendto(data.sockFd, data.sendpack[i], data.totalSize, 0,
             (struct sockaddr *)data.networkIp, sizeof(struct sockaddr_in));
  if (bytesSent < 0) {
    dprintf(1, "bytes not sent for packet %d and ttl %d\n", i, data.ttl);
    perror("bytes not sent");
  } else {
    printf("%d sent\n", i);
  }
  gettimeofday(&data.sendTime[i], NULL);
  dprintf(1, "packet sent\n");
  print_packet((char *)data.sendpack[i], data.totalSize);
}

// void send_packets() {
//   dprintf(1, "gimme current ttl %d\n", data.ttl);
//   for (int i = 0; i < 3; i++) {
//     int bytesSent =
//         sendto(data.sockFd, data.sendpack[i], data.totalSize, 0,
//                (struct sockaddr *)data.networkIp, sizeof(struct
//                sockaddr_in));
//     if (bytesSent < 0) {
//       dprintf(1, "bytes not sent for packet %d and ttl %d\n", i, data.ttl);
//       perror("bytes not sent");
//     } else if (i == 0) {
//       // printf("bytes sent = %d\n", bytesSent);
//       // print_packet(data.sendpack[i], bytesSent);
//     }
//     gettimeofday(&data.sendTime[i], NULL);
//   }
// }
