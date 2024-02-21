#include "../includes/ft_traceroute.h"
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <unistd.h>

// uint16_t checksum(const void *data, size_t len) {
//   uint32_t sum = 0;
//   for (size_t i = 0; i < len; ++i) {
//     sum += *(uint16_t *)((char *)data + i);
//   }
//   while (sum >> 16) {
//     sum = (sum & 0xffff) + (sum >> 16);
//   }
//   return ~sum;
// }

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
  // checksum(&header-> sizeof(ipHdr));
}

void construct_icmp_header(struct icmphdr *header) {
  header->type = ICMP_ECHO;
  header->code = 0;
  header->checksum = 0;
  header->un.echo.id = 0;
  header->un.echo.sequence = data.ttl;
  // header->checksum = checksum(header, data.totalSize - data.payloadSize);
}

void *construct_packet(int index) {
  // struct iphdr ipHdr;
  // struct icmphdr icmpHdr;

  struct iphdr ipHdr;
  struct icmphdr icmpHdr;

  // ft_bzero(&ipHdr, sizeof(struct iphdr));
  // ft_bzero(&icmpHdr, sizeof(struct iphdr));

  // ipHdrPtr = &ipHdr;
  // icmpHdrPtr = &icmpHdr;

  construct_ip_header(&ipHdr);
  construct_icmp_header(&icmpHdr);

  // char payload[] = "01234567";
  char *packet = (char *)malloc(data.totalSize);
  ft_memcpy(packet, &ipHdr, sizeof(struct iphdr));
  ft_memcpy(packet + sizeof(struct iphdr), &icmpHdr, sizeof(struct icmphdr));
  // ft_memcpy(packet + sizeof(ipHdr) + sizeof(icmpHdr), &payload,
  //           sizeof(payload));

  return packet;
}

void construct_packets() {
  // dprintf(1, "in construct packets gimme ttl %d\n", data.ttl);
  for (int i = 0; i < 3; i++) {
    if (data.ttl != 1 /* to remove && data.ttl != 7*/) {
      data.sendpack[i]->ipHeader.ttl = data.ttl;
    } else {
      // dprintf(1, "wesh les uni %p\n", data.sendpack[i]);
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
  } else if (i == 0) {
    // printf("bytes sent = %d\n", bytesSent);
    // print_packet(data.sendpack[i], bytesSent);
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
