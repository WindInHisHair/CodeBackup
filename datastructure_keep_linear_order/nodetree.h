#ifndef NODE_TREE__
#define NODE_TREE__
#define BLOCKSIZE_NODE 3000000
#include <stdio.h>
#include <stdlib.h>
struct Node
{
	struct Node * up;
	struct Node * left;
	struct Node * right;
	struct Node * next;
	struct Node * lnext;
	struct Node * lprev;
	long key;
	int height;
};

typedef  struct Node node;
	
class key_tree
{
	public:
		key_tree();
		~key_tree();
		node* remove(long index);
		void add(long index, node * pNNode);
		node* find(long index);
	private:
		int num;
		node * root;
		void reBalance(node ** stack, int sindex);
		node * getNode();
		void returnNode(node * n);

		void left_rotate(node * node);
		void right_rotate(node *node);

		node * currentNodeHead;
		int sz_node;
		node * freeNode;
		node * nodeHead;
};

#endif
