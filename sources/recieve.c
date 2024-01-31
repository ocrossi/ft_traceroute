#include "../includes/ft_traceroute.h"
#include <stdio.h>
#include <unistd.h>

bool recieve_packet(int index) {
  char recieve[MAX_PACKET_SIZE];
  struct iovec retMsgData;
  struct msghdr retMsg;

  ft_memset(recieve, 0, MAX_PACKET_SIZE);
  ft_memset(&retMsgData, 0, sizeof(retMsgData));
  ft_memset(&retMsg, 0, sizeof(retMsg));

  retMsgData.iov_base = &recieve;
  retMsgData.iov_len = data.payloadSize;

  retMsg.msg_iov = &retMsgData;
  retMsg.msg_iovlen = 1;

  socklen_t size = sizeof(struct sockaddr);
  ssize_t bytesRecieved = recvfrom(data.sockFd, &recieve, MAX_PACKET_SIZE, 0,
                                   (struct sockaddr *)data.networkIp, &size);
  if (bytesRecieved < 0) {
    dprintf(1, "cant recieve do u get timeout here?\n");
    return false;
  }
  // else {
  //   printf("===========================================\n");
  //   printf("we've recieved %lu\n", bytesRecieved);
  //   // print_memory(recieve, bytesRecieved, 16);
  //   print_packet(recieve, bytesRecieved);
  //   printf("===========================================\n");
  // }
  data.retpack[index] = (t_packetData *)malloc(bytesRecieved);
  ft_memcpy(data.retpack[index], recieve, bytesRecieved);
  gettimeofday(&data.recieveTime[index], NULL);

  return true;
}

void recieve_packets() {
  for (int i = 0; i < 3; i++) {
    recieve_packet(i);
  }
  store_times();
  // print_times();
}
