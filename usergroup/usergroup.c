#include<stdio.h>
#include<stdlib.h>
#include "usergroup.h"

struct node_usergroup *head_usergroup = NULL;

int user_count = 0;


int insert_usergroup(usergroupinfo_t *usergroup_info)
{
    struct node_usergroup *newnode,*temp;
    newnode = (struct node_usergroup*)malloc(sizeof(struct node_usergroup));
    if(newnode == NULL)
    {
        printf("OVERFLOW\n");
        return -1;
    }
    else
    {
    	memcpy(&newnode->data_usergroup,usergroup_info,sizeof(usergroupinfo_t));
//        strcpy(newnode->data_usergroup.client_name,name);
//       newnode->data_usergroup.group_id =groupid;
        if(head_usergroup == NULL)
        {
            newnode -> next = NULL;
            head_usergroup = newnode;
           // printf("Node inserted\n");
        }
        else
        {
            temp = head_usergroup;
            while (temp -> next != NULL)
            {
                temp = temp -> next;
            }
            temp->next = newnode;
            newnode->next = NULL;
        }
        user_count++;
        printf("user_count : %d\n",user_count);
    }
    return 0;
}

int usergroup_search(int client_sock)
{
    struct node_usergroup *newnode;
    int i=0;
    int flag=1;
    newnode = head_usergroup;
    if(newnode == NULL)
    {
        printf("Empty List\n");
    }
    else
    {
        while (newnode!=NULL)
        {
        		if(newnode->data_usergroup.sock == client_sock)
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

usergroupinfo_t *get_node_usergroup_data(int index)
{
    struct node_usergroup *newnode;
    int i=0;

    newnode = head_usergroup;
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
        		  return &newnode->data_usergroup;
        	  }
        	  else
        	  {
        		  newnode = newnode-> next;
        	  }
        }
    }
    return NULL;
}
usergroupinfo_t *find_user_in_group(int groupid , int user)
{
	 struct node_usergroup *newnode;
	 int i=0;
	 int count = 0;
	 newnode = head_usergroup;
	 if(newnode == NULL)
	 {
	       printf("Empty List\n");
	 }
	 else
	 {
	     for(i=0; i<user_count; i++)
	      {
	    	  if(newnode->data_usergroup.group_id == groupid)
	    	  {
	    		  count++;
	    		  if(count == user)
	    		  {
	    			  return &newnode->data_usergroup;
	    		  }
	    	  }
	          else
	          {

	          }
	         newnode = newnode-> next;
	     }
	     //printf("groupuser_count : %d",count);
	 }
	 return NULL;
}
int get_user_count (int group_id)
{
	 struct node_usergroup *newnode;
	 int i=0;
	 int groupuser_count = 0;
	 newnode = head_usergroup;
	 if(newnode == NULL)
	 {
	       printf("Empty List\n");
	 }
	 else
	 {
	      while (newnode!=NULL)
	      {
	    	  if(newnode->data_usergroup.group_id == group_id)
	    	  {
	    		  groupuser_count++;
	    	  }
	          else
	          {

	          }
	          i++;
	         newnode = newnode-> next;
	     }
	     printf("groupuser_count : %d",groupuser_count);
	 }
	 return groupuser_count;
}


int display_usergroup(void)
{
    struct node_usergroup *newnode;
    newnode = head_usergroup;
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
            printf("%d sock : %d .group\n",newnode->data_usergroup.sock,newnode->data_usergroup.group_id);
            newnode = newnode -> next;
        }

    }
    return 0;
}

