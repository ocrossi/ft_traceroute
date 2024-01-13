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
	construct_packets();
	//while (data.ttl < 2) {
		data.ttl = 4;
		send_packets();
		dprintf(1, "wait\n");
		recieve_packets();
		print_probes_data();
		// free probs data
		data.ttl++;
		//change send packets data for ttl
	//}
	// send packets
	// parse return 
	// display
	// incr ttl to max 30
	return EXIT_SUCCESS;
}
