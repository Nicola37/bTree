/*
 * Nicola Frachesen
 * COP3530
 * Project 2
 */

 
//This is the .cpp file for a bTree.
 
//Including the .h file.
#include "bTree.h"

//Constructor
bTree::bTree(int n){
  //The size of the nodes, as given by the user, is set here.
  size = n;
  //Sees if the size is odd or even, and sets the minimum degree accordingly.
  if (size%2 == 0){
    minDegree = size/2;
  }
  //Else if the number is odd, find the floor.
  else{
    minDegree = (size-1)/2;
  }
  //Nothing has been added yet, so the root of the tree is null.
  root = NULL;
}

//Destructor
bTree::~bTree(){
  //Delete the root node.
  delete root;
}

//Inserts a given key-value pair.
void bTree::insert(string key, string value){
  //If the tree is empty.
  if (root == NULL){
    //Create the root.
    root = new bTreeNode(size, minDegree, true);
	//Insert key and value.
	root->keys[0] = key;
	root->values[0] = value;
	//The node now has one key-value pair.
	root->keySize = 1;
  }
  //If the tree is not empty.
  else{
    //If the current root is full.
	if (root->keySize == ((2*minDegree)-1)){
	  //Make a new root.
	  bTreeNode *x = new bTreeNode(size, minDegree, false);
	  //Make the old root a child.
	  x->children[0] = root;
	  //Split up the old root, and move a key to the new root for it to hold.
	  x->split(0, root);
	  
	  //The new root should now have two children.
	  //This if statement decides which child the new key should be placed into.
	  int i = 0;
	  if (x->keys[0] < key){
	    i++;
	  }
	  x->children[i]->insert(key, value);
	  
	  //Set x to be the new root.
	  root = x;
	}
	//Else, if the current root isn't full, insert.
	else{
	  root->insert(key, value);
	}
  }
} 

//Searches for the given key, 
//and has the given pointer point to the correct value if the key is found.
bool bTree::find(string key, string *value){
  //If the tree is empty, return false.
  if (root == NULL){
    return false;
  }
  //Else, go into the root, and start looking for the key.
  else{
    return root->find(key, value);
  }
}

//If the given key is in the bTree, then that key-value pair will be deleted, and true returned.
//If the given key is not found, then false is returned. 
bool bTree::delete_key(string key){
  //Create a bool which keeps track of whether or not something has been deleted.
  bool deleted = false;
  //If the tree is empty, return false.
  if (root == NULL){
    return deleted;
  }
  
  //Go through the tree looking for the key, and deleting it and its value if found.
  //If deletion occurs, then other actions might have to be taken.
  deleted = root->delete_key(key);
  
  //If deletion causes the root to be empty, then something has to be done.
  if (root->keySize == 0){
    //Temporarily store the old root.
	bTreeNode *temp = root;
	
	//If the root is also a leaf, then that means the tree is empty, so make it NULL.
	if (root->isLeaf){
	  root = NULL;
	}
	//Else, make the first child the new root.
	else{
	  root = root->children[0];
	}
	
    //Delete the temp root.
	delete temp;
  }
  
  return deleted;
}

string bTree::toStr(){
  //Create a stringstream to store the key strings.
  stringstream info;
  
  //If the bTree is empty, then return nothing other than a new line.
  if (root == NULL){
    return "";
  }
  
  info << root->traverse();
  
  return info.str();
}

//Node Constructor
bTreeNode::bTreeNode(int n, int m, bool l){
  //Maximum number of links.
  size = n;
  //Whether or not this node is a leaf.
  isLeaf = l;
  //Minimum degree.
  minDegree = m;
  
  //Creates the arrays to store the keys, values, 
  //and pointers to the children of this node.
  keys = new string[(2*minDegree)-1];
  values = new string[(2*minDegree)-1];
  children = new bTreeNode *[2*minDegree];
  
  //Initially, there are 0 keys in the node.
  keySize = 0;
}

//Node Destructor
bTreeNode::~bTreeNode(){
  delete keys;
  delete values;
  delete children;
}

string bTreeNode::traverse(){
  //Make a stringstream to be returned.
  stringstream info;
  //Make a for loop to go through the node.
  int i;
  for (i = 0; i < keySize; i++){
    //If this is not a leaf, then, before adding the key at i,
	//traverse the child node at i.
	if (isLeaf == false){
	  info << children[i]->traverse();
	}
	info << keys[i] << endl;
  }
  
  //Add the keys from the child at the last link if this node isn't a leaf.
  if (isLeaf == false){
    info << children[i]->traverse();
  }
  
  //Return the string.
  return info.str();
}

//Searches for a key in the subtree rooted at this node
bool bTreeNode::find(string key, string *value){
  //Find the first key which is either the key you are looking for or greater than it.
  int i = 0;
  while (i < keySize && key > keys[i]){
    i++;
  }
  
  //If the key at the current position is the one you're looking for,
  //then set the value pointer, and return true.
  if (i != keySize && !keys[i].compare(key)){
    *value = values[i];
	return true;
  }
  
  //Since the key was not found here, if this is a leaf node,
  //then the key isn't in the bTree.
  if (isLeaf == true){
    return false;
  }
  
  //Else if this isn't a leaf, then the key might still be in the tree,
  //so look in the next child.
  return children[i]->find(key, value);
}

void bTreeNode::insert(string key, string value){
  //This int will go though the node, starting with its greatest key (at index keySize-1).
  int i = keySize - 1;
  
  //If this node is a leaf...
  if (isLeaf == true){
    //This loop finds the location at which to put the key and value by starting
	//at the greatest key and continuing until the next key is
    // smaller than the key to be entered is.
	//The loop also moves other keys/values ahead to accommodate for the new key/value.
	while (i >= 0 && keys[i] > key){
	  keys[i+1] = keys [i];
	  values[i+1] = values [i];
	  i--;
	}
	//The new key/value are then entered.
	keys[i+1] = key;
	values[i+1] = value;
	//Number of keys in node increased.
	keySize++;
  }
  //Else if this node isn't a leaf...
  else{
    //The child where the new key has to go is found.
	while(i >= 0 && keys[i] > key){
	  i--;
	}
	//If the child node is full, then it has to be split.
	if(children[i+1]->keySize == ((2*minDegree) - 1)){
	  //Splits the child node.
	  split(i+1, children[i+1]);
	  //See which of the two nodes spilt from the original gets the new key.
	  if(keys[i+1] < key){
	    i++;
	  }
	}
	children[i+1]->insert(key, value);
  }
}

//Helper method for insert.
//Splits a child of a node into two. Here, x is the child of the node.
void bTreeNode::split(int i, bTreeNode *x){
  //Create a new node to hold the the appropriate amount of keys/values.
  bTreeNode *y = new bTreeNode(x->size, x->minDegree, x->isLeaf);
  y->keySize = (minDegree) - 1;
  
  //This for loop copies the last (minimum degree - 1) amount of keys/values of x to y.
  for (int j = 0; j < (minDegree - 1); j++){
    y->keys[j] = x->keys[j + (minDegree)];
	y->values[j] = x->values[j + (minDegree)];
  }
  //If x isn't a leaf, then the last minimum degree (-1) amount of children are also copied.
  if(x->isLeaf == false){
    for (int j = 0; j < (minDegree); j++){
	  y->children[j] = x->children[j+(minDegree)];
    }
  }
  x->keySize = (minDegree - 1);
  
  //Since this (the parent node) is getting a new child, space is made for it.
  for (int j = keySize; j >= (i + 1); j--){
    children[j+1] = children[j];
  }
  //Assign the new child at the link.
  children[i+1] = y;
	
  //x's last key/value will move to this node.
  //This for loop finds where to put it, and moves other keys/values appropriately.
  for (int j = (keySize-1); j >= i; j--){
    keys[j+1] = keys[j];
	values[j+1] = values[j];
  }
  //Places the currently last key/value of x into this spot.
  keys[i] = x->keys[minDegree-1];
  values[i] = x->values[minDegree-1];
  
  //The amount of keys/values in this node is incremented.
  keySize = keySize + 1;
}

//Removes a key in subtree rooted at this node.
bool bTreeNode::delete_key(string key){
  //This bool keeps track of whether or not the key has been deleted.
  bool deleted = false;
  
  int i = 0;
  //This while loop gets to the index of the first key greater than or equal to the given key.
  while (i < keySize && keys[i] < key){
    i++;
  }
  
  //If the key is in this node
  if(i < keySize && !keys[i].compare(key)){
    //If this node is a leaf
	if(isLeaf){
	  //All the keys/values after index position i are moved back one,
	  //deleting the key/value and moving the rest to the correct location.
	  for (int j = i+1; j < keySize; ++j){
	    keys[j-1] = keys[j];
		values[j-1] = values[j];
	  }
	  //The number of keys is thus reduced by 1
	  keySize--;
	}
	//Else if this isn't a leaf
	else{
      deleteNonLeaf(i);
	}
	//Then deleted will be true.
	deleted = true;
  }
  //Else if the key isn't in this node
  else{
    //If this is a leaf, and the key hasn't been found, that means it isn't in the tree.
    if(isLeaf){
	  return deleted;
	}
	
	//The key might be in the subtree at this node.
	//This bool checks if the key would be in the last child or not.
	bool isLast = false;
	if (i == keySize){
	  isLast = true;
	}
	
	//If the child has less than minimum degree amount of keys, then it is filled.
	if (children[i]->keySize < (minDegree)){
	  fill(i);
	}
	
	//If it is the last child which has merged,
	//then it must have done so with the previous child,
	if (isLast && i > keySize){
	  //so recursively delete in the previous child, and set bool deleted accordingly.
	  deleted = children[i-1]->delete_key(key);
	}
	//Else, recursively delete in the current child and set bool deleted accordingly.
	else{
	  deleted = children[i]->delete_key(key);
	}
  }
  return deleted;
}

//A helper method for delete_key that deletes a key/value
//from a node that of not a leaf.
void bTreeNode::deleteNonLeaf(int i){
  string key = keys[i];
  //If the child before the key (children[i]) has at least minimum degree amount of keys,
  if (children[i]->keySize >= minDegree){
    //find the key/value which precedes the given key in order,
	string* previous = getPrevious(i);
	//replace the given key/value with the previous one,
	keys[i] = previous[0];
	values[i] = previous[1];
	//and recursively delete the previous key in other places.
	children[i]->delete_key(previous[0]);
  }
  //Else if the child has less than minimum degree amount of keys,
  //but the next child (children[i+1]) has at least that many,
  else if (children[i+1]->keySize >= minDegree){
    //find the next key/value in order,
	string* next = getNext(i);
	//replace the given key/value with the next one,
	keys[i] = next[0];
	values[i] = next[1];
	//and recursively delete the next key in other places.
	children[i+1]->delete_key(next[0]);
  }
  //Else if both have less than minimum degree amount of keys,
  else{
    //merge children[i] and children[i+1],
	merge(i);
	//recursively delete the key.
	children[i]->delete_key(key);
  }
  return;
}

//A helper method for delete_key which finds the previous key in order,
//and returns the found key/value in a string array of size 2.
string* bTreeNode::getPrevious(int i){
  //Create a pointer to a node, starting with the child at the current index.
  bTreeNode *curNode = children[i];
  //Moves the pointer to the right until a leaf is reached.
  while (curNode->isLeaf == false){
    curNode = curNode->children[curNode->keySize];
  }
  
  //Create a string array to store the key/value pair.
  string* previous = new string[2];
  //Place the found key/value pair in the array, and return it.
  previous[0] = curNode->keys[curNode->keySize-1];
  previous[1] = curNode->values[curNode->keySize-1];
  return previous;
}

//A helper method for delete_key which finds the next key in order,
//and returns the found key/value in a string array of size 2.
string* bTreeNode::getNext(int i){
  //Create a pointer to a node, starting with the child at the current index + 1.
  bTreeNode *curNode = children[i + 1];
  //Moves the pointer to the left until a leaf is reached.
  while (curNode->isLeaf == false){
    curNode = curNode->children[0];
  }
  
  //Create a string array to store the key/value pair.
  string* next = new string[2];
  //Place the found key/value pair in the array, and return it.
  next[0] = curNode->keys[0];
  next[1] = curNode->values[0];
  return next;
}

//A helper method for delete_key that fills a child node if
//it has less than minimum degree minus 1 amount of keys.
void bTreeNode::fill(int i){
  //If the previous child has more than enough keys,
  if (i != 0 && children[i-1]->keySize >= minDegree){
    //take a key/value from there.
    takeFromPrevious(i);
  }
  //Else if the next child has more than enough keys,
  else if (i != keySize && children[i+1]->keySize >= minDegree){
    //take a key/value from there.
	takeFromNext(i);
  }
  //Else if they are both too small
  else{
    //If the given index is not last child,
	if (i != keySize){
	  //Merge with the next child.
	  merge(i);
	}
	else{
	  //Else merge with the previous child.
	  merge(i-1);
	}
  }
}

//A helper method for fill that takes a key/value from the previous child,
//and places a key/value in the current child.
void bTreeNode::takeFromPrevious(int i){
  //Create pointers to the child and the child before it.
  bTreeNode *child = children[i];
  bTreeNode *previousChild = children[i-1];
  
  //Move all the keys/values in the child forward by one
  //to accommodate the new key/value to be placed.
  for (int j = (child->keySize - 1); j >= 0; j--){
    child->keys[j+1] = child->keys[j];
	child->values[j+1] = child->values[j];
  }
  
  //If the child isn't a leaf, move all of its child pointers forward by one too.
  if (child->isLeaf == false){
    for (int j = child->keySize; j >= 0; j--){
	  child->children[j+1] = child->children[j];
	}
  }
  
  //The child's first key/value is set to the key/value at i-1 of the current node.
  child->keys[0] = keys[i-1];
  child->values[0] = values[i-1];

  //If the child isn't a leaf, then the last child of the previous child
  //becomes the first child of the current child.
  if (child->isLeaf == false){
    child->children[0] = previousChild->children[previousChild->keySize];
  }
  
  //Move the key/value from the previous child to the current node.
  keys[i-1] = previousChild->keys[previousChild->keySize - 1];
  values[i-1] = previousChild->values[previousChild->keySize - 1];
  
  //Increase the keySize of the child by one, and decrease that of the previous child.
  child->keySize++;
  previousChild->keySize--;
}

//A helper method for fill that takes a key/value from the next child,
//and places a key/value in the current child.
void bTreeNode::takeFromNext(int i){
  //Create pointers to the child and the child before it.
  bTreeNode *child = children[i];
  bTreeNode *nextChild = children[i+1];
 
  //The key/value at the index in the current node becomes
  //the last key/value of the child.
  child->keys[child->keySize] = keys[i];
  child->values[child->keySize] = values[i];
  
  //If the child isn't a leaf, the next child's first child becomes the last child of the child.
  if (child->isLeaf == false){
    child->children[(child->keySize) + 1] = nextChild->children[0];
  }
  
  //The first key/value of the next child is inserted into the current node at i.
  keys[i] = nextChild->keys[0];
  values[i] = nextChild->values[0];
  //Then move all the keys/values in the next child back by one to accommodate this.
  for (int j = 1; j < nextChild->keySize; j++){
    nextChild->keys[j-1] = nextChild->keys[j];
	nextChild->values[j-1] = nextChild->values[j];
  }
  
  //The child pointers are now also moved if the next child isn't a leaf.
  if (nextChild->isLeaf == false){
    for (int j = 1; j <= nextChild->keySize; j++){
	  nextChild->children[j-1] = nextChild->children[j];
	}
  }
  
  //Increase the keySize of the child by one, and decrease that of the previous child.
  child->keySize++;
  nextChild->keySize--;
}

//A helper method for fill that merges two children that are too small.
void bTreeNode::merge(int i){
  //Create pointers to the two children.
  bTreeNode *child = children[i];
  bTreeNode *nextChild = children[i+1];
  
  //Takes a key/value from the current node and puts it into
  //the minimum degree minus 1 position in the child.
  child->keys[minDegree-1] = keys[i];
  child->values[minDegree-1] = values[i];
  
  //Copies the keys/values from the next child to the child.
  for (int j = 0; j < nextChild->keySize; j++){
    child->keys[j + (minDegree)] = nextChild->keys[j];
	child->values[j + (minDegree)] = nextChild->values[j];
  }
  //If the child isn't a leaf, then its child pointers are also copied.
  if (child->isLeaf == false){
    for (int j = 0; j <= nextChild->keySize; j++){
	  child->children[j + (minDegree)] = nextChild->children[j];
	}
  }
  
  //Moves all keys/values after index i in the current node back one to fill
  //the gap from moving the key to the child.
  for (int j = (i + 1); j < keySize; j++){
    keys[j-1] = keys[j];
	values[j-1] = values[j];
  }
  
  //Moves the child pointers of this node after the child back one to accommodate for the merge.
  for (int j = (i+2); j <= keySize; j++){
    children[j - 1] = children[j]; 
  }
  
  //Change the keySize variable for the current node and child accordingly.
  child->keySize += (nextChild->keySize + 1);
  keySize--;
  
  //Delete the next child, as it has now been fully merged with child.
  delete nextChild;
}