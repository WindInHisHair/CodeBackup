//Date: Oct 2011
//Author: Baiyu Xi
//ID:1094
//Descriptions: second homework for Advanced Data Structure
//
//
//
//tips: using "g++ *.cpp" to compile 

#include "nodetree.h"

ivlNode * currentIvlNode = NULL;
int sz_ivlNode;
ivlNode * freeIvlNode = NULL;

void interval_tree::printTree()
{
	printf("the total number is %d\n", num);
	//printf("the union is %d \n", query_length(this));
	ivlNode **	stack = new ivlNode*[num];
	ivlNode ** astack = new ivlNode* [num];

	int sindex = -1;
	int aindex = -1;

	ivlNode * pnode = root;

	if(root == NULL)
		printf("empty tree\n");
	else
		printf("%d \n", pnode ->key);

	sindex ++;
	stack[sindex] = pnode;


	//node * tmp = pnode ->left;
	while(sindex > -1)
	{

		while(sindex >= 0)
		{
			ivlNode * upper = stack[sindex];
			sindex --;
			if(upper ->left != NULL && upper ->left ->left != NULL)
				printf("%d ", upper->left ->key);
			else
				printf("Lf ");
			if(upper ->right != NULL && upper ->right ->left != NULL)
				printf("%d ", upper ->right ->key);
			else
				printf("lf ");
			if( upper ->left !=NULL && upper ->left ->left != NULL)
			{
				aindex ++;
				astack[aindex] = upper ->left;
			}
			if(upper ->right != NULL && upper ->right ->left != NULL)
			{
				aindex ++;
				astack[aindex] = upper ->right;

			}
		}
		printf("\n");


		while(aindex >= 0)
		{
			ivlNode * tmp = astack[aindex];
			aindex --;

			sindex ++;
			stack[sindex] = tmp;

		}
	}//end of while

	printf("\n");
}
ivlNode * getIvlNode()
{
	ivlNode *tmp;
	if(freeIvlNode != NULL)
	{
		tmp = freeIvlNode;
		freeIvlNode = freeIvlNode->next;
	}
	else
	{
		if(currentIvlNode == NULL || sz_ivlNode == 0)
		{
			currentIvlNode =(ivlNode*) malloc(BLOCKSIZE_IVL_NODE * sizeof(ivlNode));
			sz_ivlNode = BLOCKSIZE_IVL_NODE;
					
		}
		tmp = currentIvlNode ++;
		sz_ivlNode -= 1;
	}
	return tmp;
}

void returnIvlNode(ivlNode *n)
{
	n ->next = freeIvlNode;
	freeIvlNode = n;
}

interval_tree::interval_tree()
{
	num = 0;

	root = new ivlNode;
	root ->left = NULL;
	root ->height = 0;
	root ->right = NULL;
}
interval_tree::~interval_tree()
{
}

int interval_tree::max()
{
	ivlNode * n = root;
	while( n ->right != NULL)
		n = n ->right;
	return n ->key;
	
}

int interval_tree::min()
{
	ivlNode * n = root;
	while( n->left != NULL)
		n = n ->left;

	return n ->key;
}


ivlNode * interval_tree::find(int index)
{
	if( root ->left == NULL)
		return NULL;
	else
	{
		if(root ->key == index)
			return root ->left;
		else
		{
			ivlNode *n = root;
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
void interval_tree::add(int index)
{
	ivlNode * pnode;
	ivlNode ** stack = new ivlNode* [num];
	int sindex = -1;
	ivlNode * newNode = getIvlNode();
	newNode ->right = NULL;
	newNode ->left = NULL;
	newNode ->height = 0;
	newNode ->key = index;
	

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
			stack[sindex] = pnode;
			if(index < pnode ->key)
				pnode = pnode ->left;
			else
				pnode = pnode ->right;
		}//end of while

		if(index != pnode ->key)
		{
			ivlNode *oldLeaf, * newLeaf;
			oldLeaf = getIvlNode();
			oldLeaf ->left = pnode ->left;
			oldLeaf ->key = pnode ->key;
			oldLeaf ->height = 0;
			oldLeaf -> right = NULL;

			newLeaf = getIvlNode();
			newLeaf ->left = newNode;
			newLeaf ->key =index;
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

		//rebalacne
		reBalance(stack, sindex);
		
		delete stack;
	}//end of else

	num ++;

}

ivlNode * interval_tree::remove(int index)
{
	
	if(root == NULL)
		return NULL;
	int sindex = -1;
	ivlNode ** stack = new ivlNode*[num];
	ivlNode *delNode;


	if(root ->left == NULL)
		return NULL;
	if(root ->right == NULL)
	{
		if(root ->key == index)
		{
			ivlNode *tmp = root -> left;
			root ->left = NULL;
			num --;
			return tmp;
		}
		else
			return NULL;
	}
	else
	{
		ivlNode *pnode = root;
		ivlNode *upper, *other;
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
			returnIvlNode(pnode);
			returnIvlNode(other);
			sindex --;

		}
		//rebalance
		reBalance(stack, sindex);
	}//end of else
	delete stack;

	num --;
	return delNode;
	
}


void interval_tree::reBalance(ivlNode ** stack, int sindex)
{

	int finished = 0;

	while(sindex > -1 && !finished)
	{
		int tmpHeight, oldHeight;
		ivlNode * pnode = stack[sindex];
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
			if(pnode->left->height > pnode->right->height +1)
				pnode->height= pnode ->left ->height +1;
			else
				pnode ->height= pnode->right ->height +1;
		}
		if(pnode ->height == oldHeight)
			finished = 1;
	}
}
void interval_tree::left_rotate(ivlNode *n)
{
	ivlNode * tmp = n ->left;
	int tmpKey = n ->key;

	n ->left = n ->right;
	n ->key = n ->right ->key;
	n ->right = n ->left ->right;
	n ->left ->right = n ->left ->left;
	n ->left ->left = tmp;
	n ->left ->key = tmpKey;


}
void interval_tree::right_rotate(ivlNode * n)
{
	ivlNode * tmp = n ->right;
	int tmpKey = n ->key;
	n ->right = n ->left;
	n ->key = n ->left ->key;
	n ->left = n ->right ->left;
	n ->right ->left = n ->right ->right;
	n ->right ->right = tmp;
	n ->right ->key = tmpKey;
}
void interval_tree::clear()
{
		num = 0;
		root ->left = NULL;
		root-> right =NULL;
}
