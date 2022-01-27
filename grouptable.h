
#ifndef GROUPTABLE_GROUPTABLE_H_
#define GROUPTABLE_GROUPTABLE_H_

#include <string.h>

/*Types *****************************************************************/
typedef struct {
	char group_name[100];

}groupinfo_t;

struct node_group
{
	groupinfo_t data_group;
    struct node_group *next;
};


extern int insert_group(char name[100]);
extern groupinfo_t *get_node_data_group(int index);
extern int display_group(void);

extern int  group_count;

#endif /* GROUPTABLE_GROUPTABLE_H_ */
