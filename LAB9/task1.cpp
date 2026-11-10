
#include <iostream>
#include <stack>
using namespace std;

struct Node {
    int key;
    Node *left;
    Node *right;
    Node(int k) : key(k), left(NULL), right(NULL) {}
};

class BST {
private:
    Node *root;

    Node *insertRec(Node *p, int key);
    void inorderRec(Node *p) const;
    void rangeRec(Node *p, int low, int high, int &comparisons) const;
    void destroy(Node *p);

public:
    BST() : root(NULL) {}
    ~BST() { destroy(root); }

    void insert(int key) { root = insertRec(root, key); }
    void inorder() const { inorderRec(root); cout << endl; }
    bool predecessorSuccessor(int key, Node *&pred, Node *&succ) const;
    int rangeQueryRecursive(int low, int high) const;
    int rangeQueryIterative(int low, int high) const;
};

Node *BST::insertRec(Node *p, int key) {
    if (p == NULL) return new Node(key);
    if (key < p->key)      p->left  = insertRec(p->left, key);
    else if (key > p->key) p->right = insertRec(p->right, key);
    return p;
}

void BST::inorderRec(Node *p) const {
    if (p != NULL) {
        inorderRec(p->left);
        cout << p->key << " ";
        inorderRec(p->right);
    }
}

void BST::destroy(Node *p) {
    if (p != NULL) {
        destroy(p->left);
        destroy(p->right);
        delete p;
    }
}

bool BST::predecessorSuccessor(int key, Node *&pred, Node *&succ) const {
    pred = NULL;
    succ = NULL;
    Node *cur = root;
    while (cur != NULL) {
        if (key < cur->key) {
            succ = cur;
            cur = cur->left;
        } else if (key > cur->key) {
            pred = cur;
            cur = cur->right;
        } else {
            if (cur->left != NULL) {
                Node *t = cur->left;
                while (t->right != NULL) t = t->right;
                pred = t;
            }
            if (cur->right != NULL) {
                Node *t = cur->right;
                while (t->left != NULL) t = t->left;
                succ = t;
            }
            return true;
        }
    }
    return false;
}

void BST::rangeRec(Node *p, int low, int high, int &comparisons) const {
    if (p == NULL) return;

    comparisons++;
    if (low < p->key)
        rangeRec(p->left, low, high, comparisons);

    comparisons++;
    if (low <= p->key) {
        comparisons++;
        if (p->key <= high)
            cout << p->key << " ";
    }

    comparisons++;
    if (p->key < high)
        rangeRec(p->right, low, high, comparisons);
}

int BST::rangeQueryRecursive(int low, int high) const {
    int comparisons = 0;
    rangeRec(root, low, high, comparisons);
    cout << endl;
    return comparisons;
}

int BST::rangeQueryIterative(int low, int high) const {
    int comparisons = 0;
    stack<Node *> st;
    Node *cur = root;

    while (cur != NULL || !st.empty()) {
        while (cur != NULL) {
            comparisons++;
            if (cur->key < low) {
                cur = cur->right;
            } else {
                st.push(cur);
                cur = cur->left;
            }
        }
        cur = st.top();
        st.pop();

        comparisons++;
        if (cur->key > high) break;

        cout << cur->key << " ";
        cur = cur->right;
    }
    cout << endl;
    return comparisons;
}

int main() {
    // 1. Construct the BST from the given sequence
    int values[] = {50, 30, 70, 20, 40, 60, 80, 35, 45, 65, 85};
    int n = sizeof(values) / sizeof(values[0]);
    BST tree;
    for (int i = 0; i < n; i++)
        tree.insert(values[i]);

    // 5. Display the inorder traversal
    cout << "Inorder traversal: ";
    tree.inorder();

    // 2. Predecessor and successor of a key
    int key = 40;
    Node *pred, *succ;
    bool found = tree.predecessorSuccessor(key, pred, succ);
    cout << "\nKey " << key << (found ? " found in the BST" : " not found in the BST") << endl;
    cout << "Inorder predecessor of " << key << ": ";
    if (pred != NULL) cout << pred->key << endl; else cout << "none" << endl;
    cout << "Inorder successor of " << key << ": ";
    if (succ != NULL) cout << succ->key << endl; else cout << "none" << endl;

    // 3 and 4. Range query, recursive and iterative, with comparison counts
    int low = 30, high = 70;
    cout << "\nRange query [" << low << ", " << high << "]" << endl;

    cout << "Recursive result: ";
    int recComparisons = tree.rangeQueryRecursive(low, high);
    cout << "Iterative result: ";
    int itComparisons = tree.rangeQueryIterative(low, high);

    cout << "\nComparative analysis" << endl;
    cout << "Recursive comparisons: " << recComparisons << endl;
    cout << "Iterative comparisons: " << itComparisons << endl;
    cout << "Both versions visit only the subtrees that can hold keys in the range, so both run in O(h + k)." << endl;
    cout << "The recursive version uses the call stack, and the iterative version uses an explicit stack (O(h) space)." << endl;
    if (itComparisons < recComparisons)
        cout << "The iterative version made fewer comparisons, because it stops at the first key above the upper bound." << endl;
    else if (itComparisons > recComparisons)
        cout << "The recursive version made fewer comparisons for this range." << endl;
    else
        cout << "Both versions made the same number of comparisons for this range." << endl;

    return 0;
}