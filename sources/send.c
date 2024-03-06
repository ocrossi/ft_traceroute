#include "../includes/ft_traceroute.h"
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <stdio.h>
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
  header->tot_len = 0;
  header->id = getpid();
  header->frag_off = 0;
  header->ttl = data.ttl;
  header->protocol = IPPROTO_ICMP;
  header->check = 0;
  header->saddr = INADDR_ANY;
  header->daddr = data.networkIp->sin_addr.s_addr;
}

void construct_icmp_header(struct icmphdr *header) {
  printf("icmp header\n");
  header->type = ICMP_ECHO;
  header->code = 0;
  header->un.echo.id = 0;
  header->un.echo.sequence = data.ttl;
  header->checksum = 0;
  // header->checksum = checksum(data. , );
}

void construct_udp_header(struct udphdr *header, int port) {
  printf("udp header\n");
  header->source = INADDR_ANY;
  header->dest = htons(port);                                    // port
  header->len = htons(data.payloadSize + sizeof(struct udphdr)); //
  header->check = 0;
}

void check(struct udphdr *header, int size) {
  header->check = checksum(header, (size_t)size);
}

void *construct_packet(int index) {
  char *packet = (char *)malloc(data.totalSize);
  struct iphdr ipHdr;

  construct_ip_header(&ipHdr);
  ft_memcpy(packet, &ipHdr, sizeof(struct iphdr));
  if (data.icmp_route == true) {
    struct icmphdr icmpHdr;
    construct_icmp_header(&icmpHdr);
    ft_memcpy(packet + sizeof(struct iphdr), &icmpHdr, sizeof(struct icmphdr));
  } else {
    struct udphdr udpHdr;
    construct_udp_header(&udpHdr, UDP_PORT + index);
    ft_memcpy(packet + sizeof(struct iphdr), &udpHdr, sizeof(struct udphdr));
  }
  char s[data.payloadSize + 1];
  s[data.payloadSize] = '\0';
  memset(s, 'A', data.payloadSize);
  printf("payload [%s] for a size of %d\n", s, data.payloadSize);
  ft_memcpy(packet + sizeof(t_packetData), s, data.payloadSize);

  if (data.icmp_route == true) {
    check((struct udphdr *)(packet + sizeof(struct iphdr)),
          data.totalSize - sizeof(struct iphdr));
  }
  print_packet(packet, data.totalSize);

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
  }
  gettimeofday(&data.sendTime[i], NULL);
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
