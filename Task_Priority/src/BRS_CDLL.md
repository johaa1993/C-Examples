# BRS_CDLL
Part of Basic Real-time System (BRS).
A circular doubly linked list.

## Dependencies
1. **assert.h**  
2. **BRS_Address.h**  


## Functions

1. **Link_Self**  
Initializes list-node.
- **Link_Self_Vector**  
Initializes multiple list-nodes.
- **Insert_Between**  
Low level function. Use **Insert_After** or **Insert_Before** instead.
- **Insert_After**  
Inserts a element-node.
- **Insert_Before**  
Inserts a element-node.
- **Insert_Sorted**  
Insert where comparison is true.
- **Remove_Between**  
Low level function. Use **Remove** instead.
- **Remove**  
Removes a element-node.
- **Count**  
Number of element-nodes.

## Types
1. **struct BRS_Node**
2. **size_t**  
Used for element count.
3. **BRS_CDLL_Insert_Sorted_Compare**  
Function template used for comparison in sorting.
