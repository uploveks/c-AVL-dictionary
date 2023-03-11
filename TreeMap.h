#ifndef TREEMAP_H_
#define TREEMAP_H_

#include <stdlib.h>

/*
 * A node in the tree
 */
typedef struct node{
	void* elem; 			// element/key of the node
	void* info;				// information of the node
	struct node *parent;	// parent of the node
	struct node *left;  	// left child
	struct node *right; 	// right child
	struct node* next; 		// pointer to the next node in the list of duplicates
	struct node* prev; 		// pointer to the previous node in the list of duplicates
	struct node* end; 		// pointer to the end of the list of duplicates for
                            // current node
	long height;			// the height of the node in the tree
}TreeNode;

/*
 * Representation of a multi-dictionary
 */
typedef struct TTree{
	TreeNode *root;					// root of the tree
	void* (*createElement)(void*);  // method for creating an element
	void (*destroyElement)(void*);	// method for destroying an element
	void* (*createInfo)(void*); 	// method for creating information
	void (*destroyInfo)(void*); 	// method for deleting information
	int (*compare)(void*, void*); 	// method for comparing two elements
	long size;						// numebr of nodes in the tree
}TTree;


TTree* createTree(void* (*createElement)(void*),
				  void (*destroyElement)(void*),
				  void* (*createInfo)(void*),
				  void (*destroyInfo)(void*),
				  int compare(void*, void*));

int isEmpty(TTree* tree);
TreeNode* search(TTree* tree, TreeNode* x, void* elem);
TreeNode* minimum(TreeNode* x);
TreeNode* maximum(TreeNode* x);
TreeNode* successor(TreeNode* x);
TreeNode* predecessor(TreeNode* x);
void updateHeight(TreeNode* x);
void avlRotateLeft(TTree* tree, TreeNode* x);
void avlRotateRight(TTree* tree, TreeNode* y);
int avlGetBalance(TreeNode *x);
void avlFixUp(TTree* tree, TreeNode* y);
TreeNode* createTreeNode(TTree *tree, void* value, void* info);
void destroyTreeNode(TTree *tree, TreeNode* node);
void insert(TTree* tree, void* elem, void* info);
void delete(TTree* tree, void* elem);
void destroyTree(TTree* tree);
void printList(TTree *tree);

#endif /* TREEMAP_H_ */