#include "../includes/ft_traceroute.h"

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

	ssize_t bytesRecieved = recvmsg(data.sockFd, &retMsg, 0);
	if (bytesRecieved < 0) {
		return false;
	} else {
		// printf("===========================================\n");
		// printf("we've recieved %lu\n", bytesRecieved);
		// printf("wait data total size = %d\n", data.totalSize);
		//   print_memory(recieve, bytesRecieved, 16);
		// printf("===========================================\n");
	}
	data.retpack[index] = (t_packetData *)malloc(bytesRecieved);
	ft_memcpy(data.retpack[index], recieve, bytesRecieved);
	// data->recievedBytesArray = (char *)malloc(	bytesRecieved);
	// ft_memcpy(data->recievedBytesArray, recieve, bytesRecieved);
	gettimeofday(&data.recieveTime[index], NULL);
	// data->retPrintSize = bytesRecieved;
  printf("can we check payload from here ? %s\n", data.retpack[index]->payload);
	return true;
}

void recieve_packets() {
	for (int i = 0; i < 3; i++) {
		recieve_packet(i);
	}
}
