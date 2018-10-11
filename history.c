
#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"


typedef struct s_Item {
  int id;
  char* str;
  struct s_Item* next;
} Item;


typedef struct s_List {
  struct s_Item* root;
} List;


List* init_history() {
	List* list = (List*)malloc(sizeof(List));
	list->root = NULL;
	return list;
}


void add_history(List* list, char* str) {
	if (list == NULL || str == NULL) return; // Pointers not set, end program

	Item* node = list->root; // Get head of list
	int ind = 1;
	while (node != NULL && node->next != NULL) {
		node = node->next;
		ind++;
	}

	/*	Initialize new Item node in history list  */
	Item* newItem = (Item*)malloc(sizeof(Item)); // Allocate new list node memory
	newItem->str = (char*)malloc(sizeof(char)*string_length(str)+1); // Allocate string member memory
	for (int i = 0; i < string_length(str); i++)
			(newItem->str)[i] = str[i]; // Copy string values
	(newItem->str)[string_length(str)] = '\0'; // End string
	newItem->id = ind; // Assign list position
	newItem->next = NULL; // End list

	if (node == NULL) list->root = newItem; // Root (first history item) is not yet initialized, assign it the value of newItem.
	else {
		newItem->id++;
		node->next = newItem;
	}
}


char* get_history(List* list, int id) {
	if (list == NULL || id < 1) return NULL; // No list struct passed, end program
	else if (list->root == NULL) return NULL; // No nodes in list, end program

	Item* node = list->root;
	while (node != NULL && id > 1) {
		node = node->next;
		id--;
	}
	if (id != 1 || node == NULL) return NULL; // Invalid selection, end program
	return node->str;
}


void print_history(List* list) {
	if (list == NULL) return; // No list struct passed, end program
	else if (list->root == NULL) return; // No nodes history, end program
	Item* node = list->root;
	printf("History:\n");
	while (node != NULL) {
		printf("[%d] \'%s\'\n", node->id, node->str);
		node = node->next;
	}
	printf("\n");
}


void free_history(List* list) {
	Item* head = list->root;
	while (head != NULL) {
		Item* next = head->next;
		free(&head->id);
		char* str = head->str;
		for (int i = 0; str != NULL && str[i] != '\0'; i++)
			str[i] = '\0';
		free(head->str);
		head = NULL;
		free(head);
		head = next;
	}
	list->root = NULL;
	free(list->root);
	list = NULL;
	free(list);
}
