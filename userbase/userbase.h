

#ifndef USERBASE_USERBASE_H_
#define USERBASE_USERBASE_H_

#include <string.h>
#include <pthread.h>

/*Types *****************************************************************/
typedef struct {
	int sock;
	char nick_name[100];
	pthread_t thread;

}userinfo_t;



struct node
{
	userinfo_t data;
    struct node *next;
};


/* Functions **************************************************************/

extern int lastinsert(userinfo_t *userinfo);
extern int display(void);
extern int search(char name[100]);
extern userinfo_t *get_node_data(int index);
extern int deleteAllNodes(void);
extern int client_delete(int index);
extern int socket_search(int sock);

extern int client_count;

#endif /* USERBASE_USERBASE_H_ */
