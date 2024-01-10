#include "../includes/ft_traceroute.h"

void print_incorrect_args(char* arg) {
	dprintf(1, "Extra arg `%s' (position 3, argc 3)\n", arg);
}

void print_usage(void) {
	dprintf(1, "Usage:\n");
	dprintf(1, "\ttraceroute <destination> [optional_packet_size]\n\n");
	dprintf(1, "Options:\n");
	dprintf(1, "\t--help\t\t\tRead this help and exit\n\n");
	dprintf(1, "Arguments:\n");
	dprintf(1, "+\thost\t\t\tThe host to traceroute to\n");
	dprintf(1, "\tpacketlen\t\tThe full packet length (default is the length of an IP\n");
	dprintf(1, "\t\t\t\theader plus 40). Can be ignored or increased to a minimal\n");
	dprintf(1, "\t\t\t\tallowed value\n");
	exit(0);
}

void print_interrupt(int signum) {
	(void)signum;
	dprintf(1, "interrupt detected\n");
	exit(0);
}
