#include <iostream>

using namespace std;

struct Node {
  int key;
  Node* left;
  Node* right;
  int height;

  // constructor for the node structure
  Node(int val) : key(val), left(nullptr), right(nullptr), height(1) {}
};

int getHeight(Node* node) {
  return node ? node -> height : 0;
}

int getBalance(Node* node) {
  return node ? getHeight(node -> left) - getHeight(node -> right) : 0;
}

Node* rightRotate(Node* y) {
  Node* x = y -> left;
  y->left = x -> right;
  x -> right = y;

  y -> height = max(getHeight(y -> left), getHeight(y -> right)) + 1;
  x -> height = max(getHeight(x -> left), getHeight(x -> right)) + 1;

  return x;
}

Node* leftRotate(Node* x) {
  Node* y = x -> right;
  x->right = y -> left;
  y -> left = x;

  x -> height = max(getHeight(x -> left), getHeight(x -> right)) + 1;
  y -> height = max(getHeight(y -> left), getHeight(y -> right)) + 1;

  return y;
}

Node* insertNode(Node* root, int key) {
  if (!root) return new Node(key);
  else if (key < root -> key)
    root -> left = insertNode(root -> left, key);
  else
    root -> right = insertNode(root -> right, key);

  // height update
  root -> height = 1 + max(getHeight(root -> left), getHeight(root -> right));
  // balance factor update
  int balance = getBalance(root);

  if (balance > 1 && key < root -> left -> key)
    return rightRotate(root);
  else if (balance > 1 && key > root -> left -> key) {
    root -> left = leftRotate(root -> left);
    return rightRotate(root);
  } else if (balance < -1 && key > root -> right -> key)
    return leftRotate(root);
  if (balance < -1 && key < root -> right -> key) {
    root -> right = rightRotate(root -> right);
    return leftRotate(root);
  }

  return root;
}

Node* minValueNode(Node* node) {
  // traverse subtree to find minValue node
  Node* current = node;

  while (current -> left != nullptr)
    current = current -> left;
  return current;
}

Node *deleteNode(Node *root, int key){
  if (!root) return root;
  else if (key < root -> key){
    root -> left = deleteNode(root->left, key);
  }
  else if (key > root -> key){
    root -> right = deleteNode(root -> right, key);
  }
  else {
    Node *temp;
    if (!root -> left){
      temp = root -> right;
      root = NULL;
      return temp;
    }
    else if (! root -> right){
      temp = root -> left;
      root = NULL;
      return temp;
    }
    // find inorder successor
    temp = minValueNode(root -> right);
    root -> key = temp ->key;
    root -> right = deleteNode(root -> right, temp -> key);
  }

  root -> height = 1 + max(getHeight(root -> left), getHeight(root -> right));

  int balance = getBalance(root);

  // right rotate case
  if (balance > 1 && getBalance(root -> left) >= 0)
    return rightRotate(root);
  else if (balance < -1 && getBalance(root -> right) <= 0)
    return leftRotate(root);
  else if (balance > 1 && getBalance(root -> left) < 0) {
    root -> left = leftRotate(root -> left);
    return rightRotate(root);
  } else if (balance < -1 && getBalance(root -> right) > 0) {
    root -> right = rightRotate(root -> right);
    return leftRotate(root);
  }

  return root;
}

Node* modify(Node* root, int oldKey, int newKey) {
  root = deleteNode(root, oldKey);
  root = insertNode(root, newKey);
  return root;
}

int size(Node* root){
  if (root == NULL) return 0;
  else {
    return(size(root -> left) + 1 + size(root -> right));
  }
}

Node* kthMinFinder(Node* root, int k, int& index) {
  if (root == NULL) return NULL;

  Node* kth_node = kthMinFinder(root->left, k, index);
  if (kth_node)
    return kth_node;
  
  index++;
  
  if (index == k) return root;
  return kthMinFinder(root->right, k, index);
}

int kthMin(Node* root, int k) {
  int index = 0;
  Node *res = kthMinFinder(root, k, index);
  if (res == NULL) return -1;
  return res -> key;
}

int main() {
  Node* root = nullptr;
  char character;
  int x, y, k;

  while (true) {
    cin >> character;

    if (character == 'A') {
      cin >> x;
      root = insertNode(root, x);
    } else if (character == 'D') {
      cin >> x;
      root = deleteNode(root, x);
    } else if (character == 'C') {
      cin >> x >> y;
      root = modify(root, x, y);
    } else if (character == 'M') {
      cin >> k;
      cout << kthMin(root, k) << endl;
    } else if (character == 'E') {
      break;
    }
  }

  return 0;
}
