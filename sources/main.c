#include "../includes/ft_traceroute.h"

t_data data;

void set_signals() {
	signal(SIGALRM, &print_interrupt);
	signal(SIGINT, &print_interrupt);
}

void init_data(void) {
	data.ttl = 1;
	create_socket();
}

int main(int argc, char **argv)
{
	manage_args(argc, argv);
	parse_dest(argc, argv);
	set_signals();
	init_data();
  print_head();
	while (data.ttl < 2) {
	  construct_packets();
		send_packets();
		recieve_packets();
    print_probes_data();
    // free probs data
		data.ttl++;
	}
	// send packets
	// parse return 
	// display
	// incr ttl to max 30
	return EXIT_SUCCESS;
}
