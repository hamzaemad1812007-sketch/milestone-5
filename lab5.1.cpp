
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Node {
public:
    int key;
    int height;
    Node* left;
    Node* right;

    Node(int k) {
        key = k;
        height = 1;
        left = nullptr;
        right = nullptr;
    }
};

class AVLTree {
private:
    Node* root;
    string lastCase;

    int getHeight(Node* node) {
        if (node == nullptr) return 0;
        return node->height;
    }

    // Positive = left-heavy, negative = right-heavy
    int getBalance(Node* node) {
        if (node == nullptr) return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    void updateHeight(Node* node) {
        if (node == nullptr) return;

        node->height = 1 + max(
            getHeight(node->left),
            getHeight(node->right)
        );
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* middle = x->right;

        x->right = y;
        y->left = middle;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* middle = y->left;

        y->left = x;
        x->right = middle;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    Node* rebalance(Node* node) {
        updateHeight(node);

        int balance = getBalance(node);

        // Left-heavy
        if (balance > 1) {
            // LL case
            if (getBalance(node->left) >= 0) {
                lastCase = "LL";
                return rotateRight(node);
            }

            // LR case
            lastCase = "LR";
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Right-heavy
        if (balance < -1) {
            // RR case
            if (getBalance(node->right) <= 0) {
                lastCase = "RR";
                return rotateLeft(node);
            }

            // RL case
            lastCase = "RL";
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    Node* insertNode(Node* node, int key) {
        if (node == nullptr) return new Node(key);

        if (key < node->key)
            node->left = insertNode(node->left, key);
        else if (key > node->key)
            node->right = insertNode(node->right, key);
        else
            return node;

        return rebalance(node);
    }

    Node* removeNode(Node* node, int key) {
        if (node == nullptr) return nullptr;

        if (key < node->key) {
            node->left = removeNode(node->left, key);
        }
        else if (key > node->key) {
            node->right = removeNode(node->right, key);
        }
        else {
            // Node has zero or one child
            if (node->left == nullptr) {
                Node* child = node->right;
                delete node;
                return child;
            }

            if (node->right == nullptr) {
                Node* child = node->left;
                delete node;
                return child;
            }

            // Node has two children:
            // Find the smallest key in the right subtree
            Node* successor = node->right;

            while (successor->left != nullptr)
                successor = successor->left;

            node->key = successor->key;
            node->right = removeNode(node->right, successor->key);
        }

        return rebalance(node);
    }

    void inorderPrint(Node* node) {
        if (node == nullptr) return;

        inorderPrint(node->left);
        cout << node->key << " ";
        inorderPrint(node->right);
    }

    bool checkBalanced(Node* node) {
        if (node == nullptr) return true;

        int balance = getBalance(node);

        if (balance < -1 || balance > 1)
            return false;

        return checkBalanced(node->left) &&
               checkBalanced(node->right);
    }

    void destroy(Node* node) {
        if (node == nullptr) return;

        destroy(node->left);
        destroy(node->right);

        delete node;
    }

public:
    AVLTree() {
        root = nullptr;
        lastCase = "none";
    }

    ~AVLTree() {
        destroy(root);
    }

    void insert(int key) {
        root = insertNode(root, key);
    }

    void remove(int key) {
        root = removeNode(root, key);
    }

    bool contains(int key) {
        Node* current = root;

        while (current != nullptr) {
            if (key == current->key) return true;

            if (key < current->key)
                current = current->left;
            else
                current = current->right;
        }

        return false;
    }

    void inorder() {
        inorderPrint(root);
        cout << endl;
    }

    bool isBalanced() {
        return checkBalanced(root);
    }

    int height() {
        return getHeight(root);
    }

    int rootKey() {
        if (root == nullptr) return -1;
        return root->key;
    }

    string getLastCase() {
        return lastCase;
    }
};

void runCase(string expected, vector<int> keys) {
    AVLTree tree;

    cout << "Insert ";

    for (int i = 0; i < (int)keys.size(); i++) {
        tree.insert(keys[i]);
        cout << keys[i] << " ";
    }

    cout << "-> case " << tree.getLastCase()
         << " (expected " << expected << ")"
         << ", root = " << tree.rootKey()
         << ", balanced = "
         << (tree.isBalanced() ? "yes" : "NO")
         << endl;
}

int main() {
    runCase("RR", {10, 20, 30});
    runCase("LL", {30, 20, 10});
    runCase("LR", {30, 10, 20});
    runCase("RL", {10, 30, 20});

    cout << endl << "--- deletion test ---" << endl;

    AVLTree tree;

    for (int i = 1; i <= 20; i++)
        tree.insert(i);

    cout << "Height after inserting 1..20: "
         << tree.height() << endl;

    int toRemove[] = {1, 2, 3, 4, 5, 10, 8, 16, 7};

    for (int i = 0; i < 9; i++) {
        tree.remove(toRemove[i]);

        cout << "remove " << toRemove[i]
             << " -> balanced = "
             << (tree.isBalanced() ? "yes" : "NO")
             << ", contains = "
             << (tree.contains(toRemove[i]) ? "yes" : "no")
             << endl;
    }

    cout << "Inorder: ";
    tree.inorder();

    return 0;
}
