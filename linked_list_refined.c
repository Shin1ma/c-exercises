#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

#define ERROR_CODE	-404

typedef struct Node{	//node of a list
	double value;
	struct Node* next;
} node_t;
typedef struct LinkedList{	//linkedlist structure
	int size;
	struct Node* head;
} linklist_t;

//utils
void* error(char*);	//returns NULL and prints a string
int intError(char*);	//returns ERROR_CODE and prints a string	

//modify/create linked list
linklist_t* createLinked();	//allocates memory and initializes a linked list
node_t* listPushBack(linklist_t*, double);	//pushes back an element as new tail of the list
node_t* listPopIn(linklist_t*, double, int); //pops an element in an index, pushing the element previously there after it
node_t* listReplace(linklist_t*, double, int); //replaces the value of a node
int deleteNode(linklist_t*, int);	//deletes a node
int resize(linklist_t*, int);	//resizes the entire list if resize value is bigger initializes elements with value 0
int destroyList(linklist_t*);	//destroys and deallocates the list

//get values from linked list
node_t* listGet(linklist_t*, int);	//gets a pointer to a node on the list at index (operates on range 1-size)
double getValue(linklist_t*, int);	//returns a value from the node at index

//implementation
void pushBackCommand(linklist_t*);
void popInCommand(linklist_t*);
void replaceCommand(linklist_t*);
void deleteCommand(linklist_t*);
void resizeCommand(linklist_t*);
void printList(linklist_t*);

int main(){
	char userInput[255];
	linklist_t* list = createLinked();
	
	while(true){
		printf("\nlist:	");
		printList(list);
		
		printf("\n\n\nhi! please use:\n'1' to add an element to the list(pushback)\n'2' to add an element to the list(pop-in)\n");
		printf("'3' to replace an element\n'4' to delete an element\n'5' to resize the list\ntype exit to exit\n\n");
		printf("//: ");
		scanf("%s", userInput);
		if(!(strcmp(userInput, "exit") * strcmp(userInput, "Exit"))) break;
	
		switch(atoi(userInput)){
			case 1:
				pushBackCommand(list);
				break;
			case 2:
				popInCommand(list);
				break;
			case 3:
				replaceCommand(list);
				break;
			case 4:
				deleteCommand(list);
				break;
			case 5:
				resizeCommand(list);
				break;
			default:
				printf("please choose a valid prompt\n");
				break;
		}
	}
	
	destroyList(list);
	return 0;
}


void pushBackCommand(linklist_t* list){
	double val;
	printf("please enter a value: ");
	scanf("%lf", &val);
	listPushBack(list, val);
}
void popInCommand(linklist_t* list){
	int index;
	double val;
	printf("please enter a index(from 1 to %d): ", list->size);
	scanf("%d", &index);
	printf("please enter a value: ");
	scanf("%lf", &val);
	listPopIn(list, val, index);
}
void replaceCommand(linklist_t* list){
	int index;
	double val;
	printf("please enter a index(from 1 to %d): ", list->size);
	scanf("%d", &index);
	printf("please enter a value: ");
	scanf("%lf", &val);
	listReplace(list, val, index);
}
void deleteCommand(linklist_t* list){
	int index;
	printf("please enter a index(from 1 to %d): ", list->size);
	scanf("%d", &index);
	deleteNode(list, index);
}
void resizeCommand(linklist_t* list){
	int index;
	printf("please a non-negative size: ");
	scanf("%d", &index);
	resize(list, index);
}

void printList(linklist_t* list){
	int i;
	for(i = 1; i <= list->size; i++){
		printf("%lf, ", getValue(list, i));
	}
}


void* error(char* str){
	printf("\n%s\n", str);
	return NULL;
}

int intError(char* str){
	printf("\n%s\n", str);
	return ERROR_CODE;
}


linklist_t* createLinked(){
	linklist_t* listPtr = malloc(sizeof(linklist_t));
	if(!listPtr) return error("list memory hasn't been allocated, can't create list");
	
	//init
	listPtr->head = NULL;
	listPtr->size = 0;
	return listPtr;
}

node_t* listPushBack(linklist_t* list, double val){
	node_t* newNode = malloc(sizeof(node_t));	//allocates necessary memory for the new node
	node_t* tail = (list->head) ? listGet(list, list->size) : NULL;	//used to identify the end of the list
	if(!newNode) return error("new pushback node memory hasn't been allocated, can't pushback node");
	
	//init
	newNode->next = NULL;
	newNode->value = val;
	
	if(list->head){	
		if(!tail) return error("");		//checks for errors in listGet and then updates tails next
		tail->next = newNode;
		(list->size)++;
		return newNode;
	}
	else{	//if the list is empty (head is null) we need to update it
		list->head = newNode;	//updates the head
		(list->size)++;
		return newNode;
	}
	
	return error("something wen't wrong with end search, can't pushback");
}

node_t* listPopIn(linklist_t* list, double val, int index){
	node_t* newNode = malloc(sizeof(node_t)); //allocates memory for a new node and prepares usefull vars
	node_t* indexNode = listGet(list, index);
	node_t* previousNode = (index > 1) ? listGet(list, index - 1) : NULL;	//handles the fact of the node being the head node
	if(!indexNode) return error("");
	if(!newNode) return error("can't allocate memory for new node, failed pop in");
	
	if(index == 1){	//if the index is 1 then the node it needs to be in place of is the head node
		newNode->next = indexNode;	//setting up so original indexNode doesn't get lost, just gets placed after our new node
		newNode->value = val;
		list->head = newNode;	//sets the new node as head
		(list->size)++;
	}
	else{
		if(!previousNode) return error(""); //handles errors of listGet
		newNode->next = indexNode;	//just placing indexNode after our new node
		newNode->value = val;
		previousNode->next = newNode;	//placing it where our original indexNode was
		(list->size)++;
	}
	
	return newNode;
}

node_t* listReplace(linklist_t* list, double val, int index){
	node_t* indexNode = listGet(list, index);	//just gets the pointer to the node from listGet and changes the value
	if(!indexNode) error("");
	
	indexNode->value = val;
	return indexNode;
}

int deleteNode(linklist_t* list, int index){
	node_t* previousNode = (index > 1) ? listGet(list, index - 1) : NULL;	//gets the index node and the previous node to relink it when it deletes the index node
	node_t* indexNode = listGet(list, index);
	if(!indexNode) return intError("");
	
	if(index == 1){	//if the index is one then we need to relink head, not a node
		if(indexNode->next) list->head = indexNode->next;	//checks if there is a node after the index one if there is it's linked to head
		else list->head = NULL;	//if there isn't then head is NULLED
		free(indexNode);	//free memory and decrement size
		(list->size)--;
		return 0;
	}
	else{
		if(!previousNode) return intError("");
		
		if(indexNode->next) previousNode->next = indexNode->next;	//checks if theres another node to relink the previous one if there is link them
		else previousNode->next = NULL;	//if there isn't just make the list end there
		free(indexNode);	//fre memory and decrement
		(list->size)--;
		return 0;
	}
	
	return intError("somehow search failed, can't delete");
}

int resize(linklist_t* list, int newSize){
	int offset = newSize - list->size; //calculates how many items we need to add/delete
	if(newSize < 0) intError("size needs to be at least zero, can't resize");
	
	if(offset == 0) return 0; //if the offset is 0 then the sizes are equal and we are done
	
	int i;
	if(offset > 0){	//we need to add elements on the tail
		for(i = 1; i <= offset; i++){
			listPushBack(list, 0);
		}
	}
	else{	//we need to delete elements from the tail
		for(i = 0; i < abs(offset); i++){
			deleteNode(list, list->size);
		}
	}
	return 0;
}

int destroyList(linklist_t* list){
	if(resize(list, 0)) return intError("list not destroyed");
	free(list);
	return 0;
}


node_t* listGet(linklist_t* list, int index){
	node_t* currentNode = (list->head) ? list->head : NULL;	//used to get trough the list, starts from head as 1
	if(!(list->head)) return error("zero sized list, can't get");
	if(index < 1 || index > list->size) return error("index out of range, can't get");
	
	int i;
	for(i = 1; i <= list->size; i++){
		if(i == index) return currentNode;
		else if(!(currentNode->next)) return error("unexpected node interruption");
		else currentNode = currentNode->next;	//at the end go to the next node and increment i
	}
	
	return error("search somehow failed, can't get");
}
double getValue(linklist_t* list, int index){
	node_t* indexNode = listGet(list, index);	//just gets the pointer to the node and returns the value
	if(!indexNode) return intError("");
	else return indexNode->value;
}
