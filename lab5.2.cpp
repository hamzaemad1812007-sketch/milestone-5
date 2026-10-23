
#include <iostream>
#include <string>
using namespace std;

// Every hero has a different amount of gold.
class HeroNode {
public:
    int gold;
    string name;
    int height;
    int size;
    HeroNode* left;
    HeroNode* right;

    HeroNode(int g, string n) {
        gold = g;
        name = n;
        height = 1;
        size = 1;
        left = nullptr;
        right = nullptr;
    }
};

class Guild {
private:
    HeroNode* root;

    int getHeight(HeroNode* node) {
        if (node == nullptr) return 0;
        return node->height;
    }

    int getSize(HeroNode* node) {
        if (node == nullptr) return 0;
        return node->size;
    }

    int getBalance(HeroNode* node) {
        if (node == nullptr) return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    // Update both height and subtree size.
    void update(HeroNode* node) {
        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right);

        if (leftHeight > rightHeight)
            node->height = 1 + leftHeight;
        else
            node->height = 1 + rightHeight;

        node->size = 1 + getSize(node->left)
                       + getSize(node->right);
    }

    HeroNode* rotateRight(HeroNode* y) {
        HeroNode* x = y->left;
        HeroNode* middle = x->right;

        x->right = y;
        y->left = middle;

        update(y);
        update(x);

        return x;
    }

    HeroNode* rotateLeft(HeroNode* x) {
        HeroNode* y = x->right;
        HeroNode* middle = y->left;

        y->left = x;
        x->right = middle;

        update(x);
        update(y);

        return y;
    }

    HeroNode* rebalance(HeroNode* node) {
        if (node == nullptr) return nullptr;

        update(node);
        int balance = getBalance(node);

        if (balance > 1) {
            if (getBalance(node->left) < 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }

        if (balance < -1) {
            if (getBalance(node->right) > 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }

        return node;
    }

    HeroNode* addNode(HeroNode* node, int gold, string name) {
        if (node == nullptr)
            return new HeroNode(gold, name);

        if (gold < node->gold) {
            node->left = addNode(node->left, gold, name);
        }
        else if (gold > node->gold) {
            node->right = addNode(node->right, gold, name);
        }
        else {
            // Gold must be unique.
            return node;
        }

        return rebalance(node);
    }

    HeroNode* removeNode(HeroNode* node, int gold) {
        if (node == nullptr) return nullptr;

        if (gold < node->gold) {
            node->left = removeNode(node->left, gold);
        }
        else if (gold > node->gold) {
            node->right = removeNode(node->right, gold);
        }
        else {
            if (node->left == nullptr) {
                HeroNode* child = node->right;
                delete node;
                return child;
            }

            if (node->right == nullptr) {
                HeroNode* child = node->left;
                delete node;
                return child;
            }

            // Find the smallest gold amount in the right subtree.
            HeroNode* successor = node->right;

            while (successor->left != nullptr)
                successor = successor->left;

            node->gold = successor->gold;
            node->name = successor->name;

            node->right = removeNode(
                node->right, successor->gold
            );
        }

        return rebalance(node);
    }

    bool checkBalanced(HeroNode* node) {
        if (node == nullptr) return true;

        int balance = getBalance(node);

        if (balance > 1 || balance < -1)
            return false;

        return checkBalanced(node->left) &&
               checkBalanced(node->right);
    }

    void destroy(HeroNode* node) {
        if (node == nullptr) return;

        destroy(node->left);
        destroy(node->right);

        delete node;
    }

public:
    Guild() {
        root = nullptr;
    }

    ~Guild() {
        destroy(root);
    }

    void addHero(int gold, string name) {
        root = addNode(root, gold, name);
    }

    void removeHero(int gold) {
        root = removeNode(root, gold);
    }

    HeroNode* richest() {
        if (root == nullptr) return nullptr;

        HeroNode* current = root;

        while (current->right != nullptr)
            current = current->right;

        return current;
    }

    HeroNode* poorest() {
        if (root == nullptr) return nullptr;

        HeroNode* current = root;

        while (current->left != nullptr)
            current = current->left;

        return current;
    }

    // Count heroes with strictly more gold than the given amount.
    int countRicherThan(int gold) {
        int count = 0;
        HeroNode* current = root;

        while (current != nullptr) {
            if (current->gold > gold) {
                // Current node and all nodes in its right subtree
                // have more gold than the given amount.
                count += 1 + getSize(current->right);
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        return count;
    }

    // k = 1 means the richest hero.
    HeroNode* kthRichest(int k) {
        if (k < 1 || k > getSize(root))
            return nullptr;

        HeroNode* current = root;

        while (current != nullptr) {
            int rightSize = getSize(current->right);

            if (k == rightSize + 1) {
                return current;
            }
            else if (k <= rightSize) {
                current = current->right;
            }
            else {
                k -= rightSize + 1;
                current = current->left;
            }
        }

        return nullptr;
    }

    // Remove the old key and insert the new key.
    void changeGold(int oldGold, int newGold) {
        HeroNode* current = root;

        while (current != nullptr &&
               current->gold != oldGold) {
            if (oldGold < current->gold)
                current = current->left;
            else
                current = current->right;
        }

        if (current == nullptr) return;

        // Do not change the gold if another hero already has it.
        HeroNode* duplicate = root;

        while (duplicate != nullptr) {
            if (newGold == duplicate->gold) {
                if (newGold != oldGold)
                    return;
                break;
            }

            if (newGold < duplicate->gold)
                duplicate = duplicate->left;
            else
                duplicate = duplicate->right;
        }

        string name = current->name;

        removeHero(oldGold);
        addHero(newGold, name);
    }

    int height() {
        return getHeight(root);
    }

    int size() {
        return getSize(root);
    }

    bool isBalanced() {
        return checkBalanced(root);
    }
};

int main() {
    Guild guild;

    guild.addHero(500, "Aria");
    guild.addHero(120, "Borin");
    guild.addHero(900, "Cael");
    guild.addHero(300, "Dara");
    guild.addHero(750, "Elric");
    guild.addHero(50, "Fenna");
    guild.addHero(640, "Gorm");
    guild.addHero(220, "Hilda");

    cout << "Richest: "
         << guild.richest()->name << " ("
         << guild.richest()->gold << ")" << endl;

    cout << "Poorest: "
         << guild.poorest()->name << " ("
         << guild.poorest()->gold << ")" << endl;

    cout << "Heroes richer than 300 gold: "
         << guild.countRicherThan(300) << endl;

    cout << "Heroes richer than 1000 gold: "
         << guild.countRicherThan(1000) << endl;

    cout << "3rd richest: "
         << guild.kthRichest(3)->name << " ("
         << guild.kthRichest(3)->gold << ")" << endl;

    cout << endl
         << "Fenna finds a treasure: 50 -> 1000 gold"
         << endl;

    guild.changeGold(50, 1000);

    cout << "Richest: "
         << guild.richest()->name << " ("
         << guild.richest()->gold << ")" << endl;

    cout << "Poorest: "
         << guild.poorest()->name << " ("
         << guild.poorest()->gold << ")" << endl;

    cout << "Heroes richer than 300 gold: "
         << guild.countRicherThan(300) << endl;

    cout << "Heroes: " << guild.size()
         << ", height: " << guild.height()
         << ", balanced: "
         << (guild.isBalanced() ? "yes" : "NO")
         << endl;

    return 0;
}
