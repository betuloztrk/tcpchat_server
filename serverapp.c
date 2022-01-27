#include <stdio.h>
#include <sys/socket.h>
#include <stdint.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/syscall.h>
#include <signal.h>
#include <unistd.h>
#include "msgprotocol/msgprotocol.h"
#include "sendrecv/sendrecv.h"
#include "userbase/userbase.h"
#include "grouptable/grouptable.h"
#include "usergroup/usergroup.h"

void send_memberlist(void *thread_data);
void *receive_msg(void *thread_data);
int system_exit(void);
void sig_handler(int signal);
int socket_desc;
struct sockaddr_in server_addr,client_addr;
int client_struct_length = sizeof(client_addr);
//pthread_t thread_recv;
protocol_t packet;
protocol_t *p = &packet;
//int client_sock;
char serialize[100];
int id;
char nick_name[100];
char choice_name[100];
char data_buff[100] = {0};

userinfo_t *userdata = NULL;
userinfo_t *usertodata = NULL;
userinfo_t new_user;

usergroupinfo_t new_group;
usergroupinfo_t *usergroup_data = NULL;

typedef struct
{
	pthread_t thread_recv;
	int client_sock;

}thread_data_t;

thread_data_t thread_data;

void send_memberlist(void *thread_data)
{
		for(int i=0; i<client_count; i++)
		{
			usertodata= get_node_data(i);
			if(usertodata != NULL)
			{
				p->destination = usertodata->sock;
				p->source = 0;
				p->cmd = USERDATA;

				//printf("userdata to %d\n", p->destination);

				for(int j=0; j<client_count; j++)
				{
					userdata = get_node_data(j);

					if(userdata != NULL)
					{
						//printf("userdata for %d\n", userdata->sock);
						p->data[0] = userdata->sock;
						memcpy(&p->data[1], userdata->nick_name, strlen(userdata->nick_name));
						p->len = strlen(userdata->nick_name) + 1;

						sendrecv_send(p,usertodata->sock,serialize);
						memset(&p->data[1],0,sizeof(p->data));
					}
				}
			}
		}
}

void *receive_msg (void *thread_data)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

	int list_index = -1;
	int g_count = -1;
	//int usergroup_index = -1;
	thread_data_t dt = *(thread_data_t *) thread_data;
	//printf("socket_desc: %u\n",*(int*)socket_desc);
	printf("socket_desc: %u\n",dt.client_sock);
	printf("Thread ID : %lu\n",syscall(SYS_gettid));
	int packet_lenght = 0;
	int parsed_lenght = 0;

	while(1)
	{
		packet_lenght = recv(dt.client_sock,serialize,100,0);
		if(packet_lenght <= 0)
		{
			close(dt.client_sock);
			printf(" server can't recv\n");
			return 0;
		}

		while((packet_lenght - parsed_lenght) > 0)
		{
			parsed_lenght += msgprotocol_parse(p,&serialize[parsed_lenght]);

			if(p->cmd == REGISTER)
			{
				new_user.sock = dt.client_sock;
				new_user.thread = syscall(SYS_gettid);

				strcpy(new_user.nick_name,p->data);

				printf("%s has joined\n",p->data);
				lastinsert(&new_user);
				display();
				memset(p->data,0,sizeof(protocol_t));

				p->source = 0;
				p->data[0] = dt.client_sock;
				p->destination = 255;
				p->len = 1;

				sendrecv_send(p,dt.client_sock,serialize);
				memset(p->data,0,sizeof(protocol_t));

				send_memberlist(thread_data);

			}
			else if (p->cmd == CHOICE)
			{
				printf("message going to %s nick_name\n",p->data);
				list_index = search(p->data);

				memset(p->data,0,p->len);
			}

			else if(p->cmd == MESSAGE)
			{
				printf("%s: message going to client \n",p->data);
				if(list_index > -1)
				{
					userdata =get_node_data(list_index-1);

					if(userdata != NULL)
					{
						printf("%d socket found\n",userdata->sock);
						p->destination = dt.client_sock;
						sendrecv_send(p,userdata->sock,serialize);
					}
					else
					{
						printf("not found client !!");
					}

				}

			}
			else if (p->cmd == DELETE)
			{
				printf("%s has left \n",p->data);
				list_index = search(p->data);
				printf("list_index = %d\n",list_index);
				if(list_index > -1)
				{
					client_delete(list_index-1);
					display();
				}
				sendrecv_send(p,userdata->sock,serialize);
			}
			else if(p->cmd == GROUP)
			{
				printf("cmd girdi.");
				printf("%s name group build\n",p->data);
				//strcpy(new_group.group_name,p->data);
				insert_group(p->data);
				printf("group_count : %d , group_name : %s\n",group_count,p->data);

				//memset(p->data,0,p->len);
			}
			else if(p->cmd == GROUP_CHOICE)
			{
				printf("%d sock number client adding to %d number group\n",p->data[0],p->data[1]);

				new_group.sock = p->data[0];
				new_group.group_id = p->data[1];

				insert_usergroup(&new_group);
				display_usergroup();

				g_count=get_user_count(p->data[1]);
				printf("g_count : %d\n",g_count);

			}
			else if( p->cmd == GROUP_MSG)
			{
				g_count=get_user_count(p->data[0]);
				printf("group msg count : %d\n",g_count);
				printf(" %s :message going to clients\n",&p->data[1]);

				for(int i=1; i<=g_count; i++)
				{
					usergroup_data = find_user_in_group(p->data[0],i);
					if(usergroup_data != NULL)
					{
						printf("%d socket found\n",usergroup_data->sock);
						sendrecv_send(p,usergroup_data->sock,serialize);
					}
			    }
			}
		}
		parsed_lenght = 0;
		memset(p,0,sizeof(protocol_t));
	}

}

void sig_handler(int signal)
{
	if(signal == SIGHUP)
	{
		printf("\nreceived SIGHUP\n");
	}
	else if(signal == SIGINT)
	{
		printf("\nreceived SIGINT\n");
		system_exit();
		deleteAllNodes();
		display();
	}
	else if(signal == SIGKILL)
	{
		printf("\nreceived SIGKILL\n");
	}
	else if(signal == SIGPIPE)
	{
		printf("\nreceived SIGPIPE\n");

	}
	else if(signal == SIGQUIT)
	{
		printf("\nreceived SIGQUIT\n");
	}
}

int main()
{
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);

	if(socket_desc < 0)
	{
		printf("Error while creating socket\n");
	}
	printf("Socket created successfully\n");

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(2005);
	server_addr.sin_addr.s_addr = inet_addr("192.168.43.153");


	if( bind(socket_desc,(struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		printf("Couldn't bind to the port\n");
		return -1;
	}
	printf("Done with binding\n");
	printf("Waiting for incoming connections...");

	if(listen(socket_desc,3)==0)
	printf("Listening\n");
	else
	printf("Error\n");

	 while(1)
	 {
		 thread_data.client_sock = accept(socket_desc, (struct sockaddr *)&server_addr, (socklen_t*)&client_struct_length);
		 if (thread_data.client_sock < 0)
		 {
			perror("accept failed");
			return 1;
		 }

		 if( pthread_create( &thread_data.thread_recv, NULL , receive_msg, (void*) &thread_data) < 0)
		 {
			perror("could not create thread");
			return 1;
		 }

		 signal(SIGHUP,sig_handler);
		 signal(SIGINT,sig_handler);
		 signal(SIGKILL,sig_handler);
		 signal(SIGPIPE,sig_handler);
		 signal(SIGQUIT,sig_handler);

		 if (thread_data.client_sock < 0)
		 {
			printf("accept failed");
		 }
	}
	return 0;
}

int system_exit(void)
{
	int index = 0;

	for(index =0; index < client_count ; index++)
	{
		userdata = get_node_data(index);
		printf("sock %d thread id %ld\n",userdata->sock,userdata->thread);

		pthread_cancel(userdata->thread);
		close(userdata->sock);
	}
	//pthread_join(userdata->thread, NULL);
	printf("closing socket...\n");
	close(socket_desc);
	shutdown(socket_desc,3);

	return 0;

}

