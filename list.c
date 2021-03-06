#ifndef LIST_H
#include "list.h"
#endif


void initList(skipList *list){

	int i;
	
	list->header = calloc(1, sizeof(node*));
	list->header->key = MAX_INT;

	//level (height) of the list doesn't use header...
	//This level will be modified later (with various insert and deletes) so we can get maximal level.
	list->level = 0;

	if((list->header->nextNode = calloc(MAX_LEVEL+1, sizeof(node*))) == 0){
        printf("Error w/ memory\n");
        exit(EXIT_FAILURE);
    }

    //4 header pointers are leading to NIL
    for(i=0; i<MAX_LEVEL; i++){
    	list->header->nextNode[i] = NIL;
    }

}

void initializeFromFile(skipList *list, char *fileAdress){
	FILE* file = NULL;

	printf("%s\n", fileAdress);
	file = fopen(fileAdress, "r");

	char tempLine[MAX_CHAR];
	int tempVal;
	int tempKey;

	if(!file) {
		printf("sorry I can't open the file lol too bad\n");
		exit(1);
	}

	/*	check if the file is just a value per line
		in this case, each value is key AND value.
		Else, if it's two integers separated with a ",",
		first number is Key and second one is Value.
	*/


	while(fgets(tempLine, MAX_CHAR, file) != NIL){
		if(strstr(tempLine, ",")==NIL){
			tempKey = atoi(tempLine);
			tempVal = tempKey;
		}
		else{
			//file is separated with a ',', like a .csv file.
			tempKey = atoi(strtok(tempLine, ","));
			tempVal = atoi(strtok(NIL, ","));
		}
		
		insertNode(list, tempKey, tempVal);
	}


		// closing file 
		fclose(file);

}

void printList(skipList *list){
	int i;
	node * x = list->header;
	for(i=list->level; i>=0; i--){
		printf("level %d\n", i);
		while(x != NIL && x->nextNode[i] != list->header){
		printf("%d[%d] -> ", x->key, x->value);
		x = x->nextNode[i];
	}
		printf("NIL\n");
		x = list->header;
	}
	printf("\n");
	

}

int insertNode(skipList *list, int key, int value){
	

	node *x = list->header;
	node *update[MAX_LEVEL];

	int i, level;

	for(i=list->level; i>=0; i--){
		while(x->nextNode[i]!= NIL && x->nextNode[i]->key < key){
			x = x->nextNode[i];
		}
		update[i] = x;
	}

	x = x->nextNode[0];

	if(x!=NIL && key == x->key){
		x->value = value;
		return 0;
	}
	else{
		level = randLevel(2.0, MAX_LEVEL);

		if(level > list->level){
			for(i=list->level+1; i<=level; i++){
				update[i] = list->header;
			}
			list->level = level;
		}
		
		x = calloc(1, sizeof(node));
		x->key = key;
		x->value = value;
		x->nextNode = calloc(level+1, sizeof(node*));

		for(i=0; i<=level; i++){
			x->nextNode[i] = update[i]->nextNode[i];
			update[i]->nextNode[i] = x;
		}

		printf("inserted node %d[%d] (level %d)\n",key, value, level);	

	}
	return 0;
}

int deleteNode(skipList *list, int key) {
	node *x = list->header;
	node *update[MAX_LEVEL+1];

	int i;
		for (i=list->level;i>=0;i--) {
			while(x!=NIL && x->nextNode[i]!=NIL && x->nextNode[i]-> key < key) {
				x = x->nextNode[i];
			}
			update[i] = x;
		}

	if(x->nextNode[0]!=NIL)
		x = x->nextNode[0];

		if (x->key == key) {

			for(i=0;i<=list->level;i++) {
				if(update[i]->nextNode[i] != x) {
					break;
				}
				update[i]->nextNode[i] = x->nextNode[i];
			}
			free(x->nextNode);
			free(x);

			while (list->level>0 && list->header->nextNode[list->level] == NIL) {
				list->level = list->level-1;
			}

	}

		return 0; 
}

int randLevel(int p, int maxLevel){
	int level = 0;
		//higher p level seems -seems- to make launch a little bit longer
		while (rand()%p==0 && level < maxLevel){
			    level++;  
		}
	return level;
}

node * searchNodeFromList(skipList *list, int key){

	node * x = list->header;
	int i;
	
	for(i=list->level; i>=0; i--){
		while(x->nextNode[i]!=NIL && x->nextNode[i]->key < key){
			x = x->nextNode[i];		
		}
	}
 
 	if(x->nextNode[0] != NIL)
		x = x->nextNode[0];

	if(x->key == key){
		return x;
	}
	else{
		return NULL;
	}
}

void freeList(skipList *list){
	node *x = list->header;
	while(x->nextNode[0]!=NIL){
		deleteNode(list, x->nextNode[0]->key);
	}
	free(x->nextNode);
	free(x);
}





