#include "node.hpp"
#include "visitor.hpp"

Program::Program(const Expression* expression): e(expression) { }

Program::~Program() {
        delete e;
}

Add::Add(const Expression* e1, const Expression* e2): e1(e1), e2(e2) { }

Add::~Add() {
        delete e1;
        delete e2;
}

Integer::Integer(const int val): val(val) { }

Integer::~Integer() { }
