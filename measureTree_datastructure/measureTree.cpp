//Date: Oct 2011
//Author: Baiyu Xi
//ID:1094
//Descriptions: second homework for Advanced Data Structure
//
//
//tips: using "g++ *.cpp" to compile 
#include "measureTree.h"

//interface:
//
m_tree_t * create_m_tree()
{
	m_tree_t * ptr = new m_tree_t;
	return ptr;

}

void insert_interval(m_tree_t* tree,int a, int b)
{
	if(a < b)
		tree -> add(a, b);
	else
		tree -> add(b, a);
}

void delete_interval(m_tree_t * tree, int a,int b)
{
	//if the interval exists, then delete it
	if(tree ->find(a) != NULL && tree->find(b) != NULL)
	{
		if(a < b)
			tree ->del(a,b);
		else
			tree ->del(b, a);
	}
}

int query_length(m_tree_t * tree)
{
	if(tree != NULL)
		return tree ->getLen();
}

//implementation for the class
m_tree_t::m_tree_t()
{

	root = new node;
	root ->height = 0;
	root ->right = NULL;
	root ->left = NULL;

	root ->nodeIvl_l  = MINIF;
	root ->nodeIvl_r  = INIF;


	//apply for space for node
	headNode = NULL;
	currentNode = NULL;
	freeNode = NULL;
	szNode = 0;

	num = 0;
}

m_tree_t::~m_tree_t()
{
	if(root != NULL)
		delete root;
	if(headNode != NULL)
		delete headNode;
}
// functions for apply for the node space
node *m_tree_t::getNode()
{
	node * tmp;

	if(freeNode != NULL)
	{
		tmp = freeNode;
		freeNode = freeNode ->next;
	}
	else
	{
		if(currentNode == NULL || szNode == 0)
		{
			headNode = new node[BLOCKSIZE];
			szNode = BLOCKSIZE;
			currentNode = headNode;

		}
		tmp = currentNode ++;
		szNode --;
	}
	tmp->ep_l.clear();
	tmp->ep_r.clear();
	return tmp;
}
void m_tree_t::returnNode(node * n)
{
	n ->next = freeNode;
	n ->ep_l.clear();
	n ->ep_r.clear();
	freeNode = n;

}
//implementation for homework interface
int m_tree_t::getLen()
{
	if(root == NULL)
		return 0;
	else
		return root ->measure ;
}
void m_tree_t::add(int a, int b)
{

	insertNode(a, b);
	insertNode(b, a);

}
void m_tree_t::del(int a, int b)
{

	removeNode(a, b);
	removeNode(b, a);
}

//private implememtation
node *m_tree_t:: find(int index)
{
	node *ptr= NULL;
	if(root ->left == NULL)
		return NULL;
	else
	{
		node *pNode = root;
		while(pNode -> right != NULL)
		{
			if( index < pNode  ->key)
				pNode = pNode ->left;
			else
				pNode = pNode ->right;

		}//end of while
		if(index == pNode ->key)
			return pNode ->left;
		else
			return NULL;
	}// end of else

	return ptr;
}
int m_tree_t::insertNode(int index, int otherIndex)
{
	node * newNode = getNode();
	newNode ->height = 0;
	newNode ->left = NULL;
	newNode ->right = NULL;
	newNode ->time = 1;
	newNode ->key = index;
	newNode ->ep_l.clear();
	newNode ->ep_r.clear();

	if(index < otherIndex)
		//newNode ->ep_r.insert(otherIndex);
		newNode ->ep_r.add(otherIndex);
	else
		//newNode ->ep_l.insert(otherIndex);
		newNode ->ep_l.add(otherIndex);



	node ** stack = new node* [num];
	int sindex = -1;

	node *pnode = NULL;
	if(root ->left == NULL)
	{
		root ->left =newNode;
		root ->key = index;
		root ->right = NULL;
		root ->height = 0;

		newNode ->nodeIvl_l = root ->nodeIvl_l;
		newNode ->nodeIvl_r = root ->key;

		getLeftMin(root);
		getRightMax(root);
		getMeasure(root);
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

		if(index == pnode ->key)
		{
			//for the case there same endpoint
			pnode ->left ->time ++;
			returnNode(newNode);

			// some ivtervals share  the same endpoint, then I store the correspondence endpoint in vector (STL)
			if(index <otherIndex)
			{
				pnode ->left ->ep_r.add(otherIndex);

				pnode ->left ->rightmax = pnode->left->ep_r.max();
				pnode ->rightmax = pnode ->left ->ep_r.max();
			}
			else
			{
				pnode ->left->ep_l.add(otherIndex);
				pnode ->left ->leftmin = pnode->left->ep_l.min();
				pnode ->leftmin = pnode ->left->ep_l.min();
			}

			pnode ->ep_l = pnode->left ->ep_l;
			pnode ->ep_r = pnode ->left ->ep_r;

			getLeftMin(pnode);
			getRightMax(pnode);
			getMeasure(pnode);


		}
		else
		{
			node * oldLeaf, *newLeaf;
			oldLeaf = getNode();
			oldLeaf->left = pnode ->left;
			oldLeaf ->key = pnode ->key;
			oldLeaf ->right = NULL;
			oldLeaf ->height = 0;


			newLeaf = getNode();
			newLeaf ->left = newNode;
			newLeaf ->key = index;
			newLeaf ->right = NULL;
			newLeaf ->height = 0;

			newLeaf ->ep_l = newNode ->ep_l;
			newLeaf ->ep_r = newNode ->ep_r;

			oldLeaf ->ep_l = pnode ->ep_l;
			oldLeaf ->ep_r = pnode ->ep_r;


			if(pnode ->key <= index)
			{
				pnode ->left = oldLeaf;
				pnode ->right = newLeaf;
				pnode ->key = index;
			}
			else
			{
				pnode ->left = newLeaf;
				pnode ->right = oldLeaf;
			}
			pnode ->height = 1;


			pnode ->left ->nodeIvl_l = pnode ->nodeIvl_l;
			pnode->left ->nodeIvl_r = pnode ->key;

			getLeftMin(pnode->left);
			getRightMax(pnode ->left);
			getMeasure(pnode ->left);

			pnode ->right ->nodeIvl_l = pnode ->key;
			pnode -> right->nodeIvl_r = pnode ->nodeIvl_r;

			getLeftMin(pnode ->right);
			getRightMax(pnode ->right);
			getMeasure(pnode ->right);

			getLeftMin(pnode);
			getRightMax(pnode);
			getMeasure(pnode);
		}//end of else

		int tmpInx = sindex;
		while(tmpInx > -1)
		{
			node* n = stack[tmpInx];
			tmpInx --;
			getLeftMin(n);
			getRightMax(n);
			getMeasure(n);
		}
	}// end of else


	//rebalance the tree
	int finished = 0;
	while(sindex > -1 && !finished)
	{
		int tmpHeight, oldHeight;
		node * ptrn = stack[sindex];
		sindex --;
		oldHeight = ptrn ->height;
		//getMeasure(ptrn);
		if(ptrn ->left ->height -ptrn->right ->height == 2)
		{
			if(ptrn->left->left->height - ptrn->right->height ==1)
			{
				rightR(ptrn);
				ptrn->right->height = ptrn ->right ->left ->height +1;
				ptrn ->height = ptrn->right ->height +1;
			}
			else
			{
				leftR(ptrn->left);
				rightR(ptrn);
				tmpHeight = ptrn ->left ->left ->height;
				ptrn ->left ->height = tmpHeight +1;
				ptrn ->right ->height = tmpHeight +1;
				ptrn ->height = tmpHeight +2;
			}
		}
		else if(ptrn->left->height - ptrn->right->height == -2)
		{
			if(ptrn->right->right->height - ptrn->left->height ==1 )
			{
				leftR(ptrn);
				ptrn ->left ->height = ptrn ->left ->right ->height +1;
				ptrn ->height = ptrn ->left ->height +1;
			}
			else
			{
				rightR(ptrn ->right);
				leftR(ptrn);
				tmpHeight = ptrn ->right ->right ->height;
				ptrn ->left ->height = tmpHeight +1;
				ptrn ->right ->height = tmpHeight +1;
				ptrn ->height = tmpHeight +2;
			}
		}
		else
		{
			if(ptrn ->left ->height > ptrn ->right ->height)
				ptrn->height = ptrn ->left ->height +1;
			else
				ptrn ->height = ptrn ->right ->height +1;
		}
		if(ptrn ->height == oldHeight)
			finished = 1;

	}//end of while

	//release the space for the stack
	delete stack;

	//increase the number for totall elements
	return num ++;
}
node * m_tree_t::removeNode(int index, int otherIndex)
{
	node * upper, *other;
	node *delNode = NULL;

	num --;
	node ** stack = new node*[num];
	int sindex = -1;

	if(root == NULL)
		return NULL;
	else if(root ->right == NULL)
	{
		if(index == root ->key)
		{
			node* pnode = root ->left;
			root ->left = NULL;
			root ->measure = 0;
			root ->ep_l.clear();
			root->ep_r.clear();
			return pnode;
		}
		else return NULL;
	}
	else
	{
		node * pnode = root;
		while(pnode ->right != NULL)
		{
			sindex ++;
			stack[sindex] = pnode;
			upper = pnode;
			if(index < pnode ->key)
			{
				pnode = upper ->left;
				other = upper ->right;
			}
			else
			{
				pnode = upper ->right;
				other = upper ->left;
			}//end of if-else
		}//end of while

		if(index == pnode ->key)
		{
			//printf("get the index %d , node %d\n",index, pnode ->left->key);
			if(pnode ->left ->time == 1)
			{
				upper ->key = other ->key;
				upper ->left = other -> left;
				upper ->right = other ->right;
				upper ->height = other ->height;
				delNode = pnode ->left;
				node * tmpN = pnode;
				returnNode(tmpN  ->left);
				returnNode(other);
				sindex --;

				upper ->left ->nodeIvl_l = upper->nodeIvl_l;
				if(upper ->right ==NULL)
				upper ->left ->nodeIvl_r = upper ->nodeIvl_r;
				else
				{
					upper ->left->nodeIvl_r = upper ->key;
					upper ->right ->nodeIvl_l = upper ->key;
					upper ->right ->nodeIvl_r = upper ->nodeIvl_r;
				}

				getLeftMin(upper);
				getRightMax(upper);
				getMeasure(upper);
			}
			else
			{
				pnode ->left -> time --;

				if(index < otherIndex)
				{

					pnode->left->ep_r.remove(otherIndex);
					pnode->left ->ep_l.remove(index);

				}
				else
				{

					pnode->left->ep_l.remove(otherIndex);
					pnode->left->ep_r.remove(index);

				}

				pnode ->ep_l = pnode->left ->ep_l;
				pnode ->ep_r = pnode ->left ->ep_r;

				if(pnode->left->ep_l.size() != 0)
				{
					pnode ->left ->leftmin = pnode->left->ep_l.min();
				}
				else
				
					pnode->left->leftmin = pnode->left->nodeIvl_l;
					

				if(pnode->left->ep_r.size() != 0)
				{
					pnode ->left ->rightmax = pnode ->left->ep_r.max();
				}
				else
					pnode->left ->rightmax = pnode ->left ->nodeIvl_r;

				getLeftMin(pnode);
				getRightMax(pnode);
				getMeasure(pnode);

				getLeftMin(upper);
				getRightMax(upper);
				getMeasure(upper);
			}//end of else to process the case multiple endpoint

		}
		else
			return NULL;

		int aindex = sindex;
		while(aindex > -1)
		{
			node * n = stack[aindex];
			aindex --;
			getLeftMin(n);
			getRightMax(n);
			getMeasure(n);
		}
	}//end of else

	//rebalance
	int finished = 0;
	while(sindex > -1 && !finished)
	{
		int tmpHeight, oldHeight;

		node * pnode = stack[sindex];
		sindex --;

		oldHeight = pnode ->height;
		if(pnode->left->height -  pnode ->right->height == 2)
		{
			if(pnode->left->left->height - pnode->right->height == 1)
			{
				rightR(pnode);
				pnode ->right ->height = pnode ->right ->left ->height +1;
				pnode ->height = pnode ->right ->height +1;
			}
			else
			{
				leftR(pnode ->left);
				rightR(pnode);
				tmpHeight = pnode ->left ->left ->height;
				pnode ->left ->height = tmpHeight +1;
				pnode ->right ->height = tmpHeight +1;
				pnode ->height = tmpHeight +2;
			}
		}
		else if(pnode->left->height - pnode->right->height == -2)
		{
			if(pnode ->right ->right ->height - pnode->left->height == 1)
			{
				leftR(pnode);
				pnode ->left ->height = pnode->left ->right ->height +1;
				pnode ->height = pnode ->left ->height +1;
			}
			else
			{
				rightR(pnode->right);
				leftR(pnode);
				tmpHeight = pnode ->right ->right ->height;

			}
		}
		else
		{
			if(pnode->left->height > pnode->right->height)
				pnode->height = pnode ->left ->height +1;
			else
				pnode ->height = pnode ->right ->height +1;
		}
		if(pnode -> height == oldHeight)
			finished  =1;
	}// end of while for rebalance

	return delNode;
}
// tool function
void m_tree_t::leftR(node *n)
{
	node * tmpN;
	int tmpK;
	tmpN = n ->left;
	tmpK = n->key;

	n -> left = n -> right;
	n ->key = n ->right ->key;

	n ->right = n ->left ->right;
	n ->left ->right = n -> left -> left;
	n->left ->left = tmpN;
	n ->left ->key = tmpK;



	n ->left ->nodeIvl_l = n ->nodeIvl_l;
	n ->left ->nodeIvl_r = n ->key;

	getLeftMin(n ->left);
	getRightMax( n->left);
	getMeasure(n ->left);
}

void m_tree_t::rightR(node *n)
{
	node * tmpN;
	int tmpK;

	tmpN = n ->right;
	tmpK = n ->key;

	n->right = n ->left;
	n ->key = n ->left ->key;

	n->left= n->right ->left;
	n->right ->left = n ->right ->right ;
	n ->right ->right = tmpN;
	n ->right ->key = tmpK;

	n ->right ->nodeIvl_l = n ->key;
	n ->right ->nodeIvl_r = n ->nodeIvl_r;

	getLeftMin(n ->right );
	getRightMax( n ->right );
	getMeasure(n ->right );


}
//member function for debug  and test
//
void m_tree_t::printTree()
{
	printf("the total number is %d\n", num);
	printf("the union is %d \n", query_length(this));
	node **	stack = new node*[num];
	node ** astack = new node* [num];

	int sindex = -1;
	int aindex = -1;

	node * pnode = root;

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
			node * upper = stack[sindex];
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
			node * tmp = astack[aindex];
			aindex --;

			sindex ++;
			stack[sindex] = tmp;

		}
	}//end of while

	printf("\n");
}
// calcuate the measure
//
int m_tree_t::getMeasure(node *n)
{
	int l = n ->nodeIvl_l;
	int r = n ->nodeIvl_r;

	if(n ->right != NULL)
	{
		if(n ->right ->leftmin 	< l && n ->left->rightmax >= r)
			n ->measure = r -l;
		else if(n->right->leftmin >= l && n->left->rightmax >= r)
			n->measure = (r -n->key) +n->left->measure;
		else if(n->right->leftmin <l && n->left ->rightmax < r)
			n ->measure = n ->right ->measure +(n ->key  -l);
		else if(n->right->leftmin >= l && n->left->rightmax < r)
			n ->measure = n ->right ->measure + n ->left ->measure;
	}
	else
	{
		if(n->leftmin == n->nodeIvl_r || n->rightmax == n ->nodeIvl_l)
			n ->measure = 0;
		else
		{
			if(n ->nodeIvl_l == MINIF)
				n ->measure = n ->nodeIvl_r - n ->leftmin;
			else if(n ->nodeIvl_r == INIF)
				n -> measure = n ->rightmax - n ->nodeIvl_l;
			else
				n ->measure = std::min(n ->nodeIvl_r, n->rightmax) -std::max( n ->nodeIvl_l, n->leftmin);
			//n ->measure = n->nodeIvl_r - n->nodeIvl_l;
		}


	}

	return n->measure;
}

int m_tree_t::getRightMax(node *n)
{
	if(n ->right != NULL)
	{
		if(n ->left->rightmax > n ->right ->rightmax)
			n ->rightmax = n ->left ->rightmax;
		else
			n ->rightmax = n ->right ->rightmax;
	}
	else
	{
		if(n ->left ->ep_r.size() > 0)
		{
			n ->rightmax = n ->left->ep_r.max();
		}
		else
			n ->rightmax = n ->left ->key;
	}

	return n ->rightmax;
}

int m_tree_t::getLeftMin(node *n)
{
	if(n ->left != NULL && n->right != NULL) //inter node
	{
		if(n ->left ->leftmin < n ->right ->leftmin)
			n->leftmin = n ->left->leftmin;
		else
			n->leftmin = n ->right ->leftmin;
	}
	else if(n ->left != NULL && n ->right == NULL) //leaf node
	{

		if(n ->left ->ep_l.size() > 0)
		{
			n ->leftmin = n ->left->ep_l.min();
		}
		else
			n ->leftmin = n ->left ->key;

	}
	return n->leftmin;
}
