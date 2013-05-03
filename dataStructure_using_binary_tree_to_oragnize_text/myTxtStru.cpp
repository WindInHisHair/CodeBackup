
//Tips: my implementation is slightly different from the Prof. Brass's book:
//in my tree, the if key value <= a node key, then turn left, otherwise turn right.

// I use the leaf node to store data, and any interior node key the summary number for all the leaf node in its left-subtree.
//By doing so, the line number could be assigned dynamicly and we can avoid renumbering.


// using g++ to compile  or use the "make" command, the Makefile
//////////////////////////////////////////
#include "myTxtStru.h"
#define BLOCKSIZE 10240

// two function for apply new node and return the deleted node
node * text_t::get_node()
{
	node* tmp;
	if(free_list != NULL)
	{
		tmp = free_list;
		free_list = free_list -> next;
	}
	else
	{
		if(ptr_node == NULL || sz_lv == 0)
		{
			phead = (node*) malloc(BLOCKSIZE * sizeof(node));
			ptr_node = phead;
			sz_lv = BLOCKSIZE;
		}
		tmp = ptr_node ++;
		sz_lv -= 1;
	}
	return tmp;
}

void text_t::return_node(node* n)
{
	n ->next = free_list;
	free_list = n;

}

////////////////////////////////////////////
// the Seven functions for the homework interface
// all these interfaces will be implemented with a class named "text_t"
text_t * create_text()
{
	//text_t *:text=NULL;
	//
	//return text;
	text_t * pTxt = new text_t();

	return pTxt;
}

int length_text(text_t *txt)
{

	if(txt == NULL) return 0;
	return txt ->getLen();
}

char * get_line(text_t *txt, int index)
{
	if(txt == NULL) return NULL;

	return txt ->getLine(index);
}

void append_line(text_t * txt, char * new_line)
{
	txt -> appendLine(new_line);
}

char* set_line(text_t * txt, int index, char* new_line)
{

	if(txt == NULL) return NULL;
	return txt ->setLine(index, new_line);
}

void insert_line(text_t * txt, int index,  char * new_line)
{
	if(txt != NULL)

	txt ->insertLine(index, new_line);
}

char * delete_line(text_t * txt, int index)
{
	if(txt != NULL)
	return txt ->delLine(index);
	else
	return NULL;
}
//
//////////////////////////////////////////////////////////
//text_t class member function and member variables
text_t::text_t()
{
	length = 0;
	generateTree();
	ptr_node = NULL;
    free_list= NULL;
}
text_t::~text_t()
{
	//if(root != NULL)
	//	delTree(root);
	if(root != NULL)
	delete root;
	free(phead);
}
// calculate the leaf number for a node
int text_t::childNum(const node *n)
{
    int num = 0;
    if (n == NULL) return 0;
     if(n ->left == NULL)
      return  1;
     else
     {
         num += childNum(n -> left);
         if(n -> right != NULL)
         num += childNum(n -> right);
     }

     return num;

}
//public function for the interfaces
char * text_t::getLine(int index)
{
	if(index <= 0) return NULL;
	if(index > length) return NULL;

	return getNode(index) ->line;
}

void text_t::appendLine(char* nline)
{
	this->addNode(length+1, nline);
}
char* text_t::setLine(int index, char* nline)
{
	if(index <= 0)
		return NULL;
	if(index > length)
		return NULL;

	if(nline == NULL)
		return NULL;
    node * n = getNode(index);
    n ->line = nline;

	return getNode(index -1) ->line;
}

void text_t::insertLine(int index, char* nline)
{
	addNode(index, nline);
}
char* text_t::delLine(int index)
{

	if(index <= 0)
		return NULL;
	if(index > length)
		return NULL;

	return delNode(index);


}
///////////////////
//private functions for implementations
void text_t::generateTree()
{

	root = new node;

	root ->left = NULL;
	root -> right = NULL;
	root -> line = NULL;
	root ->index = 0;
	root -> height = 0;

}


node * text_t::getNode(int index)
{
	node * n;
//	if(index <= 0) return NULL;
//	if(index > length) return NULL;

	if (root ->left == NULL)
		return NULL;
	else
	{
		n = root;
		int currentKey = n -> index;
		int sum  = 0;
		while(n -> right != NULL)
		{
			if(index <= currentKey)
			{
				n = n ->left;
				currentKey = sum+n ->index;
			}
			else
			{
				sum += n -> index;
				n = n ->right;
				currentKey = sum + n ->index;
			}
		}//end of while

		if(currentKey == index)
			return n ->left;
		else return NULL;
	}// end of else

	return n;
}

void text_t::addNode(int index, char* nline)
{
	node* newNode = get_node();

	newNode ->line = nline;
	newNode -> index = 1;
	newNode  -> height = 0;
	newNode -> right = NULL;
	newNode -> left == NULL;


	node **stack = new node*[length];
	int sindex = -1;
	//newNode ->height
	//
	//find the position : before the index
	node* pnode = root;
	if(pnode -> left == NULL)
	{
		pnode -> left = newNode;
		pnode -> index = 1;
		pnode -> right = NULL;
		pnode -> height  = 0;
	}
	else
	{
		int currentKey = pnode -> index;
		int sum = 0;
		while(pnode ->right != NULL)
		{
		    sindex ++;
            stack[sindex] = pnode;

			if(index <= currentKey)
			{
                pnode ->index ++;
				pnode = pnode ->left;
				currentKey = sum +pnode -> index;
			}
			else
			{
				sum += pnode -> index;
				pnode = pnode ->right;
				currentKey = sum + pnode ->index;
			}

		}// end of while

			if(index == currentKey)
			{
				//insert the new node into the tree and also rebalance the tree

				//pnode  = pnode -> left;// important to make the pointer to the leaves node

				node* newLeaf = get_node();
				newLeaf -> right = NULL;
				newLeaf -> index = 1;
				newLeaf -> left  =  newNode;
				newLeaf ->height = 0;

				node * oldLeaf = get_node();
				oldLeaf -> right = pnode ->left ->right;
				oldLeaf -> index = pnode ->left -> index;
				oldLeaf -> left = pnode -> left;
				oldLeaf ->line = pnode ->left -> line;

				pnode -> right = oldLeaf;
				pnode -> left  = newLeaf;
				pnode -> height = 1;
			}
			if(index == currentKey +1)
			{
			    node *newLeaf = get_node();
			    newLeaf -> right = NULL;
			    newLeaf -> height = 0;
			    newLeaf -> index = 1;
			    newLeaf -> left = newNode;

			    node * oldLeaf = get_node();
				oldLeaf -> right = pnode ->left ->right;
				oldLeaf -> index = pnode ->left -> index;
				oldLeaf -> left = pnode -> left;
				oldLeaf ->line = pnode ->left -> line;

			    pnode ->right = newLeaf;
			    pnode ->left = oldLeaf;
			    pnode ->height  = 1;

			}

	//if necessary rebalanced the tree
			int finished = 0;
			while(sindex >= 0 && !finished)
			{
				node * tmp = stack[sindex];
				sindex --;


				int tmp_h, old_h;
				old_h = tmp ->height;

				if(tmp ->left ->height - tmp->right ->height == 2)
				{
					if(tmp ->left ->left -> height  - tmp ->right -> height == 1)
					{
						rotateRight(tmp);
						tmp->right -> height = tmp -> right -> left -> height +1;
						tmp -> height = tmp ->right -> height +1;
					}
					else
					{
						rotateLeft(tmp -> left);
						rotateRight(tmp);

						tmp_h = tmp -> left -> left -> height;
						tmp -> left -> height = tmp_h + 1;
						tmp -> right -> height = tmp_h =1;
						tmp-> height = tmp_h +2;
					}

				}
				else if(tmp ->left ->height - tmp ->right -> height == -2)
				{
					if(tmp -> right -> right -> height - tmp -> left -> height == 1)
					{
						rotateLeft(tmp);
						tmp -> left -> height = tmp -> left -> right ->height +1;
						tmp -> height = tmp -> left ->height +1;
					}
					else
					{
						rotateRight(tmp->right);
						rotateLeft(tmp);
						tmp_h = tmp -> right -> right -> height;
						tmp -> left -> height = tmp_h +1;
						tmp -> right -> height = tmp_h +1;
						tmp -> height = tmp_h +2;
					}

				}
				else
				{
					//if there is no need to rebalance the tree
					//just update the height
					if(tmp ->left ->height > tmp -> right -> height)
						tmp ->height = tmp ->left ->height +1;
					else
						tmp -> height = tmp -> right -> height +1;
				}

				if(tmp -> height == old_h) finished = 1;


			}// end of while
			delete stack;
	}//end of else

    length ++;

}

char* text_t::delNode(int index)
{

	//find the position indicated by the index
	//if necessary, rebalanced the tree

	int currentKey = 0;
	int sum = 0;
	node ** stack = new node*[length];
	int sindex = -1;
	char * ptr = new char[64];

    node* pNode = root;

	if(pNode -> left == NULL)
	{
		return NULL;
	}
	else if(pNode -> right == NULL)
	{
        if(pNode -> index == index)
        {
            strcpy(ptr, pNode ->left ->line);
            return_node(pNode -> left);
            return ptr;
        }
        else
        return NULL;
	}
	else
	{
	    currentKey = pNode ->index;
	    sum = 0;
	    node * upperNode = NULL;
	    node * otherNode = NULL;
		while(pNode ->right != NULL)
		{
		    sindex ++;
		    stack[sindex] = pNode;
		    upperNode = pNode;
			if(index <= currentKey)
			{
			    pNode ->index --;
			    pNode = pNode -> left;
			    otherNode = upperNode -> right;
			    currentKey = sum + pNode ->index;
			}
			else
			{
			    sum += pNode -> index;
			    pNode = pNode ->right;
			    otherNode = upperNode -> left;
			    currentKey = sum + pNode ->index;
			}

		}//end of while

		if(index == currentKey)
        {
            //delete the node and rebalance the tree if necessary

            strcpy(ptr, pNode ->left ->line);

            upperNode ->index = otherNode -> index;
            upperNode ->left = otherNode->left;
            upperNode ->right = otherNode -> right;
            upperNode -> height = otherNode ->height;

            return_node(pNode ->left);
            return_node(otherNode);

            sindex --;
		}
		else
            return NULL;



        //if necessary rebalance the tree;
        int finished = 0;
        node * tmp = NULL;
        while(sindex >= 0 && !finished )
        {
            tmp = stack[sindex];
            sindex --;

            int tmp_h, old_h;
            old_h = tmp->height;

            if(tmp ->left ->height - tmp ->right ->height == 2)
            {
                if(tmp->left ->left->height  -  tmp->right->height == 1)
                {
                    rotateRight(tmp);
                    tmp->right->height = tmp->right->height +1;
                    tmp->height = tmp->right->left ->height+1;
                }
                else
                {
                    rotateLeft(tmp->left);
                    rotateRight(tmp);
                    tmp_h =tmp->left->left->height;
                    tmp->left->height=tmp_h +1;
                    tmp->right->height-tmp_h +1;
                    tmp->height = tmp_h +2;
                }

            }
            else if(tmp ->left -> height - tmp->right->height == -2)
            {
                if(tmp->right->right->height  - tmp->left->height == 1)
                {
                    rotateLeft(tmp);
                    tmp->left->height = tmp->left->right->height +1;
                    tmp->height = tmp->left->height +1;

                }
                else
                {
                    rotateRight(tmp->right);
                    rotateLeft(tmp);
                    tmp_h = tmp->right ->right -> height;
                    tmp ->left ->height = tmp_h +1;
                    tmp->right ->height = tmp_h +1;
                    tmp ->height = tmp_h +2;
                }
            }//end of else if
            else
            {
                if(tmp ->left ->height > tmp ->right ->height)
                tmp->height = tmp ->left ->height +1;
                else
                tmp ->height = tmp ->right ->height +1;

            }

            if(tmp->height == old_h)
            finished = 1;

        }//end of the rebalance (while)

	}// end of else

	delete stack;
	length --;

	return ptr;
}

//tree operations
void text_t::rotateLeft(node *n)
{
	node * tmp_node;
	int tmp_index ;
	tmp_node = n ->left;
	tmp_index = n ->index;
	n -> left = n -> right;

	n -> right = n ->left -> right;
	n -> left -> right = n  -> left -> left;
	n -> left ->left = tmp_node;


	n -> left ->index = childNum(n->left ->left);
	n -> index = childNum(n -> left);
}
void text_t::rotateRight(node * n)
{
	node * tmp_node;
	int tmp_index;
	tmp_node = n ->right;
	tmp_index = n ->index;
	n ->right = n -> left;
	n ->left = n ->right ->left;

	n->right -> left = n -> right -> right;
	n -> right ->right = tmp_node;

	n ->right -> index = childNum(n -> right ->left);
	n ->index = childNum(n -> left);
}
