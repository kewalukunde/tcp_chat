#define NOT_FOUND -1
#define FOUND 1
#define MAXMSG 100

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <error.h>
#include <errno.h>

#pragma pack(1)

#define SERVER_IP     "127.0.0.1"
#define SERVER_PORT   6000
#define SERVER_LENGTH 50
#define SERVER_BUFF   50
#define CLIENT_BUFF   50
 
typedef struct log
{
		  short int state;
		  char usr_name[15];
		  int pass;
}log_cli_data;

typedef struct node
{
		  char usr[15];
		  int pass;
		  int usr_fd;
		  struct node *next;
}cli_data;

typedef struct node_data
{
		  int state;
		  char usr[15];
		  char msg[50];
}data;


