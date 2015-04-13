/*
 * Nicola Frachesen
 * COP3530
 * Project 2
 */
 
//This is the .h file for a bTree.
 
//All the necessary libraries are included.
#include <iostream>
#include <sstream>
using namespace std;

//A class which is used to create node objects for the master bTree class.
class bTreeNode{
  //Methods
  public:
    //Constructor
	bTreeNode(int n, int m, bool l);
    //Destructor
    ~bTreeNode();
	//Traverses the nodes, returning stringstreams for the toStr method.`
	string traverse();
	//Searches in subtree rooted at this node.
	bool find(string key, string *value);
	//Inserts into the node.
	void insert(string key, string value);
	//Splits the given child node at the child node's given index.
	void split(int i, bTreeNode *x);
	//Removes a key/value in subtree rooted at this node.
	bool delete_key(string key);
	//Deletes a key/value from a node that is not a leaf.
	void deleteNonLeaf(int i);
	//Finds the previous key in order compared to the current key at index i.
	//Returns an array of two strings: the found key and value.
	string* getPrevious(int i);
	//Finds the next key in order compared to the current key at index i.
	//Returns an array of two strings: the found key and value.
	string* getNext(int i);
	//Fills a child node at index i which does not have enough keys.
	void fill(int i);
	//Takes a key/value from the previous node (i-1) and places it
	//a key/value in the following node (i).
	void takeFromPrevious(int i);
	//Takes a key/value from the following node (i+1) and places it
	//a key/value in the node (i).
	void takeFromNext(int i);
	//Merges two children of the node at index i and i+1.
	void merge(int i);
	
	//This allows the node to access private variables from the bTree class.
	friend class bTree;
	
  //Variables
  private:
    //The maximum size (number of links) that a node can have.
	int size;
	//The minimum degree of the tree.
    int minDegree;	
	//The current number of keys in the node.
	int keySize;
    //The array that holds the keys
	string *keys;
	//The array that holds the values.
	string *values;
	//The array of pointers to the children of this node.
	bTreeNode **children;
    //Boolean which is true when this node is a leaf.
    bool isLeaf;	
};
 
//The bTree class.
class bTree{
  //Methods
  public:
    //Constructor
    bTree(int n);
	//Destructor
	~bTree();
	//Insertion into the bTree.
	void insert(string key, string value);
	//Searches for a key in the bTree.
	bool find(string key, string* value);
	//Deletion of an element in the bTree.
	bool delete_key(string key);
	//Returns a string of every key in the bTree in order. 
	string toStr();

  private:
    //The maximum size (number of links) of the nodes.
    int size;
	//The minimum degree of the tree.
	int minDegree;
	//A pointer to the root node.
	bTreeNode *root;
};
 