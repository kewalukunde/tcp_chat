#include <tcp_chat.h>

cli_data *create_node(char* usr,int pass);

int insert_client_data(cli_data **head,char *usr,int pass)
{
		  cli_data *ptr;

		  if (*head == NULL) {
					 *head = create_node(usr,pass);
					 if (*head == NULL)
						 return 1;
		  } else {
					 ptr = *head;
					 while (ptr->next != NULL)
								ptr = ptr->next;

					 ptr->next = create_node(usr,pass);
		  }
		  return 0;
}

cli_data *create_node(char* usr,int pass)
{
		  cli_data *new = calloc(1,sizeof(cli_data));
		  if (new != NULL) {
					 strcpy(new->usr,usr);
					 new->pass = pass;
					 new->next = NULL;
		  } else
			  return NULL;

		  return new;
}

int delete_user(cli_data **head,char *elm)
{
		  cli_data *prv,*ptr;

		  ptr = *head;
		  if (*head == NULL)
					 return 1;

		  while (!strcmp(ptr->usr, elm)) {   
					 prv = ptr;
					 ptr = ptr->next;
		  }

		  if ((!strcmp(ptr->usr,elm)) && (ptr->next == NULL))
					 return 1;
		  else if (ptr == *head) {
					 *head = ptr->next;
					 free(ptr);
		  } else {
					 prv->next = ptr->next;
					 free(ptr);
		  }
		  return 0;
}

int find_node(cli_data **head,char *elm)
{
		  cli_data *ptr;

		  ptr = *head;
		  if (*head == NULL)
					 return 0;

		  while((!strcmp(ptr->usr,elm)) && (ptr->next != NULL))
					 ptr = ptr->next;

		  if (!strcmp(ptr->usr,elm))
					 return 1;
		  else if (ptr->next == NULL)
					 return 0;
		  return 0;
}

int print_list(cli_data *head)
{
	cli_data *ptr;
	int i = 1;

	if (head == NULL)
		return 1;

	ptr = head;

	while(ptr != NULL) {
		printf("%d] %s\t%d\n",i,ptr->usr,ptr->pass);
		ptr = ptr->next;
		i++;
	}
	return 0;
}
