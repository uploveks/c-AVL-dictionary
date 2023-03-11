#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>

#include "Cipher.h"

void* createStrElement(void* str);

void destroyStrElement(void* elem);

void* createIndexInfo(void* index);

void destroyIndexInfo(void* index);

/* Build a multi-dictionary based on a text file
 * The key (element) of a node will be represented by a word from the text
 * and the value (info) will be the beginning index of that word
 * ignoring separator characters (",.? \n\r") - i.e. NUMBER
 * of preceding A-Z characters
 *
 * E.g: THIS IS AN EXAMPLE
 *
 * 			IS (4)
 *		    / \
 * 	  (6) AN  THIS (0)
 *         \
 *	 	 EXAMPLE (8)
 * */
void buildTreeFromFile(char* fileName, TTree* tree) {

	//Check arguments
	if(fileName == NULL || tree == NULL)
		return;
	char *buffer = malloc(sizeof(char) * BUFLEN);
	FILE *file = fopen(fileName, "r");
	if (file == NULL)
		return;
	int idx = 0;
	while (fgets(buffer, BUFLEN, file)) {
		char *token = strtok(buffer, " ,.?!\n");
		while (token) {
			char *element = (char *)createStrElement(token);
			int *info = createIndexInfo(&idx);
			insert(tree, element, info);
			idx += strlen(token);
			token = strtok(NULL, " ,.?!\n\r");
			destroyStrElement(element);
			destroyIndexInfo(info);
		}
	}
	fclose(file);
	free(buffer);
}


/* Function to display an encryption key
 * A key is represented by a series of offsets
 *
 * E.g: key = [1, 2, 3, 4]
 * input text  = A    A    A    A
 *			   +1|  +2|  +3|  +4|
 * 				 V    V    V    V
 * output text = B    C    D    E
 *
 */
void printKey(char *fileName, Range *key) {

	FILE *f = fopen(fileName, "w");

	if (key == NULL) {
		fprintf(f, "No key provided!\n");
		fclose(f);
		return;
	}

	fprintf(f, "Decryption key of length %d is:\n", key->size);

	// Display each offset in the key % 26 (length
    // alphabet A-Z)
	for (int i = 0; i < key->size; i++) {
		fprintf(f, "%d ", key->index[i] % 26);


		if ((i + 1) % 10 == 0)
			fprintf(f, "\n");
	}

	fclose(f);
}


/* Returns the key obtained by traversing in order
 * increasing the nodes of the tree (also going through the lists
 * of duplicates)
 */
Range* inorderKeyQuery(TTree* tree) {
	if (tree == NULL || tree->root == NULL)
		return NULL;
	Range *key_query = malloc(sizeof(Range));
	key_query->capacity = tree->size;
	key_query->index = malloc(key_query->capacity * sizeof(int));
	key_query->size = 0;
	TreeNode *min_node = minimum(tree->root);
	for (int i = 0; i < key_query->capacity; i++) {
		key_query->size++;
		key_query->index[i] = *(int*)min_node->info;
		min_node = min_node->next;
	}
	return key_query;
}


/* Function for extracting the key formed from the values
 * nodes from the level containing the most frequent word
 * (if there are more words with a maximum number
 * of occurrences then the first node among them will be considered compliant
 * traversing the tree out of order)
 */
Range* levelKeyQuery(TTree* tree) {
	if (tree == NULL || tree->root == NULL)
		return NULL;
	Range *key_query = malloc(sizeof(Range));
	key_query->size = 0;
	key_query->capacity = tree->size;
	key_query->index = malloc(key_query->capacity * sizeof(int));
	TreeNode *y = minimum(tree->root);
	TreeNode *max_freq = NULL, *curr = y, *curr_n = y;
	int max = 0, nr_duplicates = 1;
	while (curr_n) {
		if (tree->compare(curr->elem, curr_n->elem) == 0) {
			nr_duplicates++;
		} else {
			if (nr_duplicates > max) {
				max = nr_duplicates;
				max_freq = curr;
			}
			nr_duplicates = 1;
			curr = curr_n;
		}
		curr_n = curr_n->next;
	}
	if (nr_duplicates > max) {
			max = nr_duplicates;
			max_freq = curr;
	}
	int level_max_freq = 0;
	TreeNode *temp_freq = max_freq;
	while (temp_freq) {
		level_max_freq++;
		temp_freq = temp_freq->parent;
	}
	
	y = minimum(tree->root);
	TreeNode *temp_y = y;
	int level_y = 0;
	while(y) {
		temp_y = y;
		level_y = 0;
		while (temp_y != NULL) {
			level_y++;
			temp_y = temp_y->parent;
		}
		if (level_max_freq == level_y) {
			while (y->next && tree->compare(y->elem, y->next->elem) == 0) {
				if (key_query->capacity == key_query->size) {
					key_query->index = realloc(key_query->index, 2 * sizeof(int) * key_query->capacity);
					
					if (key_query->index)
						key_query->capacity = key_query->capacity * 2;
					else
						return NULL;
					}
				key_query->index[key_query->size] = *(int*)y->info;
				key_query->size++;
				y = y->next;
			}
			key_query->index[key_query->size] = *(int*)y->info;
			key_query->size++;
			y = y->next;
		} else {
			y = y->end->next;
		}
	}
	return key_query;
}



/* Extract the key from the nodes located in a certain
 * specified range of values
 */
Range* rangeKeyQuery(TTree* tree, char* q, char* p) {
	if (tree == NULL || tree->root == NULL)
		return NULL;
	Range *key_query = malloc(sizeof(Range));
	key_query->size = 0;
	key_query->capacity = tree->size;
	key_query->index = malloc(sizeof(int) * key_query->capacity);
	TreeNode *y = minimum(tree->root);
	while (y) {
		if (tree->compare(y->elem, q) == 1 && tree->compare(y->elem, p) == -1) {
			if (key_query->capacity == key_query->size) {
				key_query->index = realloc(key_query->index, 2 * sizeof(int) * key_query->capacity);
				if (key_query->index)
					key_query->capacity = key_query->capacity * 2;
				else
					return NULL;
			}
		key_query->index[key_query->size] = *(int*)y->info;
		key_query->size++;
		}
		y = y->next;
	}
	return key_query;
}


void encrypt(char *inputFile, char *outputFile, Range *key) {

	FILE * f_in  = fopen(inputFile,  "r");
	FILE * f_out = fopen(outputFile, "w");

	if (f_in == NULL)
		return;

	char *buff = (char*) malloc(BUFLEN+1);
	char c;

	int idx = 0;

	while (fgets(buff, BUFLEN, f_in) != NULL) {

		for (int i = 0; i < strlen(buff); i++) {
			if (buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\r') {
				c = ((toupper(buff[i]) - 'A') + key->index[idx] % 26) % 26 + 'A';
				idx += 1;
			} else
			 	c = buff[i];

			fprintf(f_out, "%c", c);

			if (idx == key->size)
				idx = 0;
		}
	}

	free(buff);
	fclose(f_in);
	fclose(f_out);
}


void decrypt(char *inputFile, char *outputFile, Range *key) {
 	
	FILE * f_in  = fopen(inputFile,  "r");
	FILE * f_out = fopen(outputFile, "w");

	if (f_in == NULL)
		return;

	char *buff = (char*) malloc(BUFLEN+1);
	char c;

	int idx = 0;

	while (fgets(buff, BUFLEN, f_in) != NULL) {

		for (int i = 0; i < strlen(buff); i++) {
			if (buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\r') {
				c = ((toupper(buff[i]) - 'A') - (key->index[idx] % 26) + 26) % 26 + 'A';
				idx += 1;
			} else
			 	c = buff[i];

			fprintf(f_out, "%c", c);

			if (idx == key->size)
				idx = 0;
		}
	}

	free(buff);
	fclose(f_in);
	fclose(f_out);
}