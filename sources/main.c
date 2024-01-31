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
    free(data.retpack[i]);
    if (data.resIps[i] != NULL)
      free(data.resIps[i]);
  }
  if (data.resDns != NULL) {
    free(data.resDns);
    data.resDns = NULL;
  }
}

int main(int argc, char **argv) {
  manage_args(argc, argv);
  parse_dest(argc, argv);
  set_signals();
  init_data();
  print_head();
  while (data.ttl < MAX_HOPS) {
    construct_packets();
    send_packets();
    recieve_packets();
    print_probes_data();
    free_probes_data();
    data.ttl++;
  }
  // send packets
  // parse return
  // display
  // incr ttl to max 30
  return EXIT_SUCCESS;
}
