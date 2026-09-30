#include <iostream>
#include <string>
using namespace std;

struct BitNode {
    int bit;
    BitNode *next, *prev;
    explicit BitNode(int b) : bit(b), next(nullptr), prev(nullptr) {}
};

class BinaryDLL {
    BitNode *head = nullptr, *tail = nullptr;
    int count = 0;

    void clear() {
        while (head) { BitNode* t = head; head = head->next; delete t; }
        tail = nullptr; count = 0;
    }
public:
    BinaryDLL() {}
    BinaryDLL(const BinaryDLL& o) { for (BitNode* p = o.head; p; p = p->next) pushBack(p->bit); }
    BinaryDLL& operator=(const BinaryDLL& o) {
        if (this != &o) { clear(); for (BitNode* p = o.head; p; p = p->next) pushBack(p->bit); }
        return *this;
    }
    ~BinaryDLL() { clear(); }

    int size() const { return count; }

    void pushBack(int b) {
        BitNode* n = new BitNode(b);
        if (!tail) head = tail = n;
        else { tail->next = n; n->prev = tail; tail = n; }
        ++count;
    }
    void pushFront(int b) {
        BitNode* n = new BitNode(b);
        if (!head) head = tail = n;
        else { n->next = head; head->prev = n; head = n; }
        ++count;
    }

    // Add leading zeros until the length is a multiple of 8 (at least one block)
    void padTo8() {
        if (count == 0) pushFront(0);
        while (count % 8 != 0) pushFront(0);
    }

    // Store Binary Number: string of 0/1 -> DLL in 8-bit blocks
    bool store(const string& s) {
        if (s.empty()) return false;
        for (char c : s) if (c != '0' && c != '1') return false;
        clear();
        for (char c : s) pushBack(c - '0');
        padTo8();
        return true;
    }

    // 1's complement: flip every bit (in place)
    void onesComplement() {
        for (BitNode* p = head; p; p = p->next) p->bit ^= 1;
    }

    // Binary addition, walking both lists from LSB to MSB with a carry.
    // If keepWidth is true, a final carry-out is discarded (fixed-width arithmetic).
    static BinaryDLL add(const BinaryDLL& a, const BinaryDLL& b, bool keepWidth = false) {
        BinaryDLL r;
        BitNode *pa = a.tail, *pb = b.tail;
        int carry = 0;
        while (pa || pb) {
            int sum = carry + (pa ? pa->bit : 0) + (pb ? pb->bit : 0);
            r.pushFront(sum % 2);
            carry = sum / 2;
            if (pa) pa = pa->prev;
            if (pb) pb = pb->prev;
        }
        if (carry && !keepWidth) r.pushFront(1);
        r.padTo8();
        return r;
    }

    // 2's complement = 1's complement + 1 (width preserved)
    BinaryDLL twosComplement() const {
        BinaryDLL c(*this);
        c.onesComplement();
        BinaryDLL one;
        one.store("1");
        return add(c, one, true);
    }

    // Multiplication by repeated addition + shifting:
    // for each bit of the multiplier (LSB first), add the shifted multiplicand if bit = 1,
    // then shift the multiplicand left by appending a 0 at the tail.
    static BinaryDLL multiply(const BinaryDLL& a, const BinaryDLL& b) {
        BinaryDLL result;
        result.store("0");
        BinaryDLL shifted(a);
        for (BitNode* p = b.tail; p; p = p->prev) {
            if (p->bit == 1) result = add(result, shifted);
            shifted.pushBack(0);
        }
        result.padTo8();
        return result;
    }

    // Convert to decimal. Uses a string of digits (double-and-add) so it works for any length.
    string toDecimal() const {
        string d = "0";                      // decimal digits, least significant first
        for (BitNode* p = head; p; p = p->next) {
            int carry = p->bit;              // d = d*2 + bit
            for (size_t i = 0; i < d.size(); ++i) {
                int v = (d[i] - '0') * 2 + carry;
                d[i] = char('0' + v % 10);
                carry = v / 10;
            }
            if (carry) d += char('0' + carry);
        }
        string s;
        for (int i = (int)d.size() - 1; i >= 0; --i) s += d[i];
        return s;
    }

    // Print as 8-bit groups, e.g. 00001010 11110000
    void display(const string& label) const {
        cout << label;
        int i = 0;
        for (BitNode* p = head; p; p = p->next, ++i) {
            if (i && i % 8 == 0) cout << ' ';
            cout << p->bit;
        }
        cout << "\n";
    }
};

static bool readBinary(const string& prompt, BinaryDLL& out) {
    string s;
    cout << prompt;
    cin >> s;
    if (!out.store(s)) { cout << "Invalid input: use only 0 and 1.\n"; return false; }
    return true;
}

int main() {
    int choice;
    do {
        cout << "\n===== BINARY ARITHMETIC (DLL) =====\n"
             << "1. Store & display a binary number\n2. 1's Complement\n3. 2's Complement\n"
             << "4. Binary Addition\n5. Binary Multiplication\n6. Convert to Decimal\n0. Exit\n"
             << "Choice: ";
        if (!(cin >> choice)) break;
        BinaryDLL a, b;
        switch (choice) {
            case 1:
                if (readBinary("Enter binary number: ", a)) a.display("Stored (8-bit blocks): ");
                break;
            case 2:
                if (readBinary("Enter binary number: ", a)) {
                    a.display("Original:      ");
                    a.onesComplement();
                    a.display("1's complement: ");
                }
                break;
            case 3:
                if (readBinary("Enter binary number: ", a)) {
                    a.display("Original:       ");
                    a.twosComplement().display("2's complement: ");
                }
                break;
            case 4:
                if (readBinary("First number:  ", a) && readBinary("Second number: ", b)) {
                    BinaryDLL r = BinaryDLL::add(a, b);
                    r.display("Sum: ");
                    cout << "Decimal: " << a.toDecimal() << " + " << b.toDecimal()
                         << " = " << r.toDecimal() << "\n";
                }
                break;
            case 5:
                if (readBinary("Multiplicand: ", a) && readBinary("Multiplier:   ", b)) {
                    BinaryDLL r = BinaryDLL::multiply(a, b);
                    r.display("Product: ");
                    cout << "Decimal: " << a.toDecimal() << " x " << b.toDecimal()
                         << " = " << r.toDecimal() << "\n";
                }
                break;
            case 6:
                if (readBinary("Enter binary number: ", a)) {
                    a.display("Binary:  ");
                    cout << "Decimal: " << a.toDecimal() << "\n";
                }
                break;
            case 0: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    return 0;
}