#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include <netdb.h>

#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#include "../libraries/libft.h"
#define MAX_INT 2147483647
#define PACKET_SIZE 84
#define MAX_ARR_MEDIAN 10000
#define ICMP_PAYLOAD_SIZE 56
#define MAX_PACKET_SIZE 65000
#define ONE_SEC 1000000
#define MAX_HOPS 30

// errors //
#define EICMP_DEST_UNREACH		"Destination Unreachable\n"
#define EICMP_SOURCE_QUENCH		"Source Quench\n"
#define EICMP_REDIRECT			"Redirect (change route)\n"
#define EICMP_TIME_EXCEEDED		"Time to live exceeded\n"
#define EICMP_PARAMETERPROB		"Parameter Problem\n"
#define EICMP_TIMESTAMP			"Timestamp Request"
#define EICMP_TIMESTAMPREPLY	"Timestamp Reply\n"
#define EICMP_INFO_REQUEST		"Information Request\n"
#define EICMP_INFO_REPLY		"Information Reply\n"
#define EICMP_ADDRESS			"Address Mask Request\n"
#define EICMP_ADDRESSREPLY		"Address Mask Reply\n"

#define	UNKNOWN_ERR_CODE		"Unknown error code\n" 

typedef struct timeval		t_val;

typedef struct			s_packetData {
	struct iphdr		ipHeader;	// 20 len
	struct icmphdr		icmpHeader;	// 8 len
	char				*payload;
}				t_packetData;

typedef struct			s_data {
	int				ttl;
	int				size;
	int				sockFd;
	char			*test;
	char			*destIp;

	t_packetData		*sendpack[3];
	t_packetData		*retpack[3];
	struct sockaddr_in	*networkIp;
	t_val			sendTime[3];
	t_val			recieveTime[3];

}				t_data;

extern t_data data;

/* PARSING */
void manage_args(int argc, char **argv);
void parse_dest(int argc, char **argv);

/* OUTPUT */
void print_usage(void);
void print_incorrect_args(char *arg);
void print_interrupt(int signum);



// typedef struct 	s_pingData {
// 	char					options; // bits are 1 if acceptedFlag[bit_pos] est dans input
// 	char					*strIp;
// 	struct sockaddr_in		*networkIp;
// 	char					*reverseDns;
// 	t_packetData			*spacket;
// 	int						max_ping;
// 	int						ttl;
// 	useconds_t				interval;
// 	t_packetData			*rpacket;
// 	t_val					sendTime;
// 	t_val					recieveTime;
// 	t_val					start_time;
// 	double					time;
// 	char					*error;
// }							t_pingData;
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
// void			print_output_loop(t_pingData *data, bool recieved);
// void			print_usage(char invalidFlag);
// void			print_stats(int signum);
// void			print_flood_protection(void);
// void			reverseDNS(t_pingData *data);
// void			set_median_arr(void);
// double			find_average(void);
// double			find_stddev(void);
// double			convert_to_milliseconds(t_val time, t_val base);
// bool			check_packet_data(t_pingData *data);
// void			manage_time(t_pingData *data);
// char*			get_ip_reverseDNS(t_pingData *data);
// useconds_t		convert_to_microseconds(double seconds);

#endif
