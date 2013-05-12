#include "calculator.h"
#include <cassert>
#include <iostream>

int main() {
    calculator::engine calc;

    assert(calc.run("100") == 100);
    assert(calc.run("1+2") == 3);
    assert(calc.run("1.2-0.2") == 1);
    assert(calc.run("4*.3") == 1.2);
    assert(calc.run("300/50") == 6);
    assert(calc.run("4%2") == 0);
    assert(calc.run("9.6/2") == 4.8);
    assert(calc.run("3+2*5") == 13);
    assert(calc.run("(3+2)*5") == 25);
    try {
        calc.run("1/0");
        assert("divide by ZERO" && false);
    } catch(...) {
    }

    try {
        calc.run("(");
        assert("')' expected" && false);
    } catch(...) {
    }

    try {
        calc.run("a");
        assert("invalid characer" && false);
    } catch(...) {
    }

    try {
        calc.run("");
        assert("no data found" && false);
    } catch(...) {
    }
}
