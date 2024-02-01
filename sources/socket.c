#include "../includes/ft_traceroute.h"
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>

void create_socket() {
  int sockFd;

  struct timeval tval;
  ft_memset(&tval, 0, sizeof(tval));
  tval.tv_sec = 0;
  tval.tv_usec = 20000;

  sockFd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (sockFd < 0) {
    perror("socket");
    exit(1);
  }

  // on s occupe du header
  setsockopt(sockFd, IPPROTO_IP, IP_HDRINCL, (int[1]){1}, sizeof(int));
  // timeout 1 seconde
  setsockopt(sockFd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tval,
             sizeof(tval));

  data.sockFd = sockFd;
}
