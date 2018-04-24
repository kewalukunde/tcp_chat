#include <tcp_chat.h>

int main()
{
	int pid, sock_fd, data_sock_fd, c_size, choice;
	struct sockaddr_in serv_addr;
	char client_buff[CLIENT_BUFF];
	char serv_buff[CLIENT_BUFF];
	log_cli_data usr_data;
	data send_data;
	data recv_data;
	int ret, len;
	fd_set readfds;
	int i;
	struct sockaddr_in clientname;
	size_t size;

	memset(&usr_data, 0, sizeof(log_cli_data));

	/* create tcp socket */
	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Error : could not create socket \n");
		return 0;
	}

	/* Bind it to a particular IP address & port */
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv_addr.sin_port = htons(SERVER_PORT);

	if ((connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) != 0)
		printf("Error conecting to server...\n");
	else 
		printf("Connected to  server...\n");

	printf("1. NEW USER-Sign up\t2. sign in\n\n");
	scanf("%d",&choice);

	if(choice == 1) {
		usr_data.state = 0;
		printf("Enter User name : ");    
		scanf("%s", usr_data.usr_name);
		printf("Enter passward  : ");    
		scanf("%d", &usr_data.pass);

		printf("sizeof struct =%lu\n", sizeof(usr_data));
		printf("usr_data.pass = %d\n", usr_data.pass);
		printf("usr_data.usr_name = %s\n", usr_data.usr_name);

		send(sock_fd, (void*)&usr_data, sizeof(usr_data), 0);
		perror("send log :");
	} else if(choice == 2) {
		while (1) {
			usr_data.state = 1;
			printf("Enter User name : ");    
			scanf("%s", usr_data.usr_name);
			printf("Enter passward  : ");    
			scanf("%d", &usr_data.pass);

			send(sock_fd, (void*)&usr_data, sizeof(usr_data), 0);
			perror("send log :");
			recv(sock_fd, (void*)client_buff, sizeof(client_buff), 0);
			if (!strcmp(client_buff, "success"))
			{
				perror("verified\n :");
				break;
			}
			else
				perror("not verified\n :");
		}
	}

	printf("1. personal chat\t2. chat box\t3. log out\n\n");
	scanf("%d",&choice);

	switch(choice)
	{
		case 1 : send_data.state = 0; 
				 break;
		case 2 : send_data.state = 1;
				 break;
		case 3 : printf("Loged out....\n");    
				 exit(5);
	}

	while (strcmp(client_buff,"quit"))
	{
		FD_ZERO(&readfds);
		FD_SET(fileno(stdin), &readfds);
		FD_SET(sock_fd, &readfds);

		ret = select(sock_fd+1, &readfds, NULL, NULL, NULL);

		if (ret < 0)
			error(1, errno, "poll error");

		if (FD_ISSET(fileno(stdin), &readfds)) {
			/* clent data enter */
			getchar();
			//printf("You : ");
			//if (fgets(client_buff, sizeof(client_buff), stdin) == NULL)
			//	error(1, errno, "STDIN Read Failed");
			
			scanf("%[^\n]", client_buff);

			//printf("1data : %s\n",client_buff);	
			strcpy(send_data.msg, client_buff);
			//printf("2data : %s\n",send_data.msg);	
			strcpy(send_data.usr, usr_data.usr_name);

			/* send client data */
			c_size = send(sock_fd, (void*)&send_data, sizeof(send_data), 0);
			if (c_size == -1)
				error (1, errno, "send");
		}

		if (FD_ISSET(sock_fd, &readfds)) {
			len = recvfrom(sock_fd, (void*)&recv_data, sizeof(recv_data), 0, NULL, NULL);

			if (len)
				printf("%s : %s\n", recv_data.usr, recv_data.msg);
		}
	}
	close(data_sock_fd);
	/* close the socket now */
	close(sock_fd);
	return 0;
}
