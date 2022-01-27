

#ifndef USERGROUP_USERGROUP_H_
#define USERGROUP_USERGROUP_H_

#include <string.h>

/*Types *****************************************************************/
typedef struct {
	char client_name[100];
	int group_id;
	int sock;

}usergroupinfo_t;

struct node_usergroup
{
	usergroupinfo_t data_usergroup;
    struct node_usergroup *next;
};


extern int insert_usergroup(usergroupinfo_t *usergroup_info);
extern int display_usergroup(void);
extern int usergroup_search(int client_sock);
extern usergroupinfo_t *get_node_usergroup_data(int index);
extern usergroupinfo_t *find_user_in_group(int groupid , int user);
extern int get_user_count (int group_id);


extern int user_count ;

#endif /* USERGROUP_USERGROUP_H_ */
