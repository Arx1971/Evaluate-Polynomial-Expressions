#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#define FOR(a, b, c) for (int(a) = (b); (a) < (c); ++(a))
#define FORR(a, b, c) for (int(a) = (c); (a) >= (b); (a)--)

int i, j;

using namespace std;

template<typename T>
class LinkedList {

private:
    struct Node {
        T data;
        Node *next;

        Node(T data, Node *next) : data(data), next(next) {}

    } *head, *tail;

    int size;

public:
    LinkedList() : head(NULL), tail(NULL), size(0) {}

    ~LinkedList() = default;

    void push(T d);

    T &get(int index);

    T remove(int index);

    int getSize() {
        return size;
    }

    friend ostream &operator<<(ostream &os, LinkedList<T> myList) {
        FORR(i, 0, myList.size - 1)os << myList.get(i) << " ";

        return os;
    }
};

template<class T>
void LinkedList<T>::push(T d) {
    Node *n = new Node(d, head);
    head = n;
    if (tail == NULL)
        tail = head;

    size++;
}

template<class T>
T LinkedList<T>::remove(int index) {

    if (size == 0)
        throw logic_error("Empty List");
    else if (index >= size || index < 0)
        throw runtime_error("Index out of Bound");

    Node *current = head->next;
    T ans;
    int count = 0;
    while (current != tail) {
        if (count == index - 1) {
            ans = current->next->data;
            current->next = current->next->next;
            size--;
        } else
            current = current->next;

        count++;
    }
    return ans;
}

template<class T>

T &LinkedList<T>::get(int index) {
    Node *current = head;
    T *ans;
    int count = 0;
    while (current != NULL) {
        if (count == index) {
            ans = &current->data;
            break;
        }
        count++;
        current = current->next;
    }
    return *ans;
}

class Polynomial {

private:

    struct Expression {
        int coefficient, exponent;

        Expression(int c = 0, int e = 0) : coefficient(c), exponent(e) {}

        int &getCoefficient() {
            return coefficient;
        }

        int &getExponent() {
            return exponent;
        }

        bool operator==(Expression n) {
            return getCoefficient() == n.getCoefficient() && getExponent() == n.getExponent();
        }

        friend ostream &operator<<(ostream &os, Expression expression) {
            os << expression.getCoefficient() << "X^" << expression.getExponent();
            return os;
        }

    };

    LinkedList<Expression> equation;
    LinkedList<Expression> canonicalForm;

public:

    Polynomial() = default;

    ~Polynomial() = default;

    Polynomial &operator+(const Polynomial &polynomial);

    Polynomial &operator-(const Polynomial &polynomial);

    Polynomial &operator*(const Polynomial &polynomial);


    Polynomial(string str) {
        stringstream ss{str};
        int c = 0, e = 0;
        while (ss >> c >> e) {
            equation.push(Expression(c, e));
            addExpression(c, e);
        }
        sort();
    }

    Polynomial(const Polynomial &polynomial) : equation(polynomial.equation), canonicalForm(polynomial.canonicalForm) {}

    void addExpression(int c, int e) {
        FOR(i, 0, canonicalForm.getSize() - 1) {
            if (canonicalForm.get(i).getExponent() == e) {
                canonicalForm.get(i).getCoefficient() = canonicalForm.get(i).getCoefficient() + c;
                if (canonicalForm.get(i).getCoefficient() == 0) {
                    canonicalForm.remove(i);
                }
                return;
            }
        }
        canonicalForm.push(Expression(c, e));
    }

    void sort();

    friend ostream &operator<<(ostream &os, const Polynomial &polynomial) {
        os << polynomial.canonicalForm << endl;
        return os;
    }

};

template<typename T>
void swap(T *a, T *b) {
    T temp(move(*a));
    *a = move(*b);
    *b = move(temp);
}

void Polynomial::sort() {

    FOR(i, 0, canonicalForm.getSize() - 1) {
        FOR(j, 0, canonicalForm.getSize() - 1) {
            if (canonicalForm.get(j).getExponent() > canonicalForm.get(j + 1).getExponent()) {
                swap(&canonicalForm.get(j), &canonicalForm.get(j + 1));
            }
        }
    }
}

Polynomial &Polynomial::operator+(const Polynomial &polynomial) {
    Polynomial temp(polynomial);
    Polynomial *solution = new Polynomial();

    FOR(i, 0, canonicalForm.getSize())solution->addExpression(canonicalForm.get(i).getCoefficient(),
                                                              canonicalForm.get(i).getExponent());

    FOR(i, 0, temp.canonicalForm.getSize())solution->addExpression(temp.canonicalForm.get(i).getCoefficient(),
                                                                   temp.canonicalForm.get(i).getExponent());

    solution->sort();
    return *solution;
}

Polynomial &Polynomial::operator-(const Polynomial &polynomial) {
    Polynomial temp(polynomial);
    Polynomial *solution = new Polynomial();

    FOR(i, 0, canonicalForm.getSize())solution->addExpression(canonicalForm.get(i).getCoefficient(),
                                                              canonicalForm.get(i).getExponent());

    FOR(i, 0, temp.canonicalForm.getSize())solution->addExpression((-1) * temp.canonicalForm.get(i).getCoefficient(),
                                                                   temp.canonicalForm.get(i).getExponent());

    solution->sort();
    return *solution;
}

Polynomial &Polynomial::operator*(const Polynomial &polynomial) {
    Polynomial *solution = new Polynomial();
    Polynomial temp(polynomial);

    FOR(i, 0, temp.canonicalForm.getSize()) {
        FOR (j, 0, canonicalForm.getSize()) {
            int tempCoefficient = temp.canonicalForm.get(i).getCoefficient() * canonicalForm.get(j).getCoefficient();
            int tempExponent = temp.canonicalForm.get(i).getExponent() + canonicalForm.get(j).getExponent();
            solution->addExpression(tempCoefficient, tempExponent);
        }
    }
    solution->sort();
    return *solution;
}

int main() {
    ifstream input;
    input.open("input.txt");
    string line;
    LinkedList<string> arr;

    if (input) {
        while (getline(input, line)) {
            arr.push(line);
        }

        int test_case = 0;

        for (int i = 0; i < arr.getSize() - 1; i += 2) {

            Polynomial equation1(arr.get(i));
            Polynomial equation2(arr.get(i + 1));
            cout << "CASE: " << (++test_case) << endl;
            cout << showpos;
            cout << "Equations 1 Canonical Form: \n" << equation1;
            cout << "Equations 2 Canonical Form: \n" << equation2;
            cout << "Sum: \n" << equation1 + equation2;
            cout << "Difference: \n" << equation1 - equation2;
            cout << "Product: \n" << equation1 * equation2 << endl;
            cout << endl;
        }

    } else
        throw runtime_error("File Not Found");

    input.close();

    return 0;
}
