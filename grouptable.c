#include<stdio.h>
#include<stdlib.h>
#include "grouptable.h"

struct node_group *head_group = NULL;

int group_count = 0;

int insert_group(char name[100])
{
    struct node_group *newnode,*temp;
    newnode = (struct node_group*)malloc(sizeof(struct node_group));
    if(newnode == NULL)
    {
        printf("OVERFLOW\n");
        return -1;
    }
    else
    {
    	//memcpy(&newnode->data,userinfo,sizeof(userinfo_t));
        strcpy(newnode->data_group.group_name,name);
        if(head_group == NULL)
        {
            newnode -> next = NULL;
            head_group = newnode;
           // printf("Node inserted\n");
        }
        else
        {
            temp = head_group;
            while (temp -> next != NULL)
            {
                temp = temp -> next;
            }
            temp->next = newnode;
            newnode->next = NULL;
            //printf("Node inserted\n");
        }
        group_count++;
        printf("insert group count : %d\n",group_count);
    }
    return 0;
}

groupinfo_t *get_node_data_group(int index)
{
    struct node_group *newnode;
    int i=0;

    newnode = head_group;
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
        		  return &newnode->data_group;

        	  }
        	  else
        	  {
        		  newnode = newnode-> next;
        	  }
        }
    }
    return NULL;
}

int display_group(void)
{
    struct node_group *newnode;
    newnode = head_group;
    if(newnode == NULL)
    {
        printf("all node deleted,nothing to print\n");
        return -1;
    }
    else
    {
        printf("group numbers and group name:\n");
        while (newnode!=NULL)
        {
            printf("%d group_id : %s\n",group_count,newnode->data_group.group_name);
            newnode = newnode -> next;
        }

    }
    return 0;
}



