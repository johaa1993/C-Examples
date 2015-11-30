# BRS_Schedule_P

A priority sorter.

### Dependency
* BRS_Lists_DL.h


## Initialize
~~~c
static inline void BRS_Schedule_P_Initialize
(struct BRS_Lists_DL_Node * List, size_t Count, struct BRS_Schedule_P * Schedule)
~~~


| Argument  | Direction | Description   |
| --------- | ---       | ------------- |
| List      | In        | Array of priority grouped lists where elements will be stored. |
| Count     | In        | Number of lists. That is the number of priorities.      |
| Schedule  | Out       | ... |

## Insert
Insert and updates the highest priority in the schedule.
~~~c
static inline void BRS_Schedule_P_Insert
(struct BRS_Lists_DL_Node * Item, size_t Priority, struct BRS_Schedule_P * Schedule)
~~~


## Remove
Do not remove a list node of a non-empty-list.
Removing a list node from a empty-list have no effect.
~~~c
static inline void BRS_Schedule_P_Remove
(struct BRS_Lists_DL_Node * Node, struct BRS_Schedule_P * Schedule)
~~~
