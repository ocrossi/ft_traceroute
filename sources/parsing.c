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

  // printf("test dest %s\n", arg);

  if ((ret = getaddrinfo(arg, NULL, &hints, &res)) != 0) {
    fprintf(stderr, "%s: %s\n", arg, gai_strerror(ret));
    fprintf(stderr,
            "Cannot handle \"host\" cmdline arg %s on position 1 (argc 1)\n",
            arg);
    exit(0);
  }
  h = (struct sockaddr_in *)res->ai_addr;
  inet_ntop(AF_INET, &h->sin_addr, buff, INET_ADDRSTRLEN);
  // printf("buff %s\n", buff);
  data.inputDest = ft_strdup(arg);
  data.destIp = ft_strdup(buff);
  data.networkIp = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
  data.totalSize = 60;
  data.payloadSize = data.totalSize - 28;
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

void test_packetlen(char *plen, int index) {
  int len = ft_atoi(plen);

  if (isNum(plen) == -1 || len < 0) {
    fprintf(
        stderr,
        "Cannot handle \"packetlen\" cmdline arg %s on position 2 (argc %d)\n",
        plen, index);
    exit(0);
  }
  if (len > MAX_PACKET_SIZE) {
    fprintf(stderr, "too big packetlen %d specified\n", len);
    exit(0);
  }
  data.totalSize = len < 28 ? 28 : len;
  data.payloadSize = data.totalSize - 28;
}

void check_help_flag(int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    if (ft_strcmp(argv[i], "--help") == 0) {
      print_usage();
    }
  }
}

int check_icmp_traceroute(int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    if (ft_strcmp(argv[i], "-I") == 0 || ft_strcmp(argv[i], "--icmp") == 0) {
      data.icmp_route = true;
      data.icmp_route_index = i;
      return 1;
    }
  }
  return 0;
}

void manage_args(int argc, char **argv) {
  // printf("beginning %d, argc = %d\n", data.icmp_route, argc);
  check_help_flag(argc, argv);
  argc -= check_icmp_traceroute(argc, argv);
  // printf("beginning1 %d, argc = %d\n", data.icmp_route, argc);
  if (argc < 2) {
    print_usage();
  }
  if (argc >= 4) {
    print_incorrect_args(argv[3]);
  }
  // printf("end %d, argc = %d\n", data.icmp_route, argc);
}

void parse_dest(int argc, char **argv) {
  if (argc == 2) {
    if (ft_strcmp(argv[1], "--help") == 0) {
      print_usage();
    }
    test_dest(argv[1]);
  } else if (argc == 3) {
    if (check_icmp_traceroute(argc, argv) == 1) {
      int index_dest = (data.icmp_route_index == 1) ? 2 : 1;
      test_dest(argv[index_dest]);
    } else {
      test_dest(argv[1]);
      test_packetlen(argv[2], 2);
    }
  } else if (argc == 4) {
    if (check_icmp_traceroute(argc, argv) == 0) {
      print_usage();
      exit(0);
    }
    int index_dest = (data.icmp_route_index >= 2) ? 1 : 2;
    int index_packetlen = (data.icmp_route_index <= 2) ? 3 : 2;
    test_dest(argv[index_dest]);
    test_packetlen(argv[index_packetlen], index_packetlen);
  } else {
    print_usage();
    exit(0);
  }
}
