#include "o_t.h"
o_t* create_order()
{
	return new o_t();
}

void insert_before(o_t * ord, long a, long b)
{
	ord ->insertBefore(a, b);
}
void insert_after(o_t *ord, long a, long b)
{
	ord ->insertAfter(a, b);
}

void insert_top(o_t * ord, long a)
{
	ord ->insertTop(a);
}
void insert_bottom(o_t * ord, long a)
{
	ord ->insertBottom(a);
}
void delete_o(o_t * ord, long a)
{
	ord->del(a);
}
int is_before(o_t * ord, long a, long b)
{

	return ord ->comp(a, b);
}
//the implementation for the class function
//
node* o_t::getNode()
{

	node * tmp;
	tmp = currentNodeHead ++;
	sz_node --;
		
	tmp ->left = NULL;
	tmp ->right = NULL;
	tmp ->lnext = NULL;
	tmp ->lprev = NULL;
	tmp ->up = NULL;
	tmp ->height = 0;
	return tmp;
}
void o_t::returnNode(node *p)
{
	p ->next = freeHead;
	freeHead = p;
}
o_t::o_t()
{
	oroot = new node;
	oroot ->left = oroot ->right = NULL;
	oroot ->up = NULL;
	oroot ->lprev = oroot->lnext = NULL;

	num = 0;
	head = NULL;
	tail = NULL;
	ktree = new key_tree();
	//	nodeStack = new node [NODE_SIZE];

	int tmpN = NODE_SIZE;
	nodeStack = new node [tmpN];
	sz_node = NODE_SIZE;
	currentNodeHead = nodeStack;
	freeHead = NULL;
}
o_t::~o_t()
{
	if(ktree != NULL)
		delete ktree;
	if(nodeStack != NULL)
		delete [] nodeStack;
	if(oroot != NULL)
		delete oroot;
}
void o_t::orderIns(node * prev, node * pos)
{
	if( oroot ->left == NULL)
	{
		oroot ->left = pos;
		oroot ->height = 1;
		pos ->up = oroot;
	}
	else
	{
		node ** stack = new node*[oroot->height + 2];
		int index = -1;
	
		node* lleaf = getNode();
		node * rleaf = getNode();
		node * parent;
		if(prev == NULL)
		{
			parent = head ->up;

			rleaf ->left = head;
			head ->up = rleaf;

			lleaf ->left = pos;
			pos ->up = lleaf;
		}
		else
		{
			parent = prev ->up;

			rleaf ->left = pos;
			pos ->up = rleaf;

			lleaf ->left = prev;
			prev ->up = lleaf;
		}


		lleaf ->up = parent;
		rleaf ->up = parent;

		parent ->left = lleaf;
		parent->right =  rleaf;
		
		parent ->height = 1;

		while(parent ->up != NULL)
		{
			index ++;
			stack[index] = parent ->up;
			parent = parent ->up;
		}

		orderReb(stack, index);
		delete [] stack;
	}

	if(prev == tail)
		tail = pos;
	if(prev == NULL)
		head = pos;
	num ++;
}

void o_t::orderRem(node * pos)
{
	node * parent = pos ->up;
	node * grand = pos ->up ->up;

	if(grand == NULL)
	{
		if(parent != oroot)
		{
			printf("error in delete\n");
			exit(0);
		}
		parent ->left = NULL;
	}
	else
	{
		node * other;
		if(grand ->left == parent)
			other = grand ->right;
		else
			other = grand ->left;

		if(other == NULL)
			printf("other is null\n");

		grand ->height = other ->height;
		grand ->left = other ->left;
		grand ->right = other ->right;
		
		grand ->left ->up = grand;

		if(grand->right != NULL)
			grand ->right ->up = grand;

		int index = -1;
		node ** stack = new node*[oroot ->height + 2];

		while(grand ->up != NULL)
		{
			index ++;
			stack[index] = grand ->up;
			grand = grand ->up;
		}

		orderReb(stack ,index);
		delete [] stack;

		returnNode(other);
		returnNode(parent);
	}

	if(pos == head)
		head = head ->lnext;
	else if(pos == tail)
		tail = tail ->lprev;

	returnNode(pos);
	num --;
}
void o_t::orderReb(node** stack, int index)
{

	int finished = 0;
	int sindex = -1;
	while(sindex < index && !finished)
	{
		int tmpHeight, oldHeight;
		sindex ++;
		node * pnode = stack[sindex];

		oldHeight = pnode ->height;
		if(pnode ->left -> height - 
				pnode ->right -> height == 2)
		{
			if(pnode->left ->left ->height -
					pnode ->right ->height == 1)
			{
				rightR(pnode);
				pnode ->right ->height = pnode ->right->left ->height + 1;
				pnode ->height = pnode ->right ->height + 1;
			}
			else
			{
				leftR(pnode -> left);
				rightR(pnode);
				tmpHeight = pnode ->left ->left ->height;
				pnode ->left ->height = tmpHeight +1;
				pnode ->right ->height = tmpHeight + 1;

				pnode ->height = tmpHeight + 2;
			}
		}
		else if(pnode ->left ->height - 
				pnode ->right ->height == -2)
		{
			if(pnode->right->right ->height - 
					pnode ->left->height ==1)
			{
				leftR(pnode);
				pnode ->left ->height = pnode ->left ->right ->height + 1;
				pnode ->height = pnode ->left ->height + 1;
			}
			else
			{
				rightR(pnode ->right);
				leftR(pnode);
				tmpHeight = pnode ->right ->right ->height;
				pnode ->left ->height = tmpHeight + 1;
				pnode ->right ->height = tmpHeight + 1;
				pnode ->height = tmpHeight + 2;
			}
		}
		else
		{
			if(pnode ->left ->height > pnode ->right ->height)
				pnode ->height = pnode ->left ->height +1;
			else
				pnode ->height = pnode ->right ->height +1;
		}
		if(pnode ->height == oldHeight)
			finished = 1;
	}//end of while

}
void o_t::leftR(node * p)
{
	node * tmp;
	tmp = p ->left;
	p ->left = p ->right;
	p ->left ->up = p;

	p ->right = p ->left ->right;
	p ->right ->up = p;
	
	p ->left ->right = p ->left ->left;
	p ->left ->right ->up = p ->left ;
	
	p ->left ->left = tmp;
	p ->left ->left ->up = p ->left;
}
void o_t::rightR(node * p)
{
	node * tmp;
	tmp = p ->right;
	p ->right = p ->left;
	p ->right ->up = p;

	p ->left = p ->right ->left;
	p ->left ->up = p;

	p ->right ->left = p ->right ->right;
	p ->right ->left ->up = p ->right;

	p ->right ->right = tmp;
	p ->right ->right ->up = p ->right;
}
void o_t::insertBefore(long a,  long b)
{
	node * pos = ktree ->find(b);
	node * newNode = getNode();
	newNode ->key = a;
	
	// insert into the list
	node * tmp = pos ->lprev;
	pos ->lprev = newNode;
	newNode ->lprev = tmp;
	newNode ->lnext = pos;
	if(tmp != NULL)
		tmp ->lnext = newNode;

	// insert into the order tree
	orderIns(tmp, newNode);
	
	// insert into the key tree
	ktree ->add(a, newNode);
}
void o_t::insertAfter(long a, long b)
{
	node * pos = ktree ->find(b);
	node * newNode = getNode();
	newNode ->key = a;

	//insert into the list
	node * tmp = pos ->lnext;
	pos ->lnext = newNode;
	newNode ->lnext = tmp;
	newNode ->lprev = pos;
	if(tmp != NULL)
		tmp ->lprev = newNode;
	// insert into the order tree
	orderIns(pos,newNode);	

	// insert into the key tree
	ktree ->add(a, newNode);
}
void o_t::insertBottom(long a)
{

	//insert into the list
	node * newNode = getNode();
	newNode ->key = a;

	newNode ->lnext = head;
	newNode ->lprev = NULL;
	if(head != NULL)
		head ->lprev = newNode;

	//insert into the order tree
	orderIns(NULL, newNode);


	//insert into the key tree
	ktree ->add(a, newNode);
}

void o_t::insertTop(long a)
{
	node * newNode = getNode();
	newNode ->key = a;

	newNode ->lnext = NULL;
	newNode ->lprev = tail;
	if(tail != NULL)
		tail ->lnext = newNode;

	//insert into the order tree
	orderIns(tail, newNode);	

	//insert into the key tree
	ktree->add(a,newNode);
}

void o_t::del(long a)
{

	//remove from the key tree and list
	node * pos = ktree ->remove(a);
	node * p = pos ->lprev;
	node * n = pos ->lnext;
	if(p != NULL)
		p ->lnext = n;

	if(n != NULL)
		n ->lprev = p;

	
	//remove from the order tree
	orderRem(pos);
}
int o_t::comp(long a, long b)
{

	int order = 0;
	node *pos1 = ktree ->find(a);
	node *pos2 = ktree ->find(b);

	node ** lstack = new node*[oroot ->height +2];
	int  index = -1;

	//find the lca from the order tree
	while(pos1 ->up != NULL)
	{
		index ++;
		lstack[index] = pos1 ;
		pos1 = pos1 ->up;
	}

	index ++;
	lstack[index] = pos1;
	
	bool BRK = false;
	
	node * upper = pos2 ->up;
	while(upper ->up != NULL)
	{
		int sindex = -1;
		while(sindex < index)
		{
			sindex ++;
			if(upper  == lstack[sindex])
			{
				
				if(upper ->right == pos2)
				{
					order = 1;
					BRK = true;
					break;
				}
				if(upper ->left == pos2)
				{
					order = 0;
					BRK = true;
					break;
				}
			}
		}
		if(BRK)
			break;
		upper  = upper ->up;
		pos2 = pos2 ->up;
	}
	if(upper  == oroot && upper ->right == pos2)
		order = 1;

	delete [] lstack;
	
	return order; 
}
