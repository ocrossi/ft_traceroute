#include "../includes/ft_traceroute.h"
#include <stdlib.h>

t_data data;

void set_signals() {
  signal(SIGALRM, &print_interrupt);
  signal(SIGINT, &print_interrupt);
}

void init_data(void) {
  data.ttl = 1;
  create_socket();
}

void free_probes_data() {
  for (int i = 0; i < 3; i++) {
    if (data.retpack[i] != NULL) {
      free(data.retpack[i]);
      data.retpack[i] = NULL;
    }
    if (data.resIps[i] != NULL) {
      free(data.resIps[i]);
      data.resIps[i] = NULL;
    }
    if (data.resDns[i] != NULL) {
      free(data.resDns[i]);
      data.resDns[i] = NULL;
    }
  }
}

void free_sendpackets_data() {
  for (int i = 0; i < PROBENB; i++) {
    if (data.sendpack[i] != NULL) {
      // dprintf(1, "free sendpack[%d] in interrupt, addr = %p\n", i,
      //         data.sendpack[i]);
      // print_packet(data.sendpack[i], data.totalSize);
      free(data.sendpack[i]);
      data.sendpack[i] = NULL;
    }
    if (data.retpack[i] != NULL) {
      // dprintf(1, "free retpack[%d] in interrupt\n", i);
      free(data.retpack[i]);
      data.retpack[i] = NULL;
    }
  }
}

void send_recieve() {
  ft_bzero(data.recieved, sizeof(bool) * 3);
  // dprintf(1, "gimme current ttl %d\n", data.ttl);
  for (int i = 0; i < PROBENB; i++) {
    send_packet(i);
    // dprintf(1, "end send %d\n", i);
    data.recieved[i] = recieve_packet(i);
    // dprintf(1, "end recieve %d\n", i);
    print_probe_data(i);
  }
  ft_putchar('\n');
}

int main(int argc, char **argv) {
  // ft_bzero(&data, sizeof(t_data));
  manage_args(argc, argv);
  parse_dest(argc, argv);
  set_signals();
  init_data();
  print_head();
  data.alive = true;
  // data.ttl = 7; // tests
  while (data.ttl < MAX_HOPS && data.alive) {
    construct_packets();
    // send_packets();
    // recieve_packets();
    send_recieve();
    // print_probes_data2();
    free_probes_data();
    data.ttl++;
  }
  free_sendpackets_data();
  // send packets
  // parse return
  // display
  // incr ttl to max 30
  return EXIT_SUCCESS;
}
