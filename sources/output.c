#include "../includes/ft_traceroute.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>

void print_incorrect_args(char *arg) {
  dprintf(1, "Extra arg `%s' (position 3, argc 3)\n", arg);
}

void print_packet(char *string, int len) {
  for (int i = 0; i < len; i++) {
    if (i % 8 == 0 && i % 16 != 0) {
      dprintf(1, "][");
    }
    if (i % 16 == 0) {
      dprintf(1, "\n");
    }
    dprintf(1, "%02hhx ", string[i]);
  }
  dprintf(1, "\n");
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

void store_address_packet(int index) {
  t_packetData *packet = data.retpack[index];

  struct sockaddr_in temp;
  socklen_t len;
  char buff[NI_MAXHOST];

  temp.sin_family = AF_INET;
  temp.sin_addr.s_addr = packet->ipHeader.saddr;
  len = sizeof(temp);

  if (index > 0 &&
      packet->ipHeader.saddr == data.retpack[index - 1]->ipHeader.saddr) {
    data.resIps[index] = NULL;
    return;
  } else if (index != 0)
    data.divs++;

  if (index == 0 && data.divs == 0 &&
      !getnameinfo((struct sockaddr *)&temp, len, buff, sizeof(buff), NULL, 0,
                   NI_NAMEREQD)) {
    data.resDns = ft_strdup(buff);
    // dprintf(1, "allloooooo %s\n", buff);
  }
  inet_ntop(AF_INET, &packet->ipHeader.saddr, buff, INET_ADDRSTRLEN);
  data.resIps[index] = ft_strdup(buff);
}

void construct_output_string() {
  char *res;
  char *tmp1;
  char *tmp2;

  for (int i = 0; i < PROBENB; i++) {
    if (data.divs == 0) {
      char *target = (data.resDns == NULL) ? data.resIps[0] : data.resDns;
      dprintf(1, "%d %s (%s) %.3lf ms %.3lf ms %.3lf ms\n", data.ttl, target,
              data.resIps[0], data.probeTimes[0], data.probeTimes[1],
              data.probeTimes[2]);
      return;
    }
    if (data.resIps[i] == NULL)
      continue;
    if (i == 0) {
      dprintf(1, "%d %s (%s) %.3lf ms ", data.ttl, data.resIps[i],
              data.resIps[i], data.probeTimes[i]);
    } else {
      dprintf(1, "%d %s (%s)\t%.3lf ms ", data.ttl, data.resIps[i],
              data.resIps[i], data.probeTimes[i]);
    }
  }
  ft_putchar('\n');
}

void print_probes_data(void) {
  for (int i = 0; i < PROBENB; i++) {
    store_address_packet(i);
  }
  construct_output_string();
  data.divs = 0;
}

void print_times() {
  for (int i = 0; i < PROBENB; i++) {
    dprintf(1, "time = %lf\n", data.probeTimes[i]);
  }
}
