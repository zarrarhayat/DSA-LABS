#include <iostream>
#include <string>
#include <vector>
using namespace std;

const int MAX = 100;

class CharStack {
    char arr[MAX];
    int topIndex;

public:
    CharStack() {
        topIndex = -1;
    }

    bool isEmpty() {
        return topIndex == -1;
    }

    void push(char c) {
        if (topIndex < MAX - 1) {
            topIndex++;
            arr[topIndex] = c;
        }
    }

    char pop() {
        char c = arr[topIndex];
        topIndex--;
        return c;
    }

    char peek() {
        return arr[topIndex];
    }
};

class StringStack {
    string arr[MAX];
    int topIndex;

public:
    StringStack() {
        topIndex = -1;
    }

    bool isEmpty() {
        return topIndex == -1;
    }

    void push(string s) {
        if (topIndex < MAX - 1) {
            topIndex++;
            arr[topIndex] = s;
        }
    }

    string pop() {
        string s = arr[topIndex];
        topIndex--;
        return s;
    }

    string peek() {
        return arr[topIndex];
    }
};

bool isValidBrackets(string s) {
    CharStack st;

    for (int i = 0; i < (int)s.size(); i++) {
        char c = s[i];

        if (c == '(' || c == '{' || c == '[') {
            st.push(c);
        } else if (c == ')' || c == '}' || c == ']') {
            if (st.isEmpty()) {
                return false;
            }
            char open = st.pop();
            if (c == ')' && open != '(') return false;
            if (c == '}' && open != '{') return false;
            if (c == ']' && open != '[') return false;
        }
    }
    return st.isEmpty();
}

bool validate_markup(string s, vector<string>& errors) {
    StringStack st;
    errors.clear();

    int i = 0;
    while (i < (int)s.size()) {
        if (s[i] != '<') {
            i++;
            continue;
        }

        int end = s.find('>', i);
        if (end == (int)string::npos) {
            errors.push_back("Tag is missing '>'");
            break;
        }

        string tag = s.substr(i + 1, end - i - 1);
        i = end + 1;

        if (tag.size() == 0 || tag[0] == '!' || tag[0] == '?') {
            continue;
        }

        if (tag[tag.size() - 1] == '/') {
            continue;
        }

        bool closing = false;
        if (tag[0] == '/') {
            closing = true;
            tag = tag.substr(1);
        }

        string name = "";
        for (int j = 0; j < (int)tag.size() && tag[j] != ' '; j++) {
            name += tag[j];
        }

        if (!closing) {
            st.push(name);
        } else if (st.isEmpty()) {
            errors.push_back("Closing tag </" + name + "> has no opening tag");
        } else if (st.peek() != name) {
            errors.push_back("Expected </" + st.peek() + "> but found </" + name + ">");
            st.pop();
        } else {
            st.pop();
        }
    }

    while (!st.isEmpty()) {
        errors.push_back("Tag <" + st.pop() + "> is never closed");
    }

    return errors.size() == 0;
}

void testBrackets(string s) {
    cout << "\"" << s << "\" -> ";
    if (isValidBrackets(s)) {
        cout << "Valid" << endl;
    } else {
        cout << "Invalid" << endl;
    }
}

void testMarkup(string s) {
    vector<string> errors;
    bool ok = validate_markup(s, errors);

    cout << s << endl;
    if (ok) {
        cout << "  -> Valid" << endl;
    } else {
        cout << "  -> Invalid" << endl;
        for (int i = 0; i < (int)errors.size(); i++) {
            cout << "     * " << errors[i] << endl;
        }
    }
}

int main() {
    cout << "===== Part A: Bracket Validator =====" << endl;
    testBrackets("{[()]}");
    testBrackets("([]{})");
    testBrackets("([)]");
    testBrackets("((()");
    testBrackets("}{");
    testBrackets("a(b[c]{d}e)f");

    cout << endl << "===== Part B: HTML/XML Tag Validator =====" << endl;
    testMarkup("<html><body><p>Hello</p></body></html>");
    testMarkup("<div><p>text</div></p>");
    testMarkup("<a><b/><c></c></a>");
    testMarkup("<div><img src=\"x.png\"/></div>");
    testMarkup("</p>");
    testMarkup("<div><span>");

    string input;
    cout << endl << "Enter brackets to check: ";
    getline(cin, input);
    testBrackets(input);

    cout << "Enter markup to check: ";
    getline(cin, input);
    testMarkup(input);

    return 0;
}
