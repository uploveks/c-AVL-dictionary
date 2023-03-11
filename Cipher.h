#ifndef CIPHER_H_
#define CIPHER_H_

#include "TreeMap.h"

/* Maximum length of teh buffer */
#define BUFLEN 1024

/* Length of an element in the tree */
#define ELEMENT_TREE_LENGTH 5

/* Range of offsets */
typedef struct Range{
	int *index;
	int size;  		// The current number of indexes in the range
	int capacity;	/* The maximum capacity of the range
					 * if size == capacity, additional memory will be allocated
                     * (capacity will be doubled)
                     */
}Range;

void buildTreeFromFile(char* fileName, TTree* tree);

void encrypt(char *inputFile, char *outputFile, Range *key);
void decrypt(char *inputFile, char *outputFile, Range *key);

void printKey(char *fileName, Range *key);
Range* inorderKeyQuery(TTree* tree);
Range* levelKeyQuery(TTree* tree);
Range* rangeKeyQuery(TTree* tree, char* q, char* p);


#endif /* CIPHER_H_ */