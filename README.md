# AVL Dictionary

This project is an implementation of an AVL Tree Dictionary in C. The AVL Tree is a self-balancing binary search tree that has a guaranteed logarithmic performance for each operation. In this implementation, each node in the AVL Tree holds two types of information: a key (integer) and a value (string).

# Table of Contents
1. [About the project](#start-description)
2. [Functions](#func-description)
3. [Building the Project](#build-description)
4. [Conclusion](#use-description)

<a name="start-description"></a>
## About the project

The project offers various functions that allow users to manipulate the AVL tree data structure, such as creating a new AVL tree, checking if the AVL tree is empty, searching for a node with a given key, finding the minimum and maximum elements in the tree, and inserting a new node with a given key and value. Additionally, the project includes functions for rotating nodes left or right, computing the balance factor of a node, and finding the successor and predecessor of a node.

The AVL Dictionary project is implemented in C and offers a command-line interface for users to interact with the AVL tree. Users can create a new AVL tree, insert key-value pairs, search for values, and print the tree in a sorted order. The code is modular and easy to understand, with each function implementing a specific aspect of the AVL tree algorithm.

<a name="func-description"></a>
## Functions
This AVL Tree Dictionary includes the following functions:

- **createTree** - allocates memory for the AVL Tree and initializes its size to 0 and root to NULL.
- **isEmpty** - returns 1 if the AVL Tree is empty, and 0 otherwise.
- **search** - searches for a given key in the AVL Tree and returns the corresponding node.
- **minimum** - finds the node with the minimum key in the AVL Tree and returns it.
- **maximum** - finds the node with the maximum key in the AVL Tree and returns it.
- **successor** - finds the node with the next larger key in the AVL Tree and returns it.
- **predecessor** - finds the node with the next smaller key in the AVL Tree and returns it.
- **avlRotateLeft** - performs a left rotation on a given node in the AVL Tree to maintain balance.
- **avlRotateRight** - performs a right rotation on a given node in the AVL Tree to maintain balance.
- **avlGetBalance** - returns the balance factor of a given node in the AVL Tree.
- **insert** - inserts a new node with the given key and value into the AVL Tree.

<a name="build-description"></a>
## Building the Project

In order to build the project make sure that you are on a LINUX machine or WSL, the enter the build folder and to run the Makefile*:

```BASH 
    cd build
    make
```
In order to see how to work with project functions, I suggest to look up to avl_dict_run.c file. This file is a collection of tests to check every function, especially corener cases, like NULLs statements.

<a name="use-description"></a>
## Conclusion

This AVL Dictionary implementation in C provides an efficient way to store and search for key-value pairs. With its guaranteed logarithmic performance for each operation, it is suitable for large-scale data applications.
 