#include <iostream>
#include <string>
#include <limits>
using namespace std;

struct BitNode {
    int bit;
    BitNode* prev;
    BitNode* next;

    explicit BitNode(int b) : bit(b), prev(nullptr), next(nullptr) {}
};

class BinaryNumber {
private:
    BitNode* head;
    BitNode* tail;
    int count;

    void clear() {
        BitNode* cur = head;
        while (cur != nullptr) {
            BitNode* nextNode = cur->next;
            delete cur;
            cur = nextNode;
        }
        head = nullptr;
        tail = nullptr;
        count = 0;
    }

    void copyFrom(const BinaryNumber& other) {
        for (BitNode* cur = other.head; cur != nullptr; cur = cur->next)
            pushBack(cur->bit);
    }

    void popFront() {
        if (head == nullptr)
            return;
        BitNode* old = head;
        head = head->next;
        if (head != nullptr)
            head->prev = nullptr;
        else
            tail = nullptr;
        delete old;
        count--;
    }

    void removeLeadingZeros() {
        while (count > 1 && head->bit == 0)
            popFront();
    }

    void padToByte() {
        if (count == 0)
            pushFront(0);
        while (count % 8 != 0)
            pushFront(0);
    }

    void resizeTo(int width) {
        while (count < width)
            pushFront(0);
        while (count > width)
            popFront();
    }

public:
    BinaryNumber() : head(nullptr), tail(nullptr), count(0) {}

    explicit BinaryNumber(const string& bits) : head(nullptr), tail(nullptr), count(0) {
        for (char c : bits)
            pushBack(c - '0');
        normalize();
    }

    BinaryNumber(const BinaryNumber& other) : head(nullptr), tail(nullptr), count(0) {
        copyFrom(other);
    }

    BinaryNumber& operator=(const BinaryNumber& other) {
        if (this != &other) {
            clear();
            copyFrom(other);
        }
        return *this;
    }

    ~BinaryNumber() {
        clear();
    }

    void pushFront(int bit) {
        BitNode* node = new BitNode(bit);
        if (head == nullptr) {
            head = tail = node;
        } else {
            node->next = head;
            head->prev = node;
            head = node;
        }
        count++;
    }

    void pushBack(int bit) {
        BitNode* node = new BitNode(bit);
        if (tail == nullptr) {
            head = tail = node;
        } else {
            node->prev = tail;
            tail->next = node;
            tail = node;
        }
        count++;
    }

    void normalize() {
        removeLeadingZeros();
        padToByte();
    }

    bool isEmpty() const { return count == 0; }

    static bool isValid(const string& bits) {
        if (bits.empty())
            return false;
        for (char c : bits)
            if (c != '0' && c != '1')
                return false;
        return true;
    }

    void display() const {
        int position = 0;
        for (BitNode* cur = head; cur != nullptr; cur = cur->next) {
            if (position > 0 && position % 8 == 0)
                cout << " ";
            cout << cur->bit;
            position++;
        }
    }

    BinaryNumber onesComplement() const {
        BinaryNumber result(*this);
        for (BitNode* cur = result.head; cur != nullptr; cur = cur->next)
            cur->bit = 1 - cur->bit;
        return result;
    }

    BinaryNumber twosComplement() const {
        BinaryNumber result = add(onesComplement(), BinaryNumber("1"));
        result.resizeTo(count);
        return result;
    }

    static BinaryNumber add(const BinaryNumber& a, const BinaryNumber& b) {
        BinaryNumber result;
        BitNode* x = a.tail;
        BitNode* y = b.tail;
        int carry = 0;

        while (x != nullptr || y != nullptr || carry != 0) {
            int sum = carry;
            if (x != nullptr) { sum += x->bit; x = x->prev; }
            if (y != nullptr) { sum += y->bit; y = y->prev; }
            result.pushFront(sum % 2);
            carry = sum / 2;
        }
        result.normalize();
        return result;
    }

    void shiftLeft() {
        pushBack(0);
    }

    static BinaryNumber multiply(const BinaryNumber& a, const BinaryNumber& b) {
        BinaryNumber result("0");
        BinaryNumber shifted(a);

        for (BitNode* cur = b.tail; cur != nullptr; cur = cur->prev) {
            if (cur->bit == 1)
                result = add(result, shifted);
            shifted.shiftLeft();
        }
        result.normalize();
        return result;
    }

    string toDecimal() const {
        string value = "0";
        for (BitNode* cur = head; cur != nullptr; cur = cur->next) {
            int carry = cur->bit;
            for (int i = (int)value.size() - 1; i >= 0; i--) {
                int digit = (value[i] - '0') * 2 + carry;
                value[i] = char('0' + digit % 10);
                carry = digit / 10;
            }
            if (carry > 0)
                value.insert(value.begin(), char('0' + carry));
        }
        return value;
    }
};

bool readInt(int& value) {
    if (cin >> value)
        return true;
    if (!cin.eof()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return false;
}

bool readBinary(const string& label, BinaryNumber& number) {
    string line;
    while (true) {
        cout << "Enter binary number " << label << " (only 0s and 1s): ";
        if (!getline(cin, line))
            return false;
        string bits;
        for (char c : line)
            if (c != ' ' && c != '\t' && c != '\r')
                bits += c;
        if (BinaryNumber::isValid(bits)) {
            number = BinaryNumber(bits);
            return true;
        }
        cout << "Invalid input. Use only the digits 0 and 1." << endl;
    }
}

void show(const string& label, const BinaryNumber& n) {
    cout << label;
    n.display();
    cout << "  (decimal " << n.toDecimal() << ")" << endl;
}

int main() {
    BinaryNumber a, b;
    int choice = 0;

    do {
        cout << "\n===== Binary Arithmetic Menu =====" << endl;
        cout << "1. Store binary number A" << endl;
        cout << "2. Store binary number B" << endl;
        cout << "3. Display A and B" << endl;
        cout << "4. 1's complement of A" << endl;
        cout << "5. 2's complement of A" << endl;
        cout << "6. Add A + B" << endl;
        cout << "7. Multiply A x B" << endl;
        cout << "8. Convert A and B to decimal" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter your choice: ";

        if (!readInt(choice)) {
            if (cin.eof())
                break;
            cout << "Please enter a number from 1 to 9." << endl;
            choice = 0;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                if (readBinary("A", a))
                    show("A stored as: ", a);
                break;
            case 2:
                if (readBinary("B", b))
                    show("B stored as: ", b);
                break;
            case 3:
                if (a.isEmpty()) cout << "A has not been entered." << endl;
                else show("A = ", a);
                if (b.isEmpty()) cout << "B has not been entered." << endl;
                else show("B = ", b);
                break;
            case 4:
                if (a.isEmpty()) cout << "Enter A first (option 1)." << endl;
                else {
                    show("A                = ", a);
                    show("1's complement   = ", a.onesComplement());
                }
                break;
            case 5:
                if (a.isEmpty()) cout << "Enter A first (option 1)." << endl;
                else {
                    show("A                = ", a);
                    show("2's complement   = ", a.twosComplement());
                }
                break;
            case 6:
                if (a.isEmpty() || b.isEmpty()) cout << "Enter both A and B first." << endl;
                else show("A + B = ", BinaryNumber::add(a, b));
                break;
            case 7:
                if (a.isEmpty() || b.isEmpty()) cout << "Enter both A and B first." << endl;
                else show("A x B = ", BinaryNumber::multiply(a, b));
                break;
            case 8:
                if (a.isEmpty()) cout << "A has not been entered." << endl;
                else cout << "A in decimal: " << a.toDecimal() << endl;
                if (b.isEmpty()) cout << "B has not been entered." << endl;
                else cout << "B in decimal: " << b.toDecimal() << endl;
                break;
            case 9:
                cout << "Exiting." << endl;
                break;
            default:
                cout << "Please enter a number from 1 to 9." << endl;
        }
    } while (choice != 9 && !cin.eof());

    return 0;
}