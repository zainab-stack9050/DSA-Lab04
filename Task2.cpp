#include <iostream>
using namespace std;

struct Person {
    int id;
    Person* next;
    explicit Person(int i) : id(i), next(nullptr) {} };

// building a circle of N people 
Person* createCircle(int n) {
    Person *head = nullptr, *tail = nullptr;
    for (int i = 1; i <= n; ++i) {
        Person* p = new Person(i);
        if (!head) head = tail = p;
        else { tail->next = p; tail = p; }
    }
    tail->next = head;                    //closing circle
    return tail; }

int main() {
    int n, k;
    cout << "Number of people (N): ";
    cin >> n;
    cout << "Step count (k): ";
    cin >> k;
    if (!cin || n < 1 || k < 1) { cout << "N and k must be positive integers.\n"; return 1; }

    Person* prev = createCircle(n);       //prev always sits just before the counting start

    //eliminated order being stored
    Person *orderHead = nullptr, *orderTail = nullptr;

    //counting starts at person 1 and eliminates every kth element
    while (prev->next != prev) {         
        for (int i = 1; i < k; ++i) prev = prev->next;   //move to the person before the k-th
        Person* victim = prev->next;
        prev->next = victim->next;    //unlike the victim from the circle   

        victim->next = nullptr;           // reuse the node: append it to the eliminated list
        if (!orderHead) orderHead = orderTail = victim;
        else { orderTail->next = victim; orderTail = victim; }
    }

    cout << "\nElimination order: ";
    for (Person* p = orderHead; p; p = p->next)
        cout << p->id << (p->next ? " -> " : "");
    cout << "\nSurvivor: Person " << prev->id << "\n";

    delete prev;                          //freeing the last node and elimination list
    while (orderHead) {                
        Person* t = orderHead;
        orderHead = orderHead->next;
        delete t;
    }
    return 0;
}