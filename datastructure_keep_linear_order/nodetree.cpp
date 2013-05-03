#include "nodetree.h"
node * key_tree::getNode()
{
	node *tmp;
	if(currentNodeHead == NULL || sz_node == 0)
	{
		currentNodeHead =(node*) malloc(BLOCKSIZE_NODE * sizeof(node));
		nodeHead = currentNodeHead;
		sz_node = BLOCKSIZE_NODE;

	}
		tmp = currentNodeHead ++;
		sz_node -= 1;
	tmp ->up = NULL;
	tmp ->left = NULL;
	tmp ->right = NULL;
	tmp ->next = NULL;
	tmp ->lnext = tmp ->lprev = NULL;
	tmp ->height = 0;
	return tmp;
}

void key_tree::returnNode(node *n)
{
	n ->next = freeNode;
	freeNode = n;
}

key_tree::key_tree()
{
	num = 0;
	root = new node;
	root ->left = NULL;
	root ->right = NULL;
	root ->up = NULL;
	root ->lprev = root ->lnext = 0;
	root ->next = 0;
	currentNodeHead = NULL;
	freeNode = NULL;
	nodeHead = NULL;
	sz_node = 0;
}
key_tree::~key_tree()
{

	if(nodeHead != NULL)
		free(nodeHead);
}

node * key_tree::find(long index)
{
	if( root ->left == NULL)
		return NULL;
	else
	{
		if(root ->key == index)
			return root ->left;
		else
		{
			node *n = root;
			while(n ->right != NULL)
			{
				if(index < n->key)
					n = n ->left;
				else
					n = n ->right;
			}//end of while
			
			if(index == n ->key)
				return n ->left;
			else
				return NULL;

		}//end of else
	}
}
void key_tree::add(long index, node* newNode)
{
	node * pnode;
	node ** stack = new node* [root ->height + 2];
	int sindex = -1;

	if(root ->left == NULL)
	{
		root ->left =newNode;
		root ->key = index;
		root ->height = 0;
		root->right = NULL;
	}
	else
	{
		pnode = root;
		while(pnode ->right != NULL)
		{
			sindex ++;
			//pnode ->height ++;
			stack[sindex] = pnode;
			if(index < pnode ->key)
				pnode = pnode ->left;
			else
				pnode = pnode ->right;
		}//end of while

		if(index != pnode ->key)
		{
			node *oldLeaf, * newLeaf;
			oldLeaf = getNode();
			oldLeaf ->left = pnode ->left;
			oldLeaf ->key = pnode ->key;
			oldLeaf ->height = 0;
			oldLeaf -> right = NULL;

			newLeaf = getNode();
			newLeaf ->left = newNode;
			newLeaf ->key = index;
			newLeaf ->right = NULL;
			newLeaf ->height = 0;
			if(pnode ->key < index)
			{
				pnode ->left = oldLeaf;
				pnode -> right = newLeaf;
				pnode -> key = index;
			}
			else
			{
				pnode -> left = newLeaf;
				pnode -> right = oldLeaf;
			}
			pnode ->height = 1;
		}//end of else

		reBalance(stack, sindex);
		
		delete stack;
	}//end of else

	num ++;
}

node * key_tree::remove(long index)
{
	num --;
	int sindex = -1;
	node ** stack = new node*[root ->height + 2];
	node *delNode;


	if(root == NULL)
		return NULL;
	if(root ->left == NULL)
		return NULL;
	if(root ->right == NULL)
	{
		if(root ->key == index)
		{
			node *tmp = root -> left;
			root ->left = NULL;
			return tmp;
		}
		else
			return NULL;
	}
	else
	{
		node *pnode = root;
		node *upper, *other;
		while(pnode -> right != NULL)
		{
			sindex ++;
			stack[sindex] = pnode;
			upper = pnode ;
			if(index < pnode -> key)
			{
				pnode = upper ->left;
				other = upper -> right;
			}
			else
			{
				pnode = upper ->right;
				other = upper ->left;
			}
			
		}//end of while

		if(index != pnode ->key)
			return NULL;
		else
		{
			upper ->key = other ->key;
			upper ->left = other ->left;
			upper ->right = other ->right;
			upper ->height = other ->height;
			delNode = pnode -> left;
			returnNode(pnode);
			returnNode(other);
			 sindex --;

		}
		//rebalance
		reBalance(stack, sindex);
	}//end of else
	delete stack;
	return delNode;
	
}


void key_tree::reBalance(node ** stack, int sindex)
{
	int finished = 0;

	while(sindex > -1 && !finished)
	{
		int tmpHeight, oldHeight;
		node * pnode = stack[sindex];
		sindex --;

		oldHeight = pnode ->height;
		if(pnode ->left->height - pnode->right->height == 2)
		{
			if(pnode->left->left ->height - pnode->right->height == 1)
			{
				right_rotate(pnode);
				pnode->right ->height= pnode-> right->left ->height +1;
				pnode ->height = pnode ->right ->height +1;
			}
			else
			{
				left_rotate(pnode->left);
				right_rotate(pnode);
				tmpHeight = pnode->left->left->height;
				pnode ->left->height = tmpHeight +1;
				pnode ->right ->height = tmpHeight +1;
				pnode ->height = tmpHeight + 2;
			}
		}
		else if(pnode ->left ->height - pnode->right->height == -2)
		{
			if(pnode ->right ->right ->height - pnode ->left->height  == 1)
			{
				left_rotate(pnode);
				pnode ->left ->height = pnode ->left ->right ->height +1;
				pnode ->height = pnode ->left ->height +1;
			}
			else
			{
				right_rotate(pnode->right);
				left_rotate(pnode);
				tmpHeight = pnode->right ->right ->height;
				pnode->left ->height = tmpHeight +1;
				pnode ->right ->height = tmpHeight + 1;
				pnode ->height = tmpHeight + 2;
			}
		}
		else 
		{
			if(pnode->left->height > pnode->right->height )
				pnode->height= pnode ->left ->height +1;
			else
				pnode ->height= pnode->right ->height +1;
		}
		if(pnode ->height == oldHeight)
			finished = 1;
	}
}
void key_tree::left_rotate(node *n)
{
	node * tmp = n ->left;
	long tmpKey = n ->key;

	n ->left = n ->right;
	n ->key = n ->right ->key;
	n ->right = n ->left ->right;
	n ->left ->right = n ->left ->left;
	n ->left ->left = tmp;
	n ->left ->key = tmpKey;
}
void key_tree::right_rotate(node * n)
{
	node * tmp = n ->right;
	long tmpKey = n ->key;
	n ->right = n ->left;
	n ->key = n ->left ->key;
	n ->left = n ->right ->left;
	n ->right ->left = n ->right ->right;
	n ->right ->right = tmp;
	n ->right ->key = tmpKey;
}

