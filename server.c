#include <utility_fuc.c>

fd_set active_fd_set, read_fd_set;
int i;
struct sockaddr_in clientname;
size_t size;

void brodcast(data packet)
{
	int itr = 3;
	int ret;
	printf("fd_set = %d\n", FD_SETSIZE);	
	while (itr < FD_SETSIZE) {
		printf("itr = %d\n", itr);	
		ret = send(itr, (void*)&packet, sizeof(packet), 0);
		if (ret == -1)
			error(1, errno, "Brodcast failed");
		itr++;
	}
	printf("done brodcast\n");
}

int read_from_client (int filedes)
{
	data recv_data;
	char buffer[MAXMSG];
	int nbytes;

	printf("In read function\n");
	nbytes = recv(filedes, (void*)&recv_data, sizeof(recv_data), 0);
	if (nbytes < 0)
		error(1, errno, "read");
	else if (!strcmp(recv_data.msg, "bye"))
		return -1;
	else
	{
		/* Data read. */
		printf("reading\n");
		printf("%s: %s\n", recv_data.usr, recv_data.msg);
		brodcast(recv_data);
		printf("done\n");
	}
	return 0;
}

int main()
{
	int pid, sock_fd, data_sock_fd, buffer_len;
	socklen_t c_size;
	struct sockaddr_in serv_addr;
	char serv_buff[SERVER_BUFF];
	char client_buff[SERVER_BUFF];

	log_cli_data usr_data;
	cli_data *ptr,*data_base = NULL;
	memset(&usr_data,0,sizeof(log_cli_data));

	printf("server is waiting...\n");

	/* create tcp socket */
	sock_fd = socket(AF_INET,SOCK_STREAM,0);

	/* Bind it to a particular IP address & port */
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv_addr.sin_port = htons(SERVER_PORT);

	/* bind ip address and port number */
	bind(sock_fd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

	/* wait for clint to request */
	listen(sock_fd,SERVER_LENGTH);
	/* Initialize the set of active sockets. */
	FD_ZERO (&active_fd_set);
	FD_SET (sock_fd, &active_fd_set);

	while (1)
	{
		read_fd_set = active_fd_set;
		/* Block until input arrives on one or more active sockets. */
		if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
			error(1, errno, "select failed");

		printf("FD_SETSIZE = %d\n", FD_SETSIZE);
		/* Service all the sockets with input pending. */
		for (i = 3; i < FD_SETSIZE; ++i)
		{
			if (FD_ISSET (i, &read_fd_set))
			{
				/* Connection request on original socket. */
				if (i == sock_fd)
				{
					int new;
					size = sizeof (clientname);
					new = accept(sock_fd, (struct sockaddr *)&clientname, &c_size);
					if (new < 0)
						error(1, errno, "accept");
					else
						perror ("accept");
					printf("Server: connect from host %s, port %d.\n", inet_ntoa (clientname.sin_addr),
							ntohs (clientname.sin_port)); 
					FD_SET (new, &active_fd_set);

					/* data from client */
					recv(new, (void *)&usr_data, sizeof(log_cli_data), 0);
					perror ("recive log_data");

					printf("stat = %d\n", usr_data.state);
					printf("user = %s\n", usr_data.usr_name);
					printf("pass = %d\n", usr_data.pass);
					// printf("fd = %d\n",usr_data.usr_fd);

					// data_base.usr_fd =  new;

					switch(usr_data.state)
					{
						case 0: insert_client_data(&data_base, usr_data.usr_name, usr_data.pass);
								break;
						case 1: if (find_node(&data_base, usr_data.usr_name))
								{
									send(new, (void*)"success", 8, 0);
									printf("data available\n");
								}
								else
								{
									send(new,(void*)"fail", 5, 0);
									printf("data not available\n");
								}
								break;
					}
				} else {
					/* Data arriving on an already-connected socket. */
					if (read_from_client(i) == -1)
					{
						printf("user exit\n");
						close (i);
						FD_CLR (i, &active_fd_set);
					}
				}
			}
		}
		printf("exit for loop\n");
	}
	
	/* close the socket now */
	close(data_sock_fd);
	close(sock_fd);
}
