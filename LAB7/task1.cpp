// Lab 8 - Task 1: Binary Tree Representation and Operations
// Zarrar (543233)

#include <iostream>
#include <string>
#include <stack>
using namespace std;

class Node {
public:
    char data;
    Node* left;
    Node* right;

    Node(char c) {
        data = c;
        left = NULL;
        right = NULL;
    }
};

bool isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// ---------------- 1. Build the tree from the string ----------------
// i is passed by reference so every recursive call moves along the same string
Node* buildTree(const string &s, int &i) {
    if (i >= (int)s.length() || s[i] == ',' || s[i] == ')')
        return NULL;                        // nothing here = empty child

    Node* root = new Node(s[i]);
    i++;

    if (i < (int)s.length() && s[i] == '(') {
        i++;                                // skip (
        root->left = buildTree(s, i);
        i++;                                // skip ,
        root->right = buildTree(s, i);
        i++;                                // skip )
    }
    return root;
}

// ---------------- 2. Preorder (recursive) ----------------
void preorder(Node* root) {
    if (root == NULL) return;
    cout << root->data << " ";
    preorder(root->left);
    preorder(root->right);
}

// preorder written back in the same bracket format as the input
string treeToString(Node* root) {
    if (root == NULL) return "";
    string result = "";
    result += root->data;
    if (root->left != NULL || root->right != NULL)
        result += "(" + treeToString(root->left) + "," + treeToString(root->right) + ")";
    return result;
}

// ---------------- 3. Inorder using a stack ----------------
void inorder(Node* root) {
    stack<Node*> st;
    Node* curr = root;

    while (curr != NULL || !st.empty()) {
        while (curr != NULL) {              // go as far left as possible
            st.push(curr);
            curr = curr->left;
        }
        curr = st.top();
        st.pop();
        cout << curr->data << " ";
        curr = curr->right;                 // then do the right side
    }
}

// ---------------- 4. Postorder using stacks ----------------
// s1 gives the nodes in root-right-left order, s2 flips it to left-right-root
void postorder(Node* root) {
    if (root == NULL) return;
    stack<Node*> s1, s2;
    s1.push(root);

    while (!s1.empty()) {
        Node* curr = s1.top();
        s1.pop();
        s2.push(curr);
        if (curr->left != NULL) s1.push(curr->left);
        if (curr->right != NULL) s1.push(curr->right);
    }
    while (!s2.empty()) {
        cout << s2.top()->data << " ";
        s2.pop();
    }
}

// ---------------- 5. Check the string without building the tree ----------------
// The stack has one entry for every '(' that is still open:
// 0 = its comma has not been seen yet, 1 = comma already seen
bool isValid(const string &s) {
    if (s.empty()) return false;
    stack<int> st;

    for (int i = 0; i < (int)s.length(); i++) {
        char c = s[i];
        char prev = ' ';
        if (i > 0) prev = s[i - 1];

        if (isLetter(c)) {
            // a node can only come at the start, after '(' or after ','
            if (i > 0 && prev != '(' && prev != ',') return false;
        }
        else if (c == '(') {
            // '(' must come right after a node
            if (!isLetter(prev)) return false;
            st.push(0);
        }
        else if (c == ',') {
            // each pair of brackets has exactly one comma
            if (st.empty() || st.top() == 1) return false;
            st.top() = 1;
        }
        else if (c == ')') {
            if (st.empty() || st.top() == 0) return false;
            if (prev == ',' && s[i - 2] == '(') return false;   // "(,)" has no children
            st.pop();
        }
        else {
            return false;                   // any other character is not allowed
        }
    }
    return st.empty();                      // every '(' must be closed
}

// ---------------- 6. Height and depth ----------------
// height = edges on the longest path from the root down to a leaf (a leaf has height 0)
int height(Node* root) {
    if (root == NULL) return -1;
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);
    if (leftHeight > rightHeight)
        return leftHeight + 1;
    else
        return rightHeight + 1;
}

// depth = edges from the root down to the node
void printDepth(Node* root, int depth) {
    if (root == NULL) return;
    cout << "   " << root->data << " : depth " << depth << endl;
    printDepth(root->left, depth + 1);
    printDepth(root->right, depth + 1);
}

void deleteTree(Node* root) {
    if (root == NULL) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    string s = "a(b(d,e),c(,f(g,)))";
    cout << "Tree string: " << s << endl << endl;

    // check the string before building anything from it
    cout << "5. Validation:" << endl;
    string tests[] = { s, "a(b(d,e),c(,f(g,))", "a(b,c))", "a(b)", "ab" };
    for (int k = 0; k < 5; k++) {
        cout << "   " << tests[k] << "  ->  ";
        if (isValid(tests[k])) cout << "valid" << endl;
        else cout << "invalid" << endl;
    }
    if (!isValid(s)) return 0;

    int i = 0;
    Node* root = buildTree(s, i);
    cout << "\n1. Tree built from the string" << endl;

    cout << "\n2. Preorder:  ";
    preorder(root);
    cout << "\n   As string: " << treeToString(root) << endl;

    cout << "\n3. Inorder:   ";
    inorder(root);

    cout << "\n\n4. Postorder: ";
    postorder(root);

    cout << "\n\n6. Height of the tree: " << height(root) << endl;
    cout << "   Depth of each node:" << endl;
    printDepth(root, 0);

    deleteTree(root);

    return 0;
}