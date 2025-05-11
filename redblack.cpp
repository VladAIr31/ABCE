#include <iostream>
#include <queue>
#include <fstream>

using namespace std;
ifstream f("abce.in");
ofstream g("abce.out");
const int maxim = 2147483647;
enum COLOR { RED, BLACK };
void nos()
{
    ios_base::sync_with_stdio(false);
    f.tie(NULL);
}

class Nod {
public:
int val;
COLOR color;
Nod *left, *right, *parent;

Nod(int val) : val(val) {
	parent = left = right = NULL;

	// Node is created during insertion
	// Node is red at insertion
	color = RED;
}

// returns pointer to uncle
Nod *uncle() {
	// If no parent or grandparent, then no uncle
	if (parent == NULL || parent->parent == NULL)
	return NULL;

	if (parent->isOnLeft())
	// uncle on right
	return parent->parent->right;
	else
	// uncle on left
	return parent->parent->left;
}

// check if node is left child of parent
bool isOnLeft() { return this == parent->left; }

// returns pointer to sibling
Nod *sibling() {
	// sibling null if no parent
	if (parent == NULL)
	return NULL;

	if (isOnLeft())
	return parent->right;

	return parent->left;
}

// moves node down and moves given node in its place
void moveDown(Nod *nParent) {
	if (parent != NULL) {
	if (isOnLeft()) {
		parent->left = nParent;
	} else {
		parent->right = nParent;
	}
	}
	nParent->parent = parent;
	parent = nParent;
}

bool hasRedChild() {
	return (left != NULL && left->color == RED) ||
		(right != NULL && right->color == RED);
}
};

class RBTree {
Nod *root;

// left rotates the given node
void leftRotate(Nod *x) {
	// new parent will be node's right child
	Nod *nParent = x->right;

	// update root if current node is root
	if (x == root)
	root = nParent;

	x->moveDown(nParent);

	// connect x with new parent's left element
	x->right = nParent->left;
	// connect new parent's left element with node
	// if it is not null
	if (nParent->left != NULL)
	nParent->left->parent = x;

	// connect new parent with x
	nParent->left = x;
}

void rightRotate(Nod *x) {
	// new parent will be node's left child
	Nod *nParent = x->left;

	// update root if current node is root
	if (x == root)
	root = nParent;

	x->moveDown(nParent);

	// connect x with new parent's right element
	x->left = nParent->right;
	// connect new parent's right element with node
	// if it is not null
	if (nParent->right != NULL)
	nParent->right->parent = x;

	// connect new parent with x
	nParent->right = x;
}

void swapColors(Nod *x1, Nod *x2) {
	COLOR temp;
	temp = x1->color;
	x1->color = x2->color;
	x2->color = temp;
}

void swapValues(Nod *u, Nod *v) {
	int temp;
	temp = u->val;
	u->val = v->val;
	v->val = temp;
}

// fix red red at given node
void fixRedRed(Nod *x) {
	// if x is root color it black and return
	if (x == root) {
	x->color = BLACK;
	return;
	}

	// initialize parent, grandparent, uncle
	Nod *parent = x->parent, *grandparent = parent->parent,
		*uncle = x->uncle();

	if (parent->color != BLACK) {
	if (uncle != NULL && uncle->color == RED) {
		// uncle red, perform recoloring and recurse
		parent->color = BLACK;
		uncle->color = BLACK;
		grandparent->color = RED;
		fixRedRed(grandparent);
	} else {
		// Else perform LR, LL, RL, RR
		if (parent->isOnLeft()) {
		if (x->isOnLeft()) {
			// for left right
			swapColors(parent, grandparent);
		} else {
			leftRotate(parent);
			swapColors(x, grandparent);
		}
		// for left left and left right
		rightRotate(grandparent);
		} else {
		if (x->isOnLeft()) {
			// for right left
			rightRotate(parent);
			swapColors(x, grandparent);
		} else {
			swapColors(parent, grandparent);
		}

		// for right right and right left
		leftRotate(grandparent);
		}
	}
	}
}

// find node that do not have a left child
// in the subtree of the given node
Nod *successor(Nod *x) {
	Nod *temp = x;

	while (temp->left != NULL)
	temp = temp->left;

	return temp;
}

// find node that replaces a deleted node in BST
Nod *BSTreplace(Nod *x) {
	// when node have 2 children
	if (x->left != NULL && x->right != NULL)
	return successor(x->right);

	// when leaf
	if (x->left == NULL && x->right == NULL)
	return NULL;

	// when single child
	if (x->left != NULL)
	return x->left;
	else
	return x->right;
}

// deletes the given node
void deleteNode(Nod *v) {
	Nod *u = BSTreplace(v);

	// True when u and v are both black
	bool uvBlack = ((u == NULL || u->color == BLACK) && (v->color == BLACK));
	Nod *parent = v->parent;

	if (u == NULL) {
	// u is NULL therefore v is leaf
	if (v == root) {
		// v is root, making root null
		root = NULL;
	} else {
		if (uvBlack) {
		// u and v both black
		// v is leaf, fix double black at v
		fixDoubleBlack(v);
		} else {
		// u or v is red
		if (v->sibling() != NULL)
			// sibling is not null, make it red"
			v->sibling()->color = RED;
		}

		// delete v from the tree
		if (v->isOnLeft()) {
		parent->left = NULL;
		} else {
		parent->right = NULL;
		}
	}
	delete v;
	return;
	}

	if (v->left == NULL || v->right == NULL) {
	// v has 1 child
	if (v == root) {
		// v is root, assign the value of u to v, and delete u
		v->val = u->val;
		v->left = v->right = NULL;
		delete u;
	} else {
		// Detach v from tree and move u up
		if (v->isOnLeft()) {
		parent->left = u;
		} else {
		parent->right = u;
		}
		delete v;
		u->parent = parent;
		if (uvBlack) {
		// u and v both black, fix double black at u
		fixDoubleBlack(u);
		} else {
		// u or v red, color u black
		u->color = BLACK;
		}
	}
	return;
	}

	// v has 2 children, swap values with successor and recurse
	swapValues(u, v);
	deleteNode(u);
}

void fixDoubleBlack(Nod *x) {
	if (x == root)
	// Reached root
	return;

	Nod *sibling = x->sibling(), *parent = x->parent;
	if (sibling == NULL) {
	// No sibling, double black pushed up
	fixDoubleBlack(parent);
	} else {
	if (sibling->color == RED) {
		// Sibling red
		parent->color = RED;
		sibling->color = BLACK;
		if (sibling->isOnLeft()) {
		// left case
		rightRotate(parent);
		} else {
		// right case
		leftRotate(parent);
		}
		fixDoubleBlack(x);
	} else {
		// Sibling black
		if (sibling->hasRedChild()) {
		// at least 1 red children
		if (sibling->left != NULL && sibling->left->color == RED) {
			if (sibling->isOnLeft()) {
			// left left
			sibling->left->color = sibling->color;
			sibling->color = parent->color;
			rightRotate(parent);
			} else {
			// right left
			sibling->left->color = parent->color;
			rightRotate(sibling);
			leftRotate(parent);
			}
		} else {
			if (sibling->isOnLeft()) {
			// left right
			sibling->right->color = parent->color;
			leftRotate(sibling);
			rightRotate(parent);
			} else {
			// right right
			sibling->right->color = sibling->color;
			sibling->color = parent->color;
			leftRotate(parent);
			}
		}
		parent->color = BLACK;
		} else {
		// 2 black children
		sibling->color = RED;
		if (parent->color == BLACK)
			fixDoubleBlack(parent);
		else
			parent->color = BLACK;
		}
	}
	}
}

public:

RBTree() { root = NULL; }

Nod *getRoot() { return root; }


Nod *search(int n) {
	Nod *temp = root;
	while (temp != NULL) {
	if (n < temp->val) {
		if (temp->left == NULL)
		break;
		else
		temp = temp->left;
	} else if (n == temp->val) {
		break;
	} else {
		if (temp->right == NULL)
		break;
		else
		temp = temp->right;
	}
	}

	return temp;
}


int search01(int n) {
    Nod *temp = root;
    while (temp != NULL) {
        if (n < temp->val) {
            temp = temp->left;
        } else if (n > temp->val) {
            temp = temp->right;
        } else {
            return 1; 
        }
    }
    return 0; 
}

void insert(int n) {
	Nod *newNode = new Nod(n);
	if (root == NULL) {
	
	newNode->color = BLACK;
	root = newNode;
	} else {
	Nod *temp = search(n);

	if (temp->val == n) {
		
		return;
	}

	
	newNode->parent = temp;

	if (n < temp->val)
		temp->left = newNode;
	else
		temp->right = newNode;

	fixRedRed(newNode);
	}
}

void deleteByVal(int n) {
	if (root == NULL)
	
	return;

	Nod *v = search(n), *u;

	if (v->val != n) {
	return;
	}

	deleteNode(v);
}

 void intervalXY(Nod *node, int x, int y, vector<int> &result) {
        if (node == NULL) {
            return;
        }
        if (node->val > x) {
            intervalXY(node->left, x, y, result);
        }
        if (node->val >= x && node->val <= y) {
            result.push_back(node->val);
        }
        if (node->val < y) {
            intervalXY(node->right, x, y, result);
        }
    }

    void op6 (int x,int y)
    {
        vector<int> ans;
        intervalXY(root, x, y, ans);
        for(auto i : ans)
        {
            g<<i<<' ';
        }
        g<<'\n';
    }
    int maxlessthan (int n )
    {
        int ans= -maxim;
        Nod * curent= root;
        while(curent)
        {
            if(curent->val < n)
            {
                ans= max(ans,curent->val);
                curent= curent->right;
            }
            else 
            {
                curent= curent->left;
            }
          
            
        }
        return ans;
    }
    int minmorethan (int n )
    {
        int ans= maxim;
        Nod * curent= root;
        while(curent)
        {
            if(curent->val > n)
            {
                ans= min(ans,curent->val);
                curent= curent->left;
            }
            else
            {
                curent= curent->right;
            }
           
            
        }
        return ans;

}
};

int main() {
	nos();
RBTree tree;
int tests;
    f>>tests;
    while(tests--)
    {
        int op, n;
        f>>op>>n;
        switch(op)
        {
            case 1:
                tree.insert(n);
                break;
            case 2:
                tree.deleteByVal(n);
                break;
            case 3:
                g<<tree.search01(n)<<'\n';
                break;
            case 4:
               if(tree.search01(n))
                {g<<n<<'\n';
                }
                else
                { g<<tree.maxlessthan(n)<<'\n';
                   
                }
                break;
            case 5:
                if(tree.search01(n))
                {g<<n<<'\n';
                }
                else
                { g<<tree.minmorethan(n)<<'\n';
                   
                }

                break;

            case 6:
                {
                int x;
                f>>x;
                tree.op6(n,x);
                break;
                }

           
                
        }

    }


return 0;
}
