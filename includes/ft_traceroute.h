#ifndef FT_TRACEROUTE_H
#define FT_TRACEROUTE_H

#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <netdb.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>

#include "../libraries/libft.h"
#define MAX_INT 2147483647
#define PACKET_SIZE 84
#define MAX_ARR_MEDIAN 10000
#define ICMP_PAYLOAD_SIZE 56
#define MAX_PACKET_SIZE 65000
#define ONE_SEC 1000000
#define MAX_HOPS 30
#define UDP_PORT_D 33434
#define UDP_PORT_S 33434

/* // errors // */
/* #define EICMP_DEST_UNREACH "Destination Unreachable\n" */
/* #define EICMP_SOURCE_QUENCH "Source Quench\n" */
/* #define EICMP_REDIRECT "Redirect (change route)\n" */
/* #define EICMP_TIME_EXCEEDED "Time to live exceeded\n" */
/* #define EICMP_PARAMETERPROB "Parameter Problem\n" */
/* #define EICMP_TIMESTAMP "Timestamp Request" */
/* #define EICMP_TIMESTAMPREPLY "Timestamp Reply\n" */
/* #define EICMP_INFO_REQUEST "Information Request\n" */
/* #define EICMP_INFO_REPLY "Information Reply\n" */
/* #define EICMP_ADDRESS "Address Mask Request\n" */
/* #define EICMP_ADDRESSREPLY "Address Mask Reply\n" */
/**/
#define UNKNOWN_ERR_CODE "Unknown error code\n"

#define PROBENB 3

typedef struct timeval t_val;
typedef struct udphdr t_udp;
typedef struct iphdr t_ip;

typedef struct pseudo_ipHdr {
  uint32_t source_addr;
  uint32_t dest_addr;
  uint8_t zeros;
  uint8_t protocol;
  uint16_t length;

} f_ipHdr;

typedef struct s_packetData {
  struct iphdr ipHeader; // 20 len
  union {
    struct icmphdr icmpHeader;
    struct udphdr udpHdr;
  };
} t_packetData;

typedef struct s_data {
  int ttl;
  int payloadSize;
  int totalSize;
  int sockFd;
  char *destIp;
  char *inputDest;
  struct sockaddr_in *networkIp;

  t_packetData *sendpack[PROBENB];
  t_packetData *retpack[PROBENB];
  t_val sendTime[PROBENB];
  t_val recieveTime[PROBENB];
  bool recieved[PROBENB];
  char *resIps[PROBENB];
  char *resDns[PROBENB];
  double probeTimes[PROBENB];

  bool icmp_route;
  int icmp_route_index;
  bool alive;
} t_data;

extern t_data data;

/* PARSING */
void manage_args(int argc, char **argv);
void parse_dest(int argc, char **argv);

/* SOCKET */
void create_socket(void);

/* CONSTRUCT & SEND */
void construct_packets(void);
void update_packets(void);
void send_packets(void);
void send_packet(int index);

/* RECIEVE */
void recieve_packets(void);
bool recieve_packet(int index);

/* UTILS */
double convert_to_milliseconds(t_val time, t_val base);
void free_probes_data();
void free_sendpackets_data();

/* OUTPUT */
void print_times(void);
void print_usage(void);
void print_incorrect_args(char *arg);
void print_interrupt(int signum);
void print_head();
/* void print_probes_data(void); */
// void print_probes_data2(void);
void print_probe_data(int index);
void print_packet(char *string, int len);

// typedef struct 	s_pingData {
// 	char					options; // bits are 1 if
// acceptedFlag[bit_pos] est dans input 	char
// *strIp; 	struct sockaddr_in		*networkIp; 	char
// *reverseDns; 	t_packetData			*spacket; 	int
// max_ping; 	int						ttl;
// useconds_t interval; 	t_packetData			*rpacket;
// t_val sendTime; 	t_val					recieveTime;
// t_val start_time; 	double					time; 	char
// *error; }							t_pingData;
//
//
// typedef struct 	s_statData {
// 	t_pingData				*data;
// 	char					*nameDestination;
// 	int						transmitted;
// 	int						recieved;
// 	int						lost;
// 	int						pingNb;
// 	double					time;
// 	double					min;
// 	double					average;
// 	double					median;
// 	double					max;
// 	double					mdev;
// 	int						nbErrs;
// 	double					*median_arr;
// }							t_statData;
//
//
//
// extern char						acceptedFlags[];
// extern t_statData				stats;
//
// void			parsing(int ac, char **args, t_pingData *data);
// int				create_socket(t_pingData *data);
// void			construct_packet(t_pingData *data);
// void			send_packet(t_pingData *data, int sockFd);
// bool			recieve_packet(t_pingData *data, int sockFd);
// void			print_output_loop(t_pingData *data, bool
// recieved); void			print_usage(char invalidFlag); void
// print_stats(int signum); void
// print_flood_protection(void); void			reverseDNS(t_pingData
// *data); void			set_median_arr(void); double
// find_average(void); double			find_stddev(void); double
// convert_to_milliseconds(t_val time, t_val base); bool
// check_packet_data(t_pingData *data); void
// manage_time(t_pingData *data); char*
// get_ip_reverseDNS(t_pingData *data); useconds_t
// convert_to_microseconds(double seconds);

#endif
