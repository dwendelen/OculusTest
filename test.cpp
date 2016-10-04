#include <iostream>
using namespace std;
class B {
    public:
    virtual void f1() {
        cout << "B::f1" << endl;
    }
    void f2() {
        cout << "B::f2" << endl;
    }
};

class D: public B {
public:
    void f1() {
        cout << "D::f1" << endl;
    }

    virtual void f2() {
        cout << "D::f2" << endl;
    }
};
int main() {
D d;
D& d1 = d;
d1.f1();
d1.f2();

//D dd;
B& d2 = d1;
d2.f1();
d2.f2();

B b1 = d;
b1.f1();
b1.f2();

while(true);
}
