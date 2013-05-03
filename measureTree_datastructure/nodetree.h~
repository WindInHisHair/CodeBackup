//Date: Oct 2011
//Author: Baiyu Xi
//ID:1094
//Descriptions: second homework for Advanced Data Structure
//
//
//
//tips: using "g++ *.cpp" to compile 
#ifndef NODE_TREE__
#define NODE_TREE__
#define BLOCKSIZE_IVL_NODE 200000
#include <stdio.h>
#include <stdlib.h>
struct interval_node
{
	int key;
	struct interval_node * left;
	struct interval_node * right;
	struct interval_node * next;
	int height;
};

typedef interval_node ivlNode;


ivlNode * getIvlNode();
void returnIvlNode(ivlNode * n); 
	
class interval_tree
{
	public:
		interval_tree();
		~interval_tree();
		int max();
		int min();
		int size(){return num;};
		//void clear(){num = 0; root ->left = NULL; root ->right = NULL;};
		void clear();
		ivlNode* remove(int index);
		void add(int index);
		ivlNode* find(int index);
		void printTree();
	private:
		int num;
		ivlNode * root;
		void reBalance(ivlNode ** stack, int sindex);

		void left_rotate(ivlNode * node);
		void right_rotate(ivlNode *node);

};


#endif
