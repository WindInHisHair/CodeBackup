//Author: Baiyu Xi
//ID:  1094
//Date: Oct 2011
//Descritptions: the first homework for "advanced data structure", Fall, 2011
// using g++ to compile  or use the "make" command, the Makefile
#ifndef  MY_TEXT_STRUCTURE__
#define  MY_TEXT_STRUCTURE__
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
//the node for the tree struct
struct Node
{
	int index;
	char* line;
	int height;
	struct Node* left;
	struct Node* right;
	struct Node* next;
};


typedef struct Node node;
class text_t
{

	public:
		text_t();
		~text_t();
		//this public functions could be used for the interface implementation
		char* getLine(int index);
		void appendLine(char * nline);
		char* setLine(int index,  char* nline);
		void insertLine(int index, char* nline);
		char * delLine(int index);
		int getLen(){return length;};

	private:

		int length;
		node* ptr_node;
		node* phead;
        int sz_lv;
        node* free_list;

		void generateTree();
		void addNode(int index, char* nline);
		char* delNode(int index);

		//tree operations
		void rotateLeft(node * n);
		void rotateRight(node *n);
		void reBalance(node *n);
		int  childNum(const node *n);

		//function for space applying
		void return_node(node* n);
		node * get_node();

        node* getNode(int index);
		node * root;

};

// the interface function for the homework requirements
// all of them will be implemented by the memeber functions of class text_t
text_t* create_text();
int length_text(text_t *txt);
char * get_line(text_t *txt, int index);
void append_line(text_t *txt, char * new_line);
char * set_line(text_t *txt, int index, char* new_line);
void insert_line(text_t *txt, int index, char * new_linei);
char * delete_line(text_t *txt, int index);




#endif