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

bool check_one_route() {
  for (int i = 0; i < PROBENB; i++) {
    if (data.recieved[i] == false)
      return false;
  }
  int truth = data.retpack[0]->ipHeader.saddr;
  for (int i = 1; i < PROBENB; i++) {
    if (data.retpack[i]->ipHeader.saddr != truth) {
      return false;
    }
  }
  return true;
}

void get_address(int index) {
  t_packetData *packet = data.retpack[index];

  struct sockaddr_in temp;
  socklen_t len;
  char buff[NI_MAXHOST];

  temp.sin_family = AF_INET;
  temp.sin_addr.s_addr = packet->ipHeader.saddr;
  len = sizeof(temp);

  if (!getnameinfo((struct sockaddr *)&temp, len, buff, sizeof(buff), NULL, 0,
                   NI_NAMEREQD)) {
    data.resDns[index] = ft_strdup(buff);

    // dprintf(1, "res ip = %s\n", buff);
  }
  inet_ntop(AF_INET, &packet->ipHeader.saddr, buff, INET_ADDRSTRLEN);
  data.resIps[index] = ft_strdup(buff);
  // dprintf(1, "res ip = %s\n", buff);
}

void print_single_route() {
  get_address(0);
  char *target = (data.resDns[0] == NULL) ? data.resIps[0] : data.resDns[0];
  dprintf(1, "%s (%s) %.3lf ms %.3lf ms %.3lf ms\n", target, data.resIps[0],
          data.probeTimes[0], data.probeTimes[1], data.probeTimes[2]);
}

void print_route(int index) {
  get_address(index);
  char *target =
      (data.resDns[index] == NULL) ? data.resIps[index] : data.resDns[index];
  dprintf(1, "%s (%s) %.3lf ms ", target, data.resIps[index],
          data.probeTimes[index]);
}

// void check_dest_reached() {
//   for (int i = 0; i < PROBENB; i++) {
//     if (data.recieved[i] == false)
//       continue;
//     if (data.networkIp->sin_addr.s_addr == data.retpack[i]->ipHeader.saddr) {
//       data.alive = false;
//       return;
//     }
//   }
// }

void print_probes_data2() {
  dprintf(1, "%d ", data.ttl);
  // check_dest_reached();
  if (check_one_route()) {
    print_single_route();
    return;
  }
  for (int i = 0; i < PROBENB; i++) {
    if (data.recieved[i] == false) {
      dprintf(1, "* ");
      continue;
    }
    print_route(i);
  }
  ft_putchar('\n');
}

void print_times() {
  for (int i = 0; i < PROBENB; i++) {
    dprintf(1, "time = %lf\n", data.probeTimes[i]);
  }
}
