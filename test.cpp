#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <linux/input.h>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>

using namespace std;
namespace test {
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

    union myop {
        input_event event;
        char chars[sizeof(input_event)];
    };


}
using namespace test;

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


        string path(getenv("LD_LIBRARY_PATH"));
        cout << path << endl;

        string prl(getenv("LD_PRELOAD"));
        cout << prl << endl;

        int fd;
        struct input_event event;

        fd = open("/dev/input/event18", O_RDONLY);

        while(fd >= 0){
            read(fd, &event, sizeof(struct input_event));
            cout << "Code:" << event.code << " Type: " << event.type << " Value: " << event.value << endl;
        }
    }
