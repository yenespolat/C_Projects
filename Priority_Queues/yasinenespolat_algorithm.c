//Yasin Enes Polat 150117015

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "yasinenespolat_binQ.c"

struct dataNode {
	char name[4];
	int data;
	int coming_time;
	int wt;
	int isAdded;
	double priorityvalue;
	struct dataNode *next;
};

struct dataNode *datalist = NULL;
int emax, _totaltime_, totalprocess;
BinQueue H;

void insert (struct dataNode *data) {
	if (data == NULL) return;
	
	if (datalist == NULL) {
		datalist = malloc(sizeof(struct dataNode));
		datalist = data;
		datalist->next = NULL;	
	}
	else {
		struct dataNode *temp = datalist;
		struct dataNode *pre  = NULL;
		pre = malloc(sizeof(struct dataNode));
		while (temp != NULL) {
			pre = temp;
			temp = temp->next;
		}
		
		temp = malloc(sizeof(struct dataNode));
		temp = data;
		pre->next = temp;
		temp->next = NULL;
	}
}

void divide_add (char carr[]){ //This function creates nodes from given char array and adds them to the linked list.
	
	int i = 0;
	struct dataNode *d;
	d = malloc(sizeof(struct dataNode));
	char *token = strtok(carr, " ");
	
	while (token != NULL) {
		switch (i) {
		case 0:
			strcpy(d->name, token);
			break;
		case 1:
			d->data = atoi(token);
			break;
		case 2:
			d->coming_time = atoi(token);
			break;
		}
		i++;
		token = strtok(NULL, " ");
	}
	
	d->priorityvalue = 1 * d->data;
	d->wt = 0;
	d->isAdded = 0;
	insert(d);
	totalprocess++;
	return;	
}

void calcPriorityValue (struct BinNode *hnode) { //This function calculates the priority value of given node.
	double fcube = (2.0 * hnode->ei) / (3.0 * emax);
	double cube = fcube * fcube * fcube * -1.0;
	double value = (1.0 / exp(cube));
	double pvalue = value * hnode->ei;
	hnode->priority = pvalue;
}

void incrementWT (char p[3]) {
	struct dataNode *node = datalist;
	while (node != NULL) {
		if (strcmp(node->name, p) && node->isAdded == 1) {
				
			node->wt += 1;
			
		}
		node = node->next;
	}
	return;
}

void confirmDeleted (char p[3]) {
	struct dataNode *node = datalist;
	while (node != NULL) {
		if (!strcmp(node->name, p)) {
					
			node->isAdded = 0;
			
		}
		node = node->next;
	}
	return;
}

void calculateAWT () {
	struct dataNode *node = datalist;
	int t = 0;
	while (node != NULL) {
		printf("\n%-4s%d", node->name, node->wt);
		t += node->wt;
		node = node->next;
	}
	double awt = (double)t / (totalprocess + 1);
	printf("\nTWT : %d AWT : %.2f\n", t, awt);
}

void resetWT () {
	struct dataNode *node = datalist;
	while (node != NULL) {
		node->wt = 0;
		node = node->next;
	}
}

void checkList (int k) { //This function checks the linked list for every value of time variable, if there is any node that t_arr of that node equals to the current time, function inserts that node to the binomial heap.
	struct dataNode *node = datalist;
	while (node != NULL) {
		if (node->coming_time == k) {
					
			if (H->CurrentSize < 1)	H = Initialize();
			Insert(node->data, node->priorityvalue, k, node->name, H);
			node->isAdded = 1;
			return;
			
		}
		node = node->next;
	}
}

int processor (int *time, struct BinNode *b, int q) { //This is the processor that process the given node with given quantum size.

	if(b->priority < 1) return 0;
	b->totaltime += (*time) - b->lastT;
	int i = 0, intime = *time;
	for (; b->ei > 0; i++) {
		if (i >= q) break;
		(*time)++;
		incrementWT(b->name);
		checkList(*time);
		b->ei--;
		if (b->ei == 0) {
			_totaltime_ += b->totaltime;
			calcPriorityValue(b);
			confirmDeleted(b->name);
			DeleteMin(H);
			i++;
			break;
		}
	}
	
	if (b->ei > 0) {
		calcPriorityValue(b);
		b->lastT = (*time);
	}
	
	return q-i;
	
}

int main () {
	FILE* f;
	f = fopen("input.txt", "r");
	if (f == NULL) {
		puts("File couldn't opened!");
		return 0;
	}
		
	char line[20];
	fgets(line, 20, f);
	divide_add(line);
	totalprocess = 0;
	while (fgets(line, 20, f) != NULL) {
		divide_add(line);
	}
	
	fclose(f);
	
	
	
	int quantum = 1;
	emax = 0;
	
	struct dataNode *n = datalist;
	
	while (n != NULL) {
		if (n->data > emax) emax = n->data;
		n = n->next;
	}
	
	int time;
	int finished = 0;
	
	
	
	for (; quantum <= 10; quantum++) {
		H = Initialize();
		time = 0;
		_totaltime_ = 0;
		checkList(time);
		int r = 0;
		struct BinNode *process;
		while (!IsEmpty(H)) {
				process = FindMin(H);
				if (process->priority < 1) {
					DeleteMin(H);
				}
				process->t++;
				printf(/*"\nProcess : %f, r : %d\n"*/"", process->priority, r); //If I delete this, program will not work properly. I don't know why...
				if (r > 0) r = processor(&time, process, r);
				else r = processor(&time, process, quantum);			
		}
		calculateAWT();
		resetWT();
		Destroy(H);
	}
	
	
	return 1;
}
