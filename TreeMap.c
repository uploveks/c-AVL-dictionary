#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TreeMap.h"

#define MAX(a, b) (((a) >= (b))?(a):(b))


/* Create a tree with a series of associated methods
 *
 * return: the created tree
 */
TTree* createTree(void* (*createElement)(void*),
				  void (*destroyElement)(void*),
				  void* (*createInfo)(void*),
				  void (*destroyInfo)(void*),
				  int compare(void*, void*)) {
	TTree *tree = (TTree *)malloc(sizeof(TTree));
	tree->createElement = createElement;
	tree->destroyElement = destroyElement;
	tree->createInfo = createInfo;
	tree->destroyInfo = destroyInfo;
	tree->compare = compare;
	tree->size = 0;
	tree->root = NULL;
	return tree;
}


/* Check if a tree is empty
 * 1 - if the tree is empty
 * 0 - otherwise
 */
int isEmpty(TTree* tree) {
	return tree->root == NULL;
}


/* Search for a specific element inside a tree
 *
 * tree: the structure with the methods associated with the tree
 * !used to be able to compare the current element with the searched one
 *
 * x: the root of the current tree (in which the search is made)
 * elem: the element to be searched for
 */
TreeNode* search(TTree* tree, TreeNode* x, void* elem) {
	if (x == NULL)
		return NULL;
	else if (tree->compare(x->elem, elem) == 0) 
		return x;
	else if (tree->compare(x->elem, elem) == 1) {
		return search(tree, x->left, elem);
	} else {
		return search(tree, x->right, elem);
	}
}



/* Find the node with the minimum element in a tree
 * having the root in x
 */
TreeNode* minimum(TreeNode* x) {
	TreeNode *temp = x;
	while (temp->left != NULL)
		temp = temp->left;
	return temp;
}

/* Finds the node with the maximum element in a tree
 * having the root in x
 */
TreeNode* maximum(TreeNode* x) {
	TreeNode *temp = x;
	while(temp->right != NULL)
		temp = temp->right;
	return temp;
}



/* Function to find the successor of a node
 * (successor out of order)
 */
TreeNode* successor(TreeNode* x) {
	//return NULL;
	if (x == NULL) 
		return NULL;
	if (x->right != NULL) {
		return minimum(x->right);
	} else {
		TreeNode *current = x;
		TreeNode *parent_curr = current->parent;
		while (parent_curr != NULL && current != parent_curr->left) {
			current = parent_curr;
			parent_curr = parent_curr->parent;
		}
		return parent_curr;
	}
	return NULL;
}



/* Function to find the predecessor of a node
 * (predecessor out of order)
 */
TreeNode* predecessor(TreeNode* x) {
	if (x == NULL) {
		return NULL;
	}
	if (x->left != NULL) {
		return maximum(x->left);
	} else {
		TreeNode *current = x;
		TreeNode *parent_curr = current->parent;
		while (parent_curr != NULL && current != parent_curr->right) {
			current = parent_curr;
			parent_curr = parent_curr->parent;
		}
		return parent_curr;
	}
	return NULL;
}



/* Updates the height of a node in the tree
 */
void updateHeight(TreeNode* x) {

	int leftHeight = 0;
	int rightHeight = 0;

	if (x != NULL) {
		if (x->left != NULL)  leftHeight  = x->left->height;
		if (x->right != NULL) rightHeight = x->right->height;
		x->height = MAX(leftHeight, rightHeight) + 1;
	}
}


/* Function that receives the address of a tree and
 * of a node x and performs a rotation to the left
 * of the subtree whose vertex is x
 *
 *   (x)	 Rotatie    (y)
 *	 / \     stanga     / \
 *  a  (y)    ---->   (x)  c
 *	   / \			  / \
 *    b   c          a   b
 */
void avlRotateLeft(TTree* tree, TreeNode* x) {
	if (tree == NULL || x == NULL) return;
	TreeNode *y = x->right;
	TreeNode *b = y->left;
	if (y->left != NULL) {
		b->parent = x;
		x->right = b;
	} else {
		x->right = NULL;
	}
	y->left = x;
	if (x->parent == NULL) {
		y->parent = NULL;
		tree->root = y;		
	} else if (x->parent->right == x) {
		x->parent->right = y;
		y->parent = x->parent;
	} else if (x->parent->left == x) {
		x->parent->left = y;
		y->parent = x->parent;
	}
	x->parent = y;
	updateHeight(x);
	updateHeight(y);
}



/* Function that receives the address of a tree and
 * of a node x and performs a rotation to the right
 * of the subtree whose vertex is in y
 *
 *     (y)	 Rotatie    (x)
 *	   / \   dreapta    / \
 *   (x)  c   ---->    a  (y)
 *	 / \			      / \
 *  a   b                b   c
 */
void avlRotateRight(TTree* tree, TreeNode* y) {
	if (tree == NULL || y == NULL) return;
	TreeNode *x = y->left;
	TreeNode *b = x->right;
	if (x->right != NULL){
		x->right->parent = y;
		y->left = b;
	} else 
		y->left = NULL;
	x->right = y; 
	if (y->parent == NULL) {
		x->parent = NULL;
		tree->root = x;
	} else if (y->parent->right == y){
		y->parent->right = x;
		x->parent = y->parent;
	} else if (y->parent->left == y) {
		y->parent->left = x;
		x->parent = y->parent;
	}
	y->parent = x;
	updateHeight(x);
	updateHeight(y);
}



/* Calculate the balancing factor for a node x
 * (AVL balance factor)
*/
int avlGetBalance(TreeNode *x) {
	if (x == NULL) {
		return 0;
	} else if (x->left == NULL && x->right == NULL) {
		return 0;
	} else if (x->right == NULL) {
		return x->left->height;
	} else if (x->left == NULL) {
		return 0 - x->right->height;
	} else {
	return x->left->height - x->right->height;
	}
}


/* Function for rebalancing an AVL tree
 * after an insertion through single or double rotations
 * and the recalculation of the height of each node encountered while traveling
 * the tree from the bottom up, towards the root
 *
 */
void avlFixUp(TTree* tree, TreeNode* y) {
	while (y != NULL) {
		updateHeight(y);
		int balance = avlGetBalance(y);
		int balance_right = avlGetBalance(y->right);
		int balance_left = avlGetBalance(y->left);
		if (balance > 1 && balance_left == 1) 
			avlRotateRight(tree, y);
		else if (balance < -1 && balance_right == -1)
			avlRotateLeft(tree, y);
		else if (balance > 1 && balance_left == -1) {
			avlRotateLeft(tree, y->left);
			avlRotateRight(tree, y);
		} else if (balance < -1 && balance_right == 1) {
			avlRotateRight(tree, y->right);
			avlRotateLeft(tree, y);
		}
		y = y->parent;
	}
}



/* Function to create a node
 *
 * value: the value/key in the tree
 * info: the information/value from the dictionary
 */
TreeNode* createTreeNode(TTree *tree, void* value, void* info) {

	if (tree == NULL)
		return NULL;

	// Alocate memory
	TreeNode* node = (TreeNode*) malloc(sizeof(TreeNode));

	// Set element and info
	node->elem = tree->createElement(value);
	node->info = tree->createInfo(info);


	//Initialize the links in the tree
	node->parent = node->right = node->left = NULL;


	//initialize the links in the double linked list
	node->next = node->prev = node->end = NULL;


	// Height of new node is 1
    // Height of NULL is 0
	node->height = 1;

	return node;
}

/* Inserting a new node in the multi-dictionary
 * ! After the addition, the tree must be balanced
 *
 */
void insert(TTree* tree, void* elem, void* info) {
	TreeNode *x = tree->root;
	TreeNode *y = NULL;
	TreeNode *newNode = createTreeNode(tree, elem, info);
	while (x != NULL) {
		y = x;
		if (tree->compare(elem, x->elem) == -1) 
			x = x->left;
		else if (tree->compare(elem, x->elem) == 1)
			x = x->right;
		else 
			break;
	}
	tree->size++;
	if (tree->root == NULL) {
		tree->root = newNode;
		newNode->end = newNode;
	} else {
		TreeNode *duplicate_node = search(tree, tree->root, elem);
		if (tree->compare(elem, y->elem) == 1) {
			newNode->parent = y;
			y->right = newNode;
			newNode->prev = y->end;
			newNode->next = y->end->next;
			y->end->next = newNode;
			newNode->end = newNode;
			if (newNode->next != NULL)
				newNode->next->prev = newNode;
		} else if (tree->compare(elem, y->elem) == -1) {
			newNode->parent = y;
			y->left = newNode;
			newNode->next = y;
			newNode->prev = y->prev;
			y->prev = newNode;
			newNode->end = newNode;
			if (newNode->prev != NULL)
				newNode->prev->next = newNode;
		} else if (duplicate_node != NULL) {
			TreeNode *current_end = duplicate_node->end;
			newNode->prev = current_end;
			newNode->next = current_end->next;
			current_end->next = newNode;
			if (newNode->next != NULL)
				newNode->next->prev = newNode;
			duplicate_node->end = newNode;
		}
	}
	avlFixUp(tree, y);
}


/* Remove a node from a tree
 *
 * ! tree must be used for release
 * the `elem' and `info' fields
 * */
void destroyTreeNode(TTree *tree, TreeNode* node){

	// check if the tree or the node is NULL
	if(tree == NULL || node == NULL) return;


    // Using tree methods
    // to deallocate node fields
	tree->destroyElement(node->elem);
	tree->destroyInfo(node->info);

	// free memory
	free(node);
}

void avlDeleteFixUp(TTree* tree, TreeNode* y) {
	while (y != NULL) { 
		updateHeight(y);
		int balance = avlGetBalance(y);
		int balance_right = avlGetBalance(y->right);
		int balance_left = avlGetBalance(y->left);
		if (balance > 1 && balance_left >= 0) 
			avlRotateRight(tree, y);
		else if (balance < -1 && balance_right <= 0)
			avlRotateLeft(tree, y);
		else if (balance > 1 && balance_left < 0) {
			avlRotateLeft(tree, y->left);
			avlRotateRight(tree, y);
		} else if (balance < -1 && balance_right > 0) {
			avlRotateRight(tree, y->right);
			avlRotateLeft(tree, y);
		}
		y = y->parent;
	}
}

/* Remove a node from the tree
 *
 * elem: the key of the node to be deleted
 * ! If there are duplicate keys
 * the last node in the list of duplicates will be deleted
 */
void delete(TTree* tree, void* elem) {
	if (tree == NULL || tree->root == NULL) 
		return;
	if (tree->root->left == NULL && tree->root->right == NULL) {
		destroyTreeNode(tree, tree->root);
		tree->root = NULL;
		tree->size--;
		return;
	}
	TreeNode *current = search(tree, tree->root, elem), *prev_curr = NULL;
	if (current)
		prev_curr = current->parent;
	else
		return;
	if (current->next != NULL && tree->compare(current->elem, current->next->elem) == 0) {
		TreeNode *current_end = current->end;
		if (current_end->next != NULL) 
			current_end->next->prev = current_end->prev;
		current_end->prev->next = current_end->next;
		current->end = current_end->prev;
		destroyTreeNode(tree, current_end);
	} else if (current->left == NULL && current->right == NULL) {
		if (current == prev_curr->left) {
			prev_curr->left = NULL;
			prev_curr->prev = current->prev;
			if (current->prev != NULL) 
				current->prev->next = prev_curr;
		} else if (prev_curr->right == current) {
			prev_curr->right  = NULL;
			prev_curr->next = current->next;
			if (current->next != NULL) 
				current->next->prev = prev_curr;
		}
		destroyTreeNode(tree, current);
	} else if (current->left == NULL && current->right != NULL) {
		TreeNode *new_current = NULL;
		new_current = current->right;
		new_current->parent = prev_curr;
		new_current->prev = current->prev;
		if (current->prev != NULL) 
			current->prev->next = new_current;
		if (prev_curr == NULL)
			tree->root = new_current;
		if (current == prev_curr->left)
			prev_curr->left = new_current;
		else
			prev_curr->right = new_current;
		destroyTreeNode(tree, current);
	} else if (current->left != NULL && current->right == NULL) {
		TreeNode *new_current = NULL;
		new_current = current->left;
		new_current->parent = prev_curr;
		new_current->end->next = current->next;
		if (current->next != NULL)
			current->next->prev = new_current->end;
		if (prev_curr == NULL)
			tree->root = new_current;
		if (current == prev_curr->left)
			prev_curr->left = new_current;
		else
			prev_curr->right = new_current;
		destroyTreeNode(tree, current);
	} else {
		TreeNode *succ = successor(current);
		TreeNode *prev_succ = succ->parent;
		if (succ == prev_succ->right)
			prev_succ->right = succ->right;
		else 
			prev_succ->left = succ->left;
		if (current == tree->root)
			tree->root = succ;
		if (current->left != NULL)
			current->left->parent = succ;
		if (current->right != NULL)
			current->right->parent = succ;
		succ->parent = current->parent;
		succ->left = current->left;
		succ->right = current->right;
		if (current->prev != NULL)
			current->prev->next = succ;
		succ->prev = current->prev;
		destroyTreeNode(tree, current);
	}
	tree->size--;
	if (prev_curr != NULL)
		avlDeleteFixUp(tree, prev_curr);
}


/* Free the memory allocated for the tree
 */
void destroyTree(TTree* tree){


    /* Doubly linked list can be used
    * to release memory
    */
	if (tree == NULL || tree->root == NULL)
		return;
	TreeNode *node = minimum(tree->root);
	while(node != NULL) {	
		TreeNode *temp = node;
		node = temp->next;
		if(temp)
			destroyTreeNode(tree, temp);
	}
	if (tree) 
		free(tree);
	return;
}