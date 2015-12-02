# BRS_Schedule_P
Part of Basic Real-time System (BRS).
A priority sorter. Access time = O(1).
Can be used to store task handler.
Able to iterate through highest priority list.
Other useful procedures is included.


## Dependencies
1. **BRS_CDLL.h**  
Double linked list.



## Functions

1. **Initialize**  
Initializes every list-node.
- **Insert**  
Inserts a element-node to priority designated list-node.
- **Remove**  
Removes element-node from own list-node.
Updates the current highest priority.
- **Current**  
Get the current highest priority list-node.
- **Current_Count**  
Get the number of element-nodes in the current highest priority list-node.
- **Current_Next_Node**  
Get the next element-node in the current highest priority list-node. Can be used for round-robin.
- **Current_Transfer**  
Transfer one element-node from the highest priority list-node from a schedule into a another schedule.
- **Current_Empty**  
Check if current highest priority list-node contains no element-nodes.
- **Promote**  
Change element-node's priority designation.

## Types

1. **struct BRS_CDLL_Node**  
Double linked list node.
- **struct BRS_Schedule_P**  
The schedule.
- **size_t**  
Used for countable stuff.

---


## Initialize
~~~c
static inline void BRS_Schedule_P_Initialize
(struct BRS_CDLL_Node * List, size_t Count, struct BRS_Schedule_P * Schedule)
~~~


| Argument  | Description   |
| --------- | ------------- |
| List      | Array of priority grouped lists where elements will be stored. |
| Count     | Number of lists. That is the number of priorities.      |
| Schedule  | ... |

## Insert
Insert and updates the highest priority in the schedule.
### Code
~~~c
static inline void BRS_Schedule_P_Insert
(struct BRS_CDLL_Node * Item, size_t Priority, struct BRS_Schedule_P * Schedule)
~~~
### Example
~~~c
struct Task_Handler
{
  struct BRS_CDLL_Node Node [1];
  size_t Priority;
};

struct Task_Handler Handler;

#define Priority_Count 10
struct BRS_CDLL_Node Schedule_Lists [Priority_Count];
struct BRS_Schedule_P Schedule [1];

BRS_Schedule_P_Initialize (Schedule_Lists, Priority_Count, Schedule);
BRS_Schedule_P_Insert (Handler.Node, Handler.Priority, Schedule);
~~~

---



## Remove
Removes a element-node from a non-empty-list.
Do not remove a list-node of a non-empty-list.
Removing a list-node from a empty-list have no effect.
~~~c
static inline void BRS_Schedule_P_Remove
(struct BRS_CDLL_Node * Node, struct BRS_Schedule_P * Schedule)
~~~

---


## Current
Returns the highest priority list.

---


## Current_Count
Returns number of nodes in the highest priority list.

---


## Current_Next_Node
Returns node if the list is not empty.
Returns list if the list is empty.
Can be used for round-robin.

---


## Current_Empty
Check if highest priority list is empty.
