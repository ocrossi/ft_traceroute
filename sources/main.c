#include "../includes/ft_traceroute.h"
#include <stdlib.h>

t_data data;

void kill_program() { data.alive = false; }

void set_signals() {
  signal(SIGALRM, &kill_program);
  signal(SIGINT, &kill_program);
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
      free(data.sendpack[i]);
      data.sendpack[i] = NULL;
    }
    if (data.retpack[i] != NULL) {
      free(data.retpack[i]);
      data.retpack[i] = NULL;
    }
  }
  free(data.networkIp);
}

void send_recieve() {
  ft_bzero(data.recieved, sizeof(bool) * 3);

  dprintf(1, "%d ", data.ttl);
  for (int i = 0; i < PROBENB; i++) {
    send_packet(i);
    data.recieved[i] = recieve_packet(i);
  }
}

void print_probes_output() {
  for (int i = 0; i < PROBENB; i++) {
    print_probe_data(i);
  }
}

int main(int argc, char **argv) {
  parse_dest(argc, argv);
  set_signals();
  init_data();
  print_head();
  data.alive = true;
  construct_packets();
  while (data.ttl <= MAX_HOPS && data.alive) {
    if (data.ttl != 1)
      update_packets();
    send_recieve();
    print_probes_output();
    free_probes_data();
    data.ttl++;
  }
  free_probes_data();
  free_sendpackets_data();
  return EXIT_SUCCESS;
}
