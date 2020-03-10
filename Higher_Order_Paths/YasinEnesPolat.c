//Yasin Enes POLAT 150117015

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h> 
#include<locale.h>
#include <dirent.h>
#include <math.h>

typedef struct word{
	char word[15];
	int times;
	int tof;
	int sodone;
	int idf[3];
	int where[3][5];
	int timesinfolder[3];
	struct word *next;
	struct word *sortedNext;
	struct word *sortedNext_2;
} wordNode, *wordPtr;

typedef struct sorder { //For second order co-occurrence list.
	char word1[15];
	char word2[15];
	struct word *sonext;
}sorder, *soPtr;

	wordPtr firstWord = NULL;
	wordPtr sortedFirst = NULL;
	wordPtr sortedFirst_2 = NULL;
	soPtr soFirst = NULL;
	int totalTxtFile = 0;
	int connections[15][15]; //Keeps the second order connected txt indexes.
	int connectiont[15][15]; //Keeps the third order connected txt indexes.

void readFiles ();
void insertNode (wordPtr *firstWord, char *wordForNode, int folder, int file);
void printList ();
void firstOrder ();
void secondOrder ();
void sort ();
void secondOrderConnection ();
void secondOrder_ ();
void thirdOrderConnections ();
void thirdOrder_ ();
int idfCalc ();

int main()
{
	setlocale(LC_ALL, "Turkish");
	readFiles();
	printList(firstWord);
	sort(0);
	sort(1);
	sort(2);
	idfCalc();
	firstOrder();
	secondOrderConnection();
	secondOrder_();
	thirdOrderConnections();
	thirdOrder_();
    return 0;
}

void readFiles_2 (char *path, int folder, int file_num) { //Reads dataset and inserts the nodes to the main link list.
	FILE *file;
	if ((file = fopen(path, "r")) == NULL) { 
		puts("Could not open file!"); 
	}
	char fromTxt[1000];
	
	while(fgets(fromTxt, 1000, file) != NULL) {
		char *token = strtok(fromTxt, " ");
		while (token != NULL) {
			//printf("%s\n", token);
			insertNode(&firstWord, token, folder, file_num);
			token = strtok(NULL, " \t\r\n\v\f");
		}		
	}	
	fclose(file);
}

void readFiles () { //Reads dataset and inserts the nodes to the main link list.
	int txt_name = 1;
	int folder = 0;
	char fileName[6];
	char *folderName;
	char path[25];
	sprintf(fileName, "%d.txt", txt_name);
	
	int i = 0;
	struct dirent *de;
	DIR *dr = opendir("dataset");
	
	if (dr == NULL) {
		puts("Could not open current directory!");
		return;
	}
	
	FILE *file_1;
		while ((de = readdir(dr)) != NULL) {
			if (strlen(de->d_name) > 2) { 
				folderName = de->d_name;
				sprintf(path, "dataset/%s/%s", folderName, fileName);
				while (file_1 = fopen(path, "r") != NULL) {
					readFiles_2(path, folder, txt_name - 1);
					totalTxtFile++;
					txt_name++;
					sprintf(fileName, "%d.txt", txt_name);
					sprintf(path, "dataset/%s/%s", folderName, fileName);
				}
				txt_name = 1;
				sprintf(fileName, "%d.txt", txt_name);
				folder++;
			}
		}
	printf("Total Folder %d\n",totalTxtFile);
	fclose(file_1);
	closedir(dr);
	
}

int checkOccurrence (char *toCheck, int folder, int file) { //Checks if the given word is in the list or not.
	wordPtr temp;
	temp = firstWord;
	
	if (temp == NULL) {
		return;
	}
	
	while (temp != NULL) {
		if (!strcmp(temp->word, toCheck)) {
			temp->times++;
			temp->timesinfolder[folder]++;
			if (temp->where[folder][file] != 1) {
				temp->tof++;
			}
			temp->where[folder][file] = 1; //If word added before, than this line add where the word occures to word's 2-dimensional array.
			return 1;
		}
			temp = temp->next;
	} 
	return 0;
}

void insertNode (wordPtr *firstWord, char *wordForNode, int folder, int file) { //Inserts words to the main link list.
	
	if (checkOccurrence(wordForNode, folder, file))
		return;
		
	wordPtr newPtr, temp, prev;
	newPtr = malloc(sizeof(wordNode));
	
	if (newPtr == NULL)
		return;
	
	if (!strcmp("\n", wordForNode))
		return;
	
	strcpy(newPtr->word, wordForNode);
	newPtr->times = 1;
	newPtr->tof = 1;
	newPtr->where[folder][file] = 1;
	newPtr->timesinfolder[folder] = 1;
	newPtr->next = NULL;
	newPtr->sortedNext = NULL;
	newPtr->sortedNext_2 = NULL;
	
	if(*firstWord == NULL)
		*firstWord = newPtr;
		
	else { 
		temp = *firstWord; 
		while (temp != NULL) { 
			prev = temp;
			temp = temp->next;
		} 		
		
		prev->next = newPtr;
	}
	return;
}

void printList (wordPtr list) { 
	wordPtr temp;
	temp = list;
	
	if (temp == NULL) {
		puts("List is empty!");
		return;
	}
	
	while (temp != NULL) {
		printf("Word : %-15s\t\t\tTimes : %d\t\t\tF1 : %d F2 : %d F3 : %d \t\t\t", temp->word, temp->times, temp->timesinfolder[0], temp->timesinfolder[1], temp->timesinfolder[2]);
		int i;
		for (i = 0; i < 3; i++) {
			int j;
			for (j = 0; j < 5; j++) {
				if (temp->where[i][j] == 1)
					printf("{%d, %d}", i+1, j+1);
			}
		}
		puts("");
		temp = temp->next;
	}
}

void firstOrder () {
	puts("--------------------------------------------------------");
	FILE *file2;
	if ((file2 = fopen("higherorders/firstorders.txt", "w")) == NULL) { 
		puts("Could not open file!"); 
	}
	wordPtr temp, check;
	temp = firstWord;
	printf("1st-order term co-occurrence ");
	if (temp == NULL) {
		puts("List is empty!");
		return;
	}
	int times = 0;
	while (temp != NULL) {
		check = temp->next;
		while (check != NULL) {
			int i,j,k;
			for (i = 0, k = 0; i < 3 && k != 1; i++) {
				for(j = 0; j < 5; j++) {
					if (temp->where[i][j] == 1 && check->where[i][j] == 1) {
						fprintf(file2, "{%s,%s}", temp->word, check->word);
						printf("{%s,%s}", temp->word, check->word); //program print all first order co-occurrences to the screen.
						times++;
						k = 1;
						break;
					}
				}
			}
			check = check->next;
			
		}
		temp = temp->next;
	}
	fclose(file2);
	printf("\Total 1st Order : %d\n", times);
}

int insertSO (char *w1, char *w2) { //Inserts the given words to the second order link list if that combination does not exist in the link list.
	soPtr node = malloc(sizeof(sorder));
	node->sonext = NULL;
	strcpy(node->word1, w1);
	strcpy(node->word2, w2);
	if (soFirst == NULL) {
		soFirst = node;
		return 1;
	}
	soPtr temp = soFirst, prev = NULL; int i = 0;
	while (temp != NULL) { 
		if (!strcmp(w1, temp->word1) && !strcmp(w2, temp->word2)) {
			return 0;
		}
		prev = temp;
		temp = temp->sonext;
	}
	prev->sonext = node;
	return 1;
}

void secondOrderConnection () { //Finds the second order connections between txts.
	wordPtr word = NULL;
	word = firstWord;
	int i, j, k;
	puts("--------------------------------------------------------");
	while (word != NULL) {
		for (i = 0; i <= totalTxtFile; i++) 
			if (word->where[i/5][i%5] == 1)
				for (j = i + 1; j <= totalTxtFile; j++)
					if (word->where[j/5][j%5] == 1)
						connections[i][j] = 1;
		word = word->next;
	}	
	for (i = 0; i <= totalTxtFile; i++)
				for (j = i + 1; j <= totalTxtFile; j++)
					if (connections[i][j] == 1)
						printf("\n{%d, %d} -> {%d, %d}", i/5 + 1, i%5 + 1, j/5 + 1, j%5 + 1); //This line prints the txt numbers that connected with second order co-occurrence.
}

void secondOrder_ () { 
	FILE *file3; 
	if ((file3 = fopen("higherorders/secondorders.txt", "w")) == NULL) { 
		puts("Could not open file!"); 
	} 
	wordPtr word1, word2;
	word1 = firstWord;
	int i, j, k;
	int times = 0;
	puts("");
	while (word1 != NULL) { 
		for (i = 0; i < totalTxtFile ; i++) { 
			if (word1->where[i/5][i%5] == 1) { 
				for (j = 0; j < totalTxtFile; j++) {
					if (connections[i][j] == 1 && word1->where[j/5][j%5] != 1) { 
						word2 = word1->next; 
						while (word2 != NULL) { 
							int a, b;
							b = 0;
							for (a = 0; a < totalTxtFile; a++)
								if (word1->where[a/5][a%5] == 1 && word2->where[a/5][a%5] == 1){
									b = 1;
								}
							if (word2->where[j/5][j%5] == 1 && word2->where[i/5][i%5] != 1 && b == 0) {
								if (insertSO(word1->word, word2->word)) {
										fprintf(file3, "{%s,%s}", word1->word, word2->word);
										printf("{%s,%s}", word1->word, word2->word); //program print all second order co-occurrences to the screen.
										times++;
								}
							}
							word2 = word2->next;
						}
					}
				}
			word1->sodone = 1;		
			}
		}
		word1 = word1->next;	
	}
	fclose(file3);
	printf("\nTotal 2nd Orders : %d\n", times);				
				
}

void thirdOrderConnections () { //Finds third order connections between txts.
	puts("--------------------------------------------------------");
	int i, j, k;
	for (i = 0; i < totalTxtFile; i++) {
		for (j = 0; j < totalTxtFile; j++) {
			for (k = 0; k < totalTxtFile; k++) { 
				if (connections[i][j] == 1 && connections[j][k] == 1 && connections[i][k] != 1) {
					connectiont[i][k] = 1;
				}
			}
		}
	}
	for (i = 0; i <= totalTxtFile; i++)
				for (j = i + 1; j <= totalTxtFile; j++)
					if (connectiont[i][j] == 1)
						printf("\n{%d, %d} -> {%d, %d}", i/5 + 1, i%5 + 1, j/5 + 1, j%5 + 1);
}

void thirdOrder_ () {
	FILE *file4; 
	if ((file4 = fopen("higherorders/thirdorders.txt", "w")) == NULL) { 
		puts("Could not open file!"); 
	} 
	puts("");
	wordPtr word1, word2;
	word1 = firstWord;
	int i, j, k;
	int times = 0;
	
	while (word1 != NULL) {
		for (i = 0; i < totalTxtFile; i++) {
			if (word1->where[i/5][i%5] == 1) {
				for (j = 0; j < totalTxtFile; j++) {
					if (connectiont[i][j] == 1 && word1->where[j/5][j%5] != 1) {
						word2 = word1->next;
						while (word2 != NULL) {
							if (word2->where[j/5][j%5] == 1 && word2->where[i/5][i%5] != 1) {
								int a, b;
								b = 0;
								for (a = 0; a < totalTxtFile; a++) {
									if (connections[a][j] == 1)
										if (word1->where[a/5][a%5] == 1) {
											b = 1;
										}
									if (connections[i][a] == 1)
										if (word2->where[a/5][a%5] == 1) {
											b = 1;
										}
								} 
								if (b != 1) {
									fprintf(file4, "{%s,%s}", word1->word, word2->word);
									printf("{%s,%s}", word1->word, word2->word); //program print all third order co-occurrences to the screen.	
									times++;
								}
							}
							word2 = word2->next;
						}
					}
				}
			}
		}
		word1 = word1->next;	
	}
	fclose(file4);
	printf("\nTotal 3rd Order : %d", times);
}

void printList2 (wordPtr list, int i) { //For question b.
	wordPtr temp;
	temp = list;
	int k = 0;
	printf("Most frequent 5 words in folder %d\n", i + 1);
	if (temp == NULL) {
		puts("List is empty!");
		return;
	}
	
	while (temp != NULL && k < 5) {
		printf("%2d) %-15s; %6d", k+1, temp->word, temp->timesinfolder[i]);
		puts("");
		temp = temp->sortedNext;
		k++;
	}
}

void sort (int i) {	//Sorts the words descending by their tfs.
	
	wordPtr mll, temp, sll, prev;
	sortedFirst = NULL;	
	sortedFirst = firstWord;
	mll = firstWord->next;
	
	while (mll->next != NULL) { 
		int mlltimes = mll->timesinfolder[i], sftimes = sortedFirst->timesinfolder[i];
		if (mlltimes > sftimes) {
			temp = sortedFirst;
			sortedFirst = mll;
			sortedFirst->sortedNext = temp;
		}
		else { 
			sll = sortedFirst;
			int slltimes = sll->timesinfolder[i]; int a = 0;
			while (sll->sortedNext != NULL && slltimes >= mlltimes && a < 100) {
				prev = sll;
				sll = sll->sortedNext;
				slltimes = sll->timesinfolder[i]; a++;
			} 
			if (sll->sortedNext == NULL && slltimes >= mlltimes) { 
				sll->sortedNext = mll;
			}
			else if (slltimes <= mlltimes) {
				prev->sortedNext = mll;
				mll->sortedNext = sll; 
			}
			else if (slltimes > mlltimes) {
				prev->sortedNext = mll;
				mll->sortedNext = sll;
			}
		}
		mll = mll->next;
	}
	printList2(sortedFirst, i);
	return;
}

void printList3 (wordPtr list, int i) { //For question c.
	wordPtr temp;
	temp = list;
	int k = 0;
	printf("\nMost frequent 5 words in %d. folder (TF-IDF)\n", i+1);
	if (temp == NULL) {
		puts("List is empty!");
		return;
	}
	
	while (temp != NULL && k < 5) {
		printf("%2d) %-15s; %.2f", k+1, temp->word, temp->idf[i] / 100.00);
		puts("");
		temp = temp->sortedNext_2;
		k++;
	}
}

void sort2 (int i) { //Sorts the words descending by their tf-idfs.	 
	
	wordPtr mll, temp, sll, prev;
	sortedFirst_2 = NULL;	
	sortedFirst_2 = firstWord;
	mll = firstWord->next;
	
	while (mll->next != NULL) { 
		int mlltimes = mll->idf[i], sftimes = sortedFirst_2->idf[i];
		if (mlltimes > sftimes) {
			temp = sortedFirst_2;
			sortedFirst_2 = mll;
			sortedFirst_2->sortedNext_2 = temp;
		}
		else { 
			sll = sortedFirst_2;
			int slltimes = sll->idf[i]; int a = 0;
			while (sll->sortedNext_2 != NULL && slltimes >= mlltimes && a < 100) {
				prev = sll;
				sll = sll->sortedNext_2;
				slltimes = sll->idf[i]; a++;
			} 
			if (sll->sortedNext_2 == NULL && slltimes >= mlltimes) { 
				sll->sortedNext_2 = mll;
			}
			else if (slltimes <= mlltimes) {
				prev->sortedNext_2 = mll;
				mll->sortedNext_2 = sll;
			}
			else if (slltimes > mlltimes) {
				prev->sortedNext_2 = mll;
				mll->sortedNext_2 = sll;
			}
		}
		mll = mll->next;
	}
	printList3(sortedFirst_2, i);
	return;
}

int idfCalc () { //Calculates the tf-idf values of each word in the main link list.
	wordPtr temp;
	temp = firstWord;
	
	if (temp == NULL) {
		return;
	}
	int i,j;
	for (i = 0; i < 3; i++) {
		temp = firstWord;
		while (temp != NULL) {
			int smalln = 0;
			for (j = 0; j < 5; j++) {
				if (temp->where[i][j] == 1)
					smalln++;
			}
				if (smalln > 0) {
					temp->idf[i] = (int)(temp->timesinfolder[i] * log(((totalTxtFile * 10) / 10.0) / smalln) * 100);
				}
				else temp->idf[i] = 0;				
		temp = temp->next;
		}
		sort2(i);
	}
	 
	return 0;
}
