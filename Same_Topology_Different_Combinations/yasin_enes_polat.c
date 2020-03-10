//Yasin Enes Polat 150117015
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

typedef struct node
{
    int data, depth;
    struct element *pn;
    struct node* left;
    struct node* right;
}node, *nodeptr;


typedef struct element
{
	struct node *n;
	struct element *next;
}elem, *elmptr;

int depth = 0;
int total = 0;
int totalseq = 0;
struct node* last = NULL;
struct node* root_;
FILE *file;

void printSeq (elmptr first) {
	while (first != NULL) {
		node *a = first->n;
		printf("%d ", a->data);
		first = first->next;
	}
}

struct element* createSeq (elmptr el, nodeptr n) {
	if (el == NULL) 
	{
		el = (elmptr)malloc(sizeof(struct element));
		el->n = n;
		el->next = NULL;
		return el;
	}
	else el->next = createSeq(el->next, n);
	
}

struct node* createNode(value, depth){
    struct node* newNode = malloc(sizeof(struct node));
    newNode->data = value;
    newNode->depth = depth;
    newNode->pn = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    last = newNode;
    return newNode;
}
    
struct node* insert(struct node* root, int data, int depth)
{ 
    if (root == NULL) return createNode(data, depth);
    if (data < root->data)
        root->left  = insert(root->left, data, depth + 1);
    else if (data > root->data)
        root->right = insert(root->right, data, depth + 1);   
 
    return root;
}
void inorder(struct node* root)
{
    if(root == NULL) return;
    inorder(root->left);
    if (root->depth > depth) depth = root->depth;
    printf("  Data: %3d Depth: %3d\n", root->data, root->depth);
    inorder(root->right);
}

int existOnPath (nodeptr node, nodeptr r, int sq[], int i) { //Checks if the nodes must come before "node" exists on that path. 
	int k, j;
	if (r->data == node->data) return 1;
	for (j = 0, k = 0; j < i; j++) {
		if (sq[j] == r->data) {
			k = 1;
			break;
		}
	}
	if (k != 1) return k;
	if (node->data < r->data) k = existOnPath(node, r->left, sq, i);
	else k = existOnPath(node, r->right, sq, i);
	return k;
}

void createAnotherSeq (nodeptr r, int sq[], int i, int pre[]) { //This function creates unique paths that gives same bst with the sequence user enters.
	if (i == total) {
		int j = 0;
		int c = 0;
		for (; j < total; j++) if (sq[j] != pre[j]) c = 1; //This statement checks if the sq array same as previous sequence array. Sadece bir oncekiyle ayni ciktigi icin boyle bir cozum buldum. Rastgele ayni cikmiyor hicbir zaman.
		if (c == 1) {
			for (j = 0; j < total; j++) pre[j] = sq[j];
			//for (; j < i; j++) printf("%d ", sq[j]);
			//puts("");
			for (j = 0; j < i; j++) fprintf(file, "%d ", sq[j]);
			fprintf(file, "\n");
			totalseq++;
			return;
		}
		else return;
		
		
	}
	int j;
	for (j = 0; j < i; j++) if (sq[j] == r->data) return;
	if (!existOnPath(r, root_, sq, i)) return;
	sq[i] = r->data;
	elmptr e = r->pn;
	while (e != NULL) {
		createAnotherSeq(e->n, sq, i+1, pre);
		e = e->next;
	}
	return;
}

int onPath (nodeptr node, nodeptr check, nodeptr r) { 
	int k;
	if (r == NULL) return 0; //check path uzerinde degil.
	if (check->data == r->data) {
		return 1; //check path uzerinde.
	}
	if (node->data < r->data) k = onPath(node, check, r->left);
	else k = onPath(node, check, r->right);
	return k;
}

struct element* add (elmptr e, nodeptr a) { //Adds node to given linked list. 
	elmptr first = e;
	if (e == NULL) { 
		e = malloc(sizeof(struct element));
		e->n = a;
		e->next = NULL;
		return e;
	}
	elmptr pre = NULL;
	while (e != NULL) {
		pre = e;
		e = e->next;
	}
	e = malloc(sizeof(struct element));
	e->n = a;
	pre->next = e;
	e->next = NULL;
	return first;
}

void pnAdd (nodeptr node, nodeptr a, int i) { //Adds possible nexts to the given nodes.
	if (i == 1) {
		node->pn = add(node->pn, a);
		return;
	}
	if (!onPath(node, a, root_)) {
		node->pn = add(node->pn, a);
	}
}

void traverseTree2 (nodeptr node, nodeptr root) {
	if (root == NULL) return;
	if (root->data == node->data) {
		if (root->left != NULL)  pnAdd(node, root->left,  1);
		if (root->right != NULL) pnAdd(node, root->right, 1);
		return;
	}
	pnAdd(node, root, 0);
	traverseTree2(node, root->left);
	traverseTree2(node, root->right);
}


void traverseTree (nodeptr root) {
	if (root == NULL) return;
	traverseTree2(root, root_);
	traverseTree(root->left);
	traverseTree(root->right);
}

int main () 
{
	struct node* root = NULL;
	struct element *first = NULL;
	last = malloc(sizeof(struct node));
	puts("Program gets number until you enter a 0.");
	printf("Please enter a number : ");
	int num;
	scanf("%d", &num);
	root = insert(root, num, depth);
	root_ = root;
	first = (elmptr)malloc(sizeof(struct element));
	first->n = root; 
	first->next = NULL;
	total++;
	while (num != 0) {
		printf("Please enter a number : ");
		scanf("%d", &num); 
		if(num != 0) 
		{
			insert(root, num, depth);
			createSeq(first, last);
			total++;
		}
	}
	system("cls");
	puts("-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-");
	printf("Sequence you enter : ");
	printSeq(first);
	puts("\n\n---IN-ORDER TRAVERSAL---");
	inorder(root);
	
	printf("\nTotal Nodes : %3d", total);
	traverseTree(root);
	
	if ((file = fopen("sequences.txt", "w")) == NULL) puts("File Couldn't Opened!");
	printf("\nCalculating . . .");
	int sq[total];
	int prev[total];
	int a = 0;
	for (; a < total; a++) prev[a] = 0;
	createAnotherSeq(root_, sq, 0, prev);
	printf("\rTotal sequence number that gives same BST : %3d", totalseq);
	printf("\nAll sequences printed in 'sequences.txt' file in the same directory with this .c file!");
	
	return 1;
}
