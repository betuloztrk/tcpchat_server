#include<stdio.h>
#include<stdlib.h>
#include "userbase.h"

struct node *head = NULL;

int client_count = 0;

int lastinsert(userinfo_t *userinfo)
{
    struct node *newnode,*temp;
    newnode = (struct node*)malloc(sizeof(struct node));
    if(newnode == NULL)
    {
        printf("OVERFLOW\n");
        return -1;
    }
    else
    {
    	memcpy(&newnode->data,userinfo,sizeof(userinfo_t));
//        newnode->data.sock = sock;
//        strcpy(newnode->data.nick_name,name);
        if(head == NULL)
        {
            newnode -> next = NULL;
            head = newnode;
           // printf("Node inserted\n");
        }
        else
        {
            temp = head;
            while (temp -> next != NULL)
            {
                temp = temp -> next;
            }
            temp->next = newnode;
            newnode->next = NULL;

            //printf("Node inserted\n");
        }
        client_count++;
        printf("insert  client count : %d\n",client_count);
    }
    return 0;
}

int search(char name[100])
{
    struct node *newnode;
    int i=0;
    int flag=1;
    newnode = head;
    if(newnode == NULL)
    {
        printf("Empty List\n");
    }
    else
    {
        while (newnode!=NULL)
        {
        	int res = strcmp(newnode->data.nick_name,name);
            if(res == 0)
            {
                printf("nick_name found at location %d\n",i+1);
                flag=0;
                return i+1;
            }
            else
            {
                //flag=1;
            }
            i++;
            newnode = newnode-> next;
        }

        if(flag==1)
        {
        	printf("nick_name not found\n");
        }
    }
    return -1;
}

int client_delete(int index)
{
    struct node *newnode,*newnode2;

    newnode=head;
    for(int i=0; i<index; i++)
    {
        newnode2 = newnode;
        newnode = newnode ->next;

        if(newnode == NULL)
        {
           // printf("Can't delete");
            free(newnode);
            //return -1;
        }
    }
    newnode2 ->next = newnode ->next;
    free(newnode);
    printf("Deleted node %d ",index+1);

    return 0;
}

userinfo_t *get_node_data(int index)
{
    struct node *newnode;
    int i=0;

    newnode = head;
    if(newnode == NULL)
    {
        printf("Empty List\n");
    }
    else
    {
        for(i=0; i <= index ; i++)
        {
        	  if(i == index)
        	  {
        		  return &newnode->data;
        	  }
        	  else
        	  {
        		  newnode = newnode-> next;
        	  }
        }
    }
    return NULL;
}

int deleteAllNodes(void)
{
	struct node *temp;
    while(head != NULL)
    {
      temp = head;
      head = head->next;
      free(temp);
    }
    return 0;
}

int socket_search(int sock)
{
    struct node *newnode;
    int i=0;
    int flag=1;
    newnode = head;
    if(newnode == NULL)
    {
        printf("Empty List\n");
    }
    else
    {
        while (newnode!=NULL)
        {
            if((newnode->data.sock == sock))
            {
                printf("sock found at location %d\n",i+1);
                flag=0;
                return i+1;
            }
            else
            {
                //flag=1;
            }
            i++;
            newnode = newnode-> next;
        }

        if(flag==1)
        {
        	printf("nick_name not found\n");
        }
    }
    return -1;
}

int display(void)
{
    struct node *newnode;
    newnode = head;
    if(newnode == NULL)
    {
        printf("all socket deleted\n");
        return -1;
    }
    else
    {
        printf("socket numbers and member ıd : \n");
        while (newnode!=NULL)
        {
            printf("%u sock : %s  %ld thread id \n",newnode->data.sock,newnode->data.nick_name,newnode->data.thread);
            newnode = newnode -> next;
        }
    }
    return 0;
}
