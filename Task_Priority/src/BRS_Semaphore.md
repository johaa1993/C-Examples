# BRS_Semaphore
Part of Basic Real-time System (BRS).
A circular doubly linked list.

## Dependencies
1. **BRS_Schedule_P.h**  


## Functions

1. **Initialize**  
Initializes the semaphore.
- **Take**  
Takes the semaphore if possible otherwise transfer task to another schedule.
- **Release**  
Releases the semaphore. Then transfer the highest available priority node to another schedule.

## Types
1. **struct BRS_Schedule_P**  
Used for highest priority access and sorting.
2. **struct BRS_Semaphore**  
The semaphore in topic.
3. **size_t**  
Used for priority and element count.
