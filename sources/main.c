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

int main(int argc, char **argv) {
  manage_args(argc, argv);
  parse_dest(argc, argv);
  set_signals();
  init_data();
  print_head();
  data.alive = true;
  while (data.ttl < MAX_HOPS && data.alive) {
    construct_packets();
    send_packets();
    recieve_packets();
    print_probes_data2();
    free_probes_data();
    data.ttl++;
  }
  // send packets
  // parse return
  // display
  // incr ttl to max 30
  return EXIT_SUCCESS;
}
