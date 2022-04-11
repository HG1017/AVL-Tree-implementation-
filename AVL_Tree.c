#include<stdio.h>
#include<stdlib.h>
#define endl printf("\n")

struct avlnode
// The AVL tree node contains members: an integer value, the height of the node, pointers to left and right children
{
    int val;
    int h;
    struct avlnode* left;
    struct avlnode* right;
};

struct avlnode* root; // This is the pointer to the root of the AVL tree

struct avlnode* getnode(int x)
// Function for creating a new node for the AVL tree
{
    struct avlnode* t;
    t = (struct avlnode*)malloc(sizeof(struct avlnode));

    if (t == NULL)
    {
        printf("The device could not allocate memory for a new node");
        endl;
    }
    else
    {
        t->val = x;
        t->h = 1; // It is assumed that one single node has height 1, that is why the final height of any node is printed as (result of function)-1
        t->left = NULL;
        t->right = NULL;
    }
    return t;
}

int get_height(struct avlnode* one)
{
    if (one!=NULL)
    {
        return one->h; // Returns the height of node
    }
    else
    {
        return 0;
    }
}

int balance_factor(struct avlnode* one)
// Returns the difference between the heights of the left and right sub-trees of a node
{
    if(one!=NULL)
    {
        return get_height(one->left) - get_height(one->right);
    }
    else
    {
        return 0;
    }
}

struct avlnode* turn_left(struct avlnode* one)
// This function helps in the left rotation in order to maintain the balance condition in the AVL tree
{
    struct avlnode* temp1;
    struct avlnode* temp2;

    temp1 = one->right; // New head node after left rotation will be the right child of the node where balance condition is violated

    temp2 = temp1->left; // For using this further since we won't be able to access it later for making the pointer changes

    temp1->left = one; // Performing the rotation, this node now becomes the new head to maintain balance condition
    one->right = temp2;

    // Updating the height of the previous head node
    if(get_height(one->left)>get_height(one->right))
    {
        one->h = get_height(one->left)+1;
    }
    else
    {
        one->h = get_height(one->right)+1;
    }

    // Updating the height of the new node accordingly
    if(get_height(temp1->left)>get_height(temp1->right))
    {
        temp1->h = get_height(temp1->left)+1;
    }
    else
    {
        temp1->h = get_height(temp1->right)+1;
    }

    return temp1; // Returns the new node
}

struct avlnode* turn_right(struct avlnode* one)
// This function helps in the right rotation in order to maintain the balance condition in the AVL tree
{
    struct avlnode* temp1;
    struct avlnode* temp2;

    temp1 = one->left; // New head node after right rotation will be the left child of the node where balance condition is violated

    temp2 = temp1->right; // For using this further since we won't be able to access it later for making the pointer changes

    temp1->right = one; // Performing the rotation, this node now becomes the new head to maintain balance condition
    one->left = temp2;

    // Updating the height of the previous head node
    if(get_height(one->left)>get_height(one->right))
    {
        one->h = get_height(one->left)+1;
    }
    else
    {
        one->h = get_height(one->right)+1;
    }

    // Updating the height of the new node accordingly
    if(get_height(temp1->left)>get_height(temp1->right))
    {
        temp1->h = get_height(temp1->left)+1;
    }
    else
    {
        temp1->h = get_height(temp1->right)+1;
    }

    return temp1; // Returns the new node
}

struct avlnode* search(struct avlnode* one, int x)
// This function searches for a value in the AVL tree and returns a pointer to the node whose value is same
{
    if(one == NULL)
    {
        return one;
    }
    if (one->val == x)
    {
        return one;
    }
    if (one->val > x)
    {
        return search(one->left, x); // Search in the left sub-tree if value is less than the tree node value
    }
    return search(one->right, x); // Otherwise search in the right sub-tree
}

void preorder(struct avlnode* one)
// This function prints the preorder traversal sequence for the AVL tree
{
    if (one != NULL)
    {
        printf("%d ",one->val);
        preorder(one->left);
        preorder(one->right);
    }
}

void inorder(struct avlnode* one)
// This function prints the inorder traversal sequence for the AVL tree
{
    if (one != NULL)
    {
        inorder(one->left);
        printf("%d ",one->val);
        inorder(one->right);
    }
}

struct avlnode* insert(struct avlnode* one, int x)
// This function inserts a node into the AVL tree, first normally and then balances the tree if required at any node
{
    if (one == NULL)
    {
        return getnode(x); // If the tree is empty, create a new node
    }

// It is assumed that the user enters distinct elements only while calling the insert function (as given in instructions)
        if(x > one->val)
        {
            one->right = insert(one->right, x); // Add to the right sub-tree is value is greater than the tree node value
        }
        else
        {
            one->left = insert(one->left, x); // Otherwise add to the left sub-tree
        }

        // Updating the height of the tree nodes as the values are added
        if(get_height(one->left)>get_height(one->right))
        {
            one->h = get_height(one->left)+1;
        }
        else
        {
            one->h = get_height(one->right)+1;
        }

        int bf = balance_factor(one); // Calculate the balance factor, if it is within the range of {-1, 0, 1}, proceed without doing anything

        // Tree is not balanced
        if(bf < -1) // Unbalance due to bigger height of right sub-tree
        {
            if(one->right->val < x) //RR configuration, needs to be turned left
            {
                return turn_left(one);
            }
            if(one->right->val > x) //RL configuration, we will first turn it to RR and then rotate it left
            {
                one->right = turn_right(one->right); //RL got converted to RR

                return turn_left(one); //Left rotation to get the balanced configuration
            }
        }
        if(bf > 1) // Unbalance due to bigger height of left sub-tree
        {
            if(one->left->val > x) //LL configuration, needs to be turned right
            {
                return turn_right(one);
            }
            if(one->left->val < x) //LR configuration, we will first turn it to LL and then rotate it right
            {
                one->left = turn_left(one->left); //LR got converted to LL

                return turn_right(one); //Right rotation to get the balanced configuration
            }
        }

        return one;
}

int main()
{
    root = NULL;

    // For creating an AVL tree with elements 0,1,2,3,...,24 inserted in this order (as given in instructions)
    for (int i = 0; i <= 24; i++)
    {
        root = insert(root, i);
    }

    struct avlnode* temp;
    char c;
    int n;

    printf("i) Insert :");
    endl;
    printf("s) Search :");
    endl;
    printf("p) Print Preorder and Inorder in Separate lines:");
    endl;
    printf("q) Quit:");
    endl;
    endl;
    printf("Enter Your Choice: ");

    while(1)
    {
        scanf("%c", &c);

        if (c == 'q')
        {
            break;
        }
        else
        {
        switch(c)
        {

        case 'i':
            printf("Enter a data to insert: ");
            scanf("%d", &n);
            root = insert(root, n);
            endl;
            printf("i) Insert :");
            endl;
            printf("s) Search :");
            endl;
            printf("p) Print Preorder and Inorder in Separate lines:");
            endl;
            printf("q) Quit:");
            endl;
            endl;
            printf("Enter Your Choice: ");
            break;


        case 's':
            printf("Enter data you want to search: ");
            scanf("%d", &n);
            temp = search(root, n);
            if(temp == NULL)
            {
                printf("Not present");
                endl;
            }
            else
            {
                printf("present");
                endl;
                printf("Balance factor of %d is %d", n, balance_factor(temp));
                endl;
                printf("Height of %d in the AVL tree is %d", n, get_height(temp)-1); // -1 is done since the function is such that a single node is at level 1.
                endl;
            }
            endl;
            printf("i) Insert :");
            endl;
            printf("s) Search :");
            endl;
            printf("p) Print Preorder and Inorder in Separate lines:");
            endl;
            printf("q) Quit:");
            endl;
            endl;
            printf("Enter Your Choice: ");
            break;


        case 'p':
            printf("Preorder sequence:");
            endl;
            preorder(root);
            endl;
            printf("Inorder sequence:");
            endl;
            inorder(root);
            endl;
            endl;
            printf("i) Insert :");
            endl;
            printf("s) Search :");
            endl;
            printf("p) Print Preorder and Inorder in Separate lines:");
            endl;
            printf("q) Quit:");
            endl;
            endl;
            printf("Enter Your Choice: ");
            break;

        default:
            break;
        }
        }
    }

    return 0;
}
