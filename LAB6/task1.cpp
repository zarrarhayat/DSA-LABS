#include <iostream>
#include <string>
using namespace std;

const int MAX = 100;

template <typename T>
class Stack {
    T arr[MAX];
    int topIndex;

public:
    Stack() {
        topIndex = -1;
    }

    bool isEmpty() {
        return topIndex == -1;
    }

    bool isFull() {
        return topIndex == MAX - 1;
    }

    void push(T x) {
        if (isFull()) {
            cout << "Stack overflow" << endl;
            return;
        }
        topIndex++;
        arr[topIndex] = x;
    }

    T pop() {
        if (isEmpty()) {
            cout << "Stack underflow" << endl;
            return T();
        }
        T x = arr[topIndex];
        topIndex--;
        return x;
    }

    T peek() {
        if (isEmpty()) {
            return T();
        }
        return arr[topIndex];
    }
};

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

int precedence(char c) {
    if (c == '^') return 3;
    if (c == '*' || c == '/') return 2;
    if (c == '+' || c == '-') return 1;
    return 0;
}

string toPostfix(string expr, bool forPrefix) {
    Stack<char> st;
    string result = "";

    for (int i = 0; i < (int)expr.size(); i++) {
        char c = expr[i];

        if (c == ' ') {
            continue;
        }

        if (isOperator(c)) {
            while (!st.isEmpty() && st.peek() != '(') {
                int top = precedence(st.peek());
                int cur = precedence(c);
                bool popIt;
                if (c == '^' && !forPrefix) {
                    popIt = top > cur;
                } else if (c == '^' && forPrefix) {
                    popIt = top >= cur;
                } else if (forPrefix) {
                    popIt = top > cur;
                } else {
                    popIt = top >= cur;
                }
                if (!popIt) break;
                result += st.pop();
            }
            st.push(c);
        } else if (c == '(') {
            st.push(c);
        } else if (c == ')') {
            while (!st.isEmpty() && st.peek() != '(') {
                result += st.pop();
            }
            st.pop();
        } else {
            result += c;
        }
    }

    while (!st.isEmpty()) {
        result += st.pop();
    }
    return result;
}

string infix_to_postfix(string expr) {
    return toPostfix(expr, false);
}

string infix_to_prefix(string expr) {
    string rev = "";
    for (int i = (int)expr.size() - 1; i >= 0; i--) {
        char c = expr[i];
        if (c == '(') c = ')';
        else if (c == ')') c = '(';
        rev += c;
    }

    string post = toPostfix(rev, true);

    string result = "";
    for (int i = (int)post.size() - 1; i >= 0; i--) {
        result += post[i];
    }
    return result;
}

string postfix_to_infix(string expr) {
    Stack<string> st;

    for (int i = 0; i < (int)expr.size(); i++) {
        char c = expr[i];
        if (c == ' ') {
            continue;
        }
        if (isOperator(c)) {
            string b = st.pop();
            string a = st.pop();
            st.push("(" + a + c + b + ")");
        } else {
            st.push(string(1, c));
        }
    }
    return st.pop();
}

double calculate(double a, double b, char op) {
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    if (op == '/') {
        if (b == 0) {
            cout << "Division by zero" << endl;
            return 0;
        }
        return a / b;
    }
    double result = 1;
    for (int i = 0; i < (int)b; i++) {
        result = result * a;
    }
    return result;
}

double evaluate_postfix(string expr) {
    Stack<double> st;

    for (int i = 0; i < (int)expr.size(); i++) {
        char c = expr[i];
        if (c == ' ') {
            continue;
        }
        if (isOperator(c)) {
            double b = st.pop();
            double a = st.pop();
            st.push(calculate(a, b, c));
        } else {
            st.push(c - '0');
        }
    }
    return st.pop();
}

double evaluate_prefix(string expr) {
    Stack<double> st;

    for (int i = (int)expr.size() - 1; i >= 0; i--) {
        char c = expr[i];
        if (c == ' ') {
            continue;
        }
        if (isOperator(c)) {
            double a = st.pop();
            double b = st.pop();
            st.push(calculate(a, b, c));
        } else {
            st.push(c - '0');
        }
    }
    return st.pop();
}

void test(string infix) {
    string post = infix_to_postfix(infix);
    string pre = infix_to_prefix(infix);

    cout << "Infix             : " << infix << endl;
    cout << "Postfix           : " << post << endl;
    cout << "Prefix            : " << pre << endl;
    cout << "Postfix to infix  : " << postfix_to_infix(post) << endl;
    cout << "Postfix value     : " << evaluate_postfix(post) << endl;
    cout << "Prefix value      : " << evaluate_prefix(pre) << endl;
    cout << endl;
}

int main() {
    cout << "Use single digit numbers to see the evaluated values" << endl << endl;

    test("2+3*4");
    test("(2+3)*4");
    test("8-3-2");
    test("2^3^2");
    test("9/(5-2)+6*2");

    string expr;
    cout << "Enter your own infix expression (no spaces): ";
    cin >> expr;
    test(expr);

    return 0;
}
