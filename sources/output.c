#include "../includes/ft_traceroute.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

void print_incorrect_args(char *arg) {
  dprintf(1, "Extra arg `%s' (position 3, argc 3)\n", arg);
}

void print_usage(void) {
  dprintf(1, "Usage:\n");
  dprintf(1, "\ttraceroute <destination> [optional_packet_size]\n\n");
  dprintf(1, "Options:\n");
  dprintf(1, "\t--help\t\t\tRead this help and exit\n\n");
  dprintf(1, "Arguments:\n");
  dprintf(1, "+\thost\t\t\tThe host to traceroute to\n");
  dprintf(1, "\tpacketlen\t\tThe full packet length (default is the length of "
             "an IP\n");
  dprintf(
      1, "\t\t\t\theader plus 40). Can be ignored or increased to a minimal\n");
  dprintf(1, "\t\t\t\tallowed value\n");
  exit(0);
}

void print_interrupt(int signum) {
  (void)signum;
  dprintf(1, "interrupt detected\n");
  exit(0);
}

void print_head(void) {
  dprintf(1, "traceroute to %s (%s): %d hops max, %d bytes packets\n",
          data.inputDest, data.destIp, MAX_HOPS, data.totalSize);
}

void display_address_packet(int index) {
  t_packetData *packet = data.retpack[index];

  struct sockaddr_in temp;
  socklen_t len;
  char buff[NI_MAXHOST];

  temp.sin_family = AF_INET;
  temp.sin_addr.s_addr = packet->ipHeader.saddr;
  len = sizeof(temp);

  if (!getnameinfo((struct sockaddr *)&temp, len, buff, sizeof(buff), NULL, 0,
                   NI_NAMEREQD)) {
    dprintf(1, "gimme address return %s\n", buff);
  } else {
    dprintf(1, "wesh %d\n", index);
  }

  inet_ntop(AF_INET, &packet->ipHeader.daddr, buff, INET_ADDRSTRLEN);

  dprintf(1, "buff = %s\n", buff);
  //
  // unsigned short source port = ntohs((unsigned short *)(packet+8));
  //
  // struct in_addr addr;
  // addr.s_addr = packet->udpHeader.source;
  // char *ip = inet_ntoa(addr);
  //
  // dprintf(1, "ip ret = %s\n", ip);
}

void print_probes_data(void) {
  for (int i = 0; i < 3; i++) {
    display_address_packet(i);
  }
}
