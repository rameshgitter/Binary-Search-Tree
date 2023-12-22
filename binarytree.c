#include <stdio.h>
#include <stdlib.h>

// Define the structure for the nodes of the BST
typedef struct node {
    int data;
    struct node *lchild;
    struct node *rchild;
    struct node *parent;
} treenode;

// Function to check if the BST is empty
int isEmpty(treenode *t) {
    return t == NULL;
}

// Function to insert a node with a given value into the BST
treenode *insertNode(treenode *t, int val) {
    // If the tree is empty, create a new node as the root
    if (t == NULL) {
        treenode *newNode = (treenode *)malloc(sizeof(treenode));
        newNode->data = val;
        newNode->lchild = NULL;
        newNode->rchild = NULL;
        newNode->parent = NULL;
        return newNode;
    }

    // Otherwise, recursively insert the node in the appropriate subtree
    if (val < t->data) {
        t->lchild = insertNode(t->lchild, val);
        t->lchild->parent = t;
    } else if (val > t->data) {
        t->rchild = insertNode(t->rchild, val);
        t->rchild->parent = t;
    }

    return t;
}

// Function to count the number of nodes in the BST
int getCount(treenode *t) {
    if (t == NULL) {
        return 0;
    }
    return 1 + getCount(t->lchild) + getCount(t->rchild);
}

// Function to perform inorder traversal and print the nodes
int inorder(treenode *t) {
    if (t == NULL) {
        return 0;
    }
    inorder(t->lchild);
    printf("%d ", t->data);
    inorder(t->rchild);
    return 1;
}

// Function to perform preorder traversal and print the nodes
int preorder(treenode *t) {
    if (t == NULL) {
        return 0;
    }
    printf("%d ", t->data);
    preorder(t->lchild);
    preorder(t->rchild);
    return 1;
}

// Function to perform postorder traversal and print the nodes
int postorder(treenode *t) {
    if (t == NULL) {
        return 0;
    }
    postorder(t->lchild);
    postorder(t->rchild);
    printf("%d ", t->data);
    return 1;
}

// Function to find the height of the BST
int height(treenode *t) {
    if (t == NULL) {
        return 0;
    }
    int left_height = height(t->lchild);
    int right_height = height(t->rchild);
    return (left_height > right_height) ? (left_height + 1) : (right_height + 1);
}

// Function to find the maximum key in the BST
int max(treenode *t) {
    if (t == NULL) {
        fprintf(stderr, "Tree is empty.\n");
        exit(1);
    }
    while (t->rchild != NULL) {
        t = t->rchild;
    }
    return t->data;
}

// Function to find the minimum key in the BST
int min(treenode *t) {
    if (t == NULL) {
        fprintf(stderr, "Tree is empty.\n");
        exit(1);
    }
    while (t->lchild != NULL) {
        t = t->lchild;
    }
    return t->data;
}

// Function to check if two trees are equal
int equal(treenode *t1, treenode *t2) {
    if (t1 == NULL && t2 == NULL) {
        return 1;
    }
    if (t1 == NULL || t2 == NULL) {
        return 0;
    }
    return (t1->data == t2->data) && equal(t1->lchild, t2->lchild) && equal(t1->rchild, t2->rchild);
}

// Function to find the inorder successor of a node with a given key
treenode *insucc(treenode *t, int key) {
    if (t == NULL) {
        return NULL;
    }
    treenode *current = t;
    treenode *successor = NULL;
    while (current != NULL) {
        if (key < current->data) {
            successor = current;
            current = current->lchild;
        } else if (key > current->data) {
            current = current->rchild;
        } else {
            if (current->rchild != NULL) {
                successor = current->rchild;
                while (successor->lchild != NULL) {
                    successor = successor->lchild;
                }
            }
            break;
        }
    }
    return successor;
}

// Function to delete a node with a given value from the BST
int deleteNode(treenode *t, int val) {
    // Find the node to delete
    treenode *current = t;
    treenode *parent = NULL;
    while (current != NULL && current->data != val) {
        parent = current;
        if (val < current->data) {
            current = current->lchild;
        } else {
            current = current->rchild;
        }
    }

    // If the node with the value is not found, return False
    if (current == NULL) {
        return 0;
    }

    // Case 1: Node has no children
    if (current->lchild == NULL && current->rchild == NULL) {
        if (parent != NULL) {
            if (parent->lchild == current) {
                parent->lchild = NULL;
            } else {
                parent->rchild = NULL;
            }
            free(current);
        } else {
            // If the node to delete is the root
            free(current);
            t = NULL;
        }
    }
    // Case 2: Node has one child
    else if (current->lchild == NULL || current->rchild == NULL) {
        treenode *child = (current->lchild != NULL) ? current->lchild : current->rchild;
        if (parent != NULL) {
            if (parent->lchild == current) {
                parent->lchild = child;
            } else {
                parent->rchild = child;
            }
            free(current);
        } else {
            // If the node to delete is the root
            free(current);
            t = child;
        }
    }
    // Case 3: Node has two children
    else {
        treenode *successor = insucc(t, val);
        int temp = successor->data;
        deleteNode(t, temp);
        current->data = temp;
    }
    return 1;
}

// Function to create a new node
treenode *createNode(int val) {
    treenode *newNode = (treenode *)malloc(sizeof(treenode));
    newNode->data = val;
    newNode->lchild = NULL;
    newNode->rchild = NULL;
    return newNode;
}

// Helper function to construct a binary tree from inorder and preorder traversals
treenode *buildTreeFromInPre(int *inorder, int *preorder, int inStart, int inEnd, int *preIndex) {
    if (inStart > inEnd) {
        return NULL;
    }

    // Pick the current root node from preorder traversal
    treenode *root = createNode(preorder[(*preIndex)++]);

    // Find the index of this node in inorder traversal
    int inIndex;
    for (inIndex = inStart; inIndex <= inEnd; inIndex++) {
        if (inorder[inIndex] == root->data) {
            break;
        }
    }

    // Recursively construct the left and right subtrees
    root->lchild = buildTreeFromInPre(inorder, preorder, inStart, inIndex - 1, preIndex);
    root->rchild = buildTreeFromInPre(inorder, preorder, inIndex + 1, inEnd, preIndex);

    return root;
}

// Function to create a binary tree from inorder and preorder traversals
treenode *getTreeInPre(int inorder[], int asize, int preorder[], int bsize) {
    int preIndex = 0;
    return buildTreeFromInPre(inorder, preorder, 0, asize - 1, &preIndex);
}

// Helper function to construct a binary tree from inorder and postorder traversals
treenode *buildTreeFromInPost(int *inorder, int *postorder, int inStart, int inEnd, int *postIndex) {
    if (inStart > inEnd) {
        return NULL;
    }

    // Pick the current root node from postorder traversal
    treenode *root = createNode(postorder[(*postIndex)--]);

    // Find the index of this node in inorder traversal
    int inIndex;
    for (inIndex = inStart; inIndex <= inEnd; inIndex++) {
        if (inorder[inIndex] == root->data) {
            break;
        }
    }

    // Recursively construct the right and left subtrees
    root->rchild = buildTreeFromInPost(inorder, postorder, inIndex + 1, inEnd, postIndex);
    root->lchild = buildTreeFromInPost(inorder, postorder, inStart, inIndex - 1, postIndex);

    return root;
}

// Function to create a binary tree from inorder and postorder traversals
treenode *getTreeInPost(int inorder[], int asize, int postorder[], int bsize) {
    int postIndex = bsize - 1;
    return buildTreeFromInPost(inorder, postorder, 0, asize - 1, &postIndex);
}

// Function to perform inorder traversal and print the nodes
void inorderTraversal(treenode *root) {
    if (root == NULL) {
        return;
    }
    inorderTraversal(root->lchild);
    printf("%d ", root->data);
    inorderTraversal(root->rchild);
}


int main() {
    int choice;
    treenode* t=NULL;
    treenode *t1=NULL;
    do{
	printf("\n");
	printf("1. checking for empty treenode\n");
        printf("2. inserting a node\n");
        printf("3. count number of nodes\n");
        printf("4. printing inorder traversal\n");
        printf("5. printing preorder traversal\n");
        printf("6. printing postorder traversal\n");
        printf("7. height of tree\n");
        printf("8. maximum key\n");
        printf("9. minimum key\n");
        printf("10. equal tree\n");
        printf("11. insuccessor pointer to an inorder\n");
        printf("12. deteting a node\n");
	printf("13. Creation of Binary trees from their traversals\n");
	printf("enter 0 to exit the program\n");
        printf("enter your choice:\n");
	scanf("%d",&choice);
	printf("\n");
	switch(choice){
	    case 1:{
	            int x=isEmpty(t);
		    if(x==1) printf("tree is empty\n");
		    else printf("tree is non empty\n");
		    break;
		   }
	    case 2:{
		    int n,val,m;
		    printf("Enter how many elements you want to insert: ");
		    scanf("%d",&n);
		    for(int i=1;i<=n;i++){
                        printf("enter the value you want to insert: ");
                        scanf("%d",&val);
                        t=insertNode(t,val);
		    }
		    printf("we will create new tree for purpose of comparing it with another one");
		    printf("How many elements you want to insert in this tree: ");
		    scanf("%d",&m);
		    for(int i=1;i<=m;i++){
                        printf("enter the value you want to insert: ");
                        scanf("%d",&val);
                        t1=insertNode(t1,val);
                    }

		    break;
		   }
	    case 3:{
		    int x=getCount(t);
		    printf("no. of nodes=%d",x);
		    break;
		   }
	    case 4:{
		    inorder(t);
		    break;
		   }
	    case 5:{
		    preorder(t);
		    break;
		   }
	    case 6:{
		    postorder(t);
		    break;
		   }
	    case 7:{
		    int x=height(t);
		    printf("height=%d",x);
		    break;
		   }
	    case 8:{
		    int x=max(t);
		    printf("maximum element=%d",x);
		    break;
		   }
	    case 9:{
		    int x=min(t);
                    printf("minimum element=%d",x);
                    break;
		   }
	    case 10:{
		    int x=equal(t,t1);
		    if(x==1) printf("both trees are equal\n");
		    else printf("both trees are not equal\n");
		    break;
		    }
	    case 11:{
		    int key;
		    printf("enter the key value you need: ");
		    scanf("%d",&key);
		    t=insucc(t,key);
		    break;
		    }
	   case 12:{
		   int val;
		   printf("enter element you want to delete: ");
		   scanf("%d",&val);
		   deleteNode(t,val);
		   break;
		   }
	   case 13:{
		   int inorder[] = {4, 2, 5, 1, 6, 3};
    	       	   int preorder[] = {1, 2, 4, 5, 3, 6};
  	           int postorder[] = {4, 5, 2, 6, 3, 1};
	           int n = sizeof(inorder) / sizeof(inorder[0]);
		   printf("Creating tree from inorder and preorder traversals:\n");
                   treenode *inPreRoot = getTreeInPre(inorder, n, preorder, n);
                   inorderTraversal(inPreRoot);
                   printf("\n");
                   printf("Creating tree from inorder and postorder traversals:\n");
                   treenode *inPostRoot = getTreeInPost(inorder, n, postorder, n);
                   inorderTraversal(inPostRoot);
                   printf("\n");
                   return 0;
		   break;
                   }
        }
    } while(choice!=0);
    return 0;
}
