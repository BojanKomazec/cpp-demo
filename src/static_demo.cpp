#include <static_demo.hpp>
#include <iostream>

namespace static_demo {
    class C {
    public:
        // static variable has to be defined (initialized) somewhere.
        static int n1;
        // Since C++17, we can initialize static variables from inside the class:
        inline static int n2 = 2;

        // non-static data member (will not be initialized by compiler)
        const int n3;
        static const int n4;
        static int n5;

        void try_to_change_class_state(int i) const {
            // error: cannot assign to non-static data member within const member function 'try_to_change_class_state'
            // priv_n1_ = i;

            // error: cannot assign to variable 'n4' with const-qualified type 'const int'
            // n4 = i;

            // const member function can change non-const static data members
            n5 = i;
        }
    private:
        int priv_n1_;

    };

    int C::n1 = 1;

    void run() {
        std::cout << "static_demo()" << std::endl;

        // If we omit C::n1 initialization we'll get:
        // linker error: undefined reference to `static_demo::C::n1'
        std::cout << "C::n1 = " << C::n1 << std::endl;
        std::cout << "C::n2 = " << C::n2 << std::endl;
        // std::cout << "C::n3 = " << C::n3 << std::endl;
    }
}
