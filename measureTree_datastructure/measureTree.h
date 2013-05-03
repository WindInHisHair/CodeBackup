//
//
//
//tips: using "g++ *.cpp" to compile 
#ifndef MEASURE_TREE_HEAD__
#define MEASURE_TREE_HEAD__
#define INIF 2147483647
#define MINIF -2147483648
#include <stdio.h>

#include <set>
#include <algorithm>

#include "nodetree.h"
#define BLOCKSIZE 1024000

using namespace std;
struct Node {


	int key;
	int nodeIvl_l;
	int nodeIvl_r;

	interval_tree ep_l;
	interval_tree ep_r;
	int leftmin;
	int rightmax;

	int height;
	int measure;

	int time;
	struct Node * left;
	struct Node * right;
	struct Node * transfer;
	struct Node * next;

};

typedef Node node;

class m_tree_t
{
	public:
		m_tree_t();
		~m_tree_t();
		int getLen();
		void add(int a, int b);
		void del(int a, int b);
		node * find(int index);
	private:

		node * root;
		int num;

		//member and functions for the apply for spaces
		int szNode;
		node* currentNode;
		node * headNode;
		node* freeNode;
		void returnNode(node *n);
		node * getNode();

		node* removeNode(int index, int otherIndex);
		int insertNode(int index, int otherIndex);

		void leftR(node * n);
		void rightR(node * n);


		//the function for calcualte the measure
		int getMeasure(node *n);
		int getRightMax(node *n);
		int getLeftMin(node *n);

		//function for debug
	public:
		void printTree();

};



//interface for Homework requirements
m_tree_t *create_m_tree();

void insert_interval(m_tree_t * tree, int a, int b);
void delete_interval(m_tree_t * tree,  int a, int b);

int query_length(m_tree_t *tree);

#endif
