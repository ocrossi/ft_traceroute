#include "../includes/ft_traceroute.h"

void test_dest(char *arg) {
	char buff[INET_ADDRSTRLEN];
	int ret;
	struct addrinfo hints;
	struct addrinfo *res, *p;
	struct sockaddr_in *h;
	ft_memset(&hints, 0, sizeof(hints));
	ft_memset(&buff, 0, sizeof(buff));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_flags = 0;

	if ((ret = getaddrinfo(arg, NULL, &hints, &res)) != 0) {
		fprintf(stderr, "%s: %s\n", arg, gai_strerror(ret));
		fprintf(stderr, "Cannot handle \"host\" cmdline arg %s on position 1 (argc 1)\n", arg);
		exit(0);
	}
	h = (struct sockaddr_in *)res->ai_addr;
	inet_ntop(AF_INET, &h->sin_addr, buff, INET_ADDRSTRLEN);
	// printf("buff %s\n", buff);
	data.destIp = ft_strdup(buff);
	data.networkIp  = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
	ft_memcpy(data.networkIp, h, sizeof(struct sockaddr_in));
	freeaddrinfo(res);
}

int isNum(char *arg) {
	for (int i = 0; i < ft_strlen(arg); i++) {
		if (arg[i] < '0' || arg[i] > '9')
			return -1;
	}
	return 1;
}

void test_packetlen(char *plen) {
	if (isNum(plen) == -1 || ft_atoi(plen) < 0) {
		fprintf(stderr, "Cannot handle \"packetlen\" cmdline arg %s on position 2 (argc 2)\n", plen);
		exit(0);
	}
	if (ft_atoi(plen) > MAX_PACKET_SIZE) {
		fprintf(stderr, "too big packetlen %d specified\n", ft_atoi(plen));
		exit(0);
	}
	data.size = ft_atoi(plen);
}

void parse_dest(int argc, char **argv) {
	test_dest(argv[1]);
	if (argc > 2) {
		test_packetlen(argv[2]);
	} else {
		data.size = 60;
	} 
}

void check_help_flag(int argc, char **argv) {
	for (int i = 1; i < argc; i++) {
		if (ft_strcmp(argv[i], "--help") == 0) {
			print_usage();
		}
	}

}

void manage_args(int argc, char **argv) {
	check_help_flag(argc, argv);

	if (argc < 2) {
		print_usage();
	}
	if (argc >= 4) {
		print_incorrect_args(argv[3]);
	}
}
