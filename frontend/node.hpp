#ifndef NODE_H
#define NODE_H

#define MAKE_VISITABLE virtual void accept(Visitor* v) const { v->visit(*this); }

class Program;
class Expression;
class Add;
class Integer;

#include <iostream>
#include <vector>
#include "visitor.hpp"

class Node {
 public:
        virtual ~Node() { }
        virtual void accept(Visitor* v) const = 0;
};

class Expression : public Node {
public:
        virtual ~Expression() { }
};

class Program : public Node {
public:
        Program(const Expression* expression);
        ~Program();
        MAKE_VISITABLE
        const Expression* e;
};

class Add : public Expression {
public:
        Add(const Expression* e1, const Expression* e2);
        ~Add();
        MAKE_VISITABLE
        const Expression* e1;
        const Expression* e2;
};

class Integer : public Expression {
public:
        Integer(const int val);
        ~Integer();
        MAKE_VISITABLE
        const int val;
};

#endif // NODE_H
