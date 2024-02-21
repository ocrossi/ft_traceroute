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
    // dprintf(1, "cant recieve do u get timeout here?\n");
    data.retpack[index] = NULL;
    return false;
  }
  // else if (index == 0) {
  //   print_packet(recieve, bytesRecieved);
  // }
  data.retpack[index] = (t_packetData *)malloc(bytesRecieved);
  ft_memcpy(data.retpack[index], recieve, bytesRecieved);
  gettimeofday(&data.recieveTime[index], NULL);

  if (data.retpack[index]->icmpHeader.type == 0) {
    dprintf(1, "do we get a reply here?\n");
  }
  // dprintf(1, "ret pack type = %d code =  %d\n",
  //         data.retpack[index]->icmpHeader.type,
  //         data.retpack[index]->icmpHeader.code);
  //
  return true;
}

void recieve_packets() {
  ft_bzero(data.recieved, sizeof(bool) * 3);
  for (int i = 0; i < 3; i++) {
    data.recieved[i] = recieve_packet(i);
  }
  store_times();
  // print_times();
}
