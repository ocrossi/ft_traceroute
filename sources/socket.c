#include "../includes/ft_traceroute.h"
#include <netinet/in.h>
#include <sys/socket.h>

void create_socket() {
  int sockFd;

  struct timeval tval;
  ft_memset(&tval, 0, sizeof(tval));
  tval.tv_sec = 1;
  tval.tv_usec = 0;

  sockFd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  // !! needs sudo privileges to create raw socket
  if (sockFd < 0) {
    perror("socket");
    exit(1);
  }

  // struct sockaddr_in dest;
  // dest.sin_family = AF_INET;
  // dest.sin_port = htons(8080);
  // dest.sin_addr.s_addr = INADDR_ANY;

  // if (bind(sockFd, (struct sockaddr *)&dest, sizeof(dest)) < 0) {
  // 	perror("bind");
  // 	exit(1);
  // }

  // protocol = IPPROTO_IP;
  // setsockopt(sockFd, protocol, IP_HDRINCL, (int[1]){1}, sizeof(int));
  // setsockopt(sockFd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tval,
  // sizeof(tval));
  int option = 1;
  if (setsockopt(sockFd, IPPROTO_IP, IP_HDRINCL, &option, sizeof(option))) {
    dprintf(1, "problem setsockopt\n");
    exit(0);
  }

  data.sockFd = sockFd;
}
