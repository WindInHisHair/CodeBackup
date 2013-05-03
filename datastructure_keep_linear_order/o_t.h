#ifndef __O_T_H__
#define __O_T_H__
#include <stdio.h>
#include <stdlib.h>

#include "nodetree.h"
#define NODE_SIZE 3000000;
class o_t
{
	public:
		o_t();
		~o_t();
		void insertBefore(long a, long b);
		void insertAfter(long a, long b);
		void insertTop(long a);
		void insertBottom(long a);
		void del(long a);
		int comp(long a, long b);

	private:
	
		node * getNode();
		void returnNode(node *p);
		node * currentNodeHead;
		node * freeHead;
		node * nodeStack;
		int sz_node;
		node * find(long key);
		void orderIns(node* prev, node * pos);
		void orderRem(node *pos);
		void orderReb(node** stack, int index);

		void leftR(node * p);
		void rightR(node * p);

		node * oroot;	
		int num;
		node * head;
		node * tail;

		key_tree * ktree;
};

//the interface for the homework
o_t * create_order();
void insert_before(o_t * ord, long a, long b);
void insert_after(o_t * ord, long a, long b);

void insert_top(o_t * ord, long a);
void insert_bottom(o_t * ord, long a);

void delete_o(o_t * ord, long a);
int is_before(o_t *ord, long a, long b);
#endif
