#include <iostream>
#include <vector>
#include <llvm/IR/Value.h>

class Expression;

class Node {
 public:
        virtual ~Node() {}
        virtual llvm::Value* codeGen() { }
};

class Program : public Node {
public:
        Program(const Expression& expression): e(expression) { }
private:
        const Expression& e;
};

class Expression : public Node {
};

class Add : public Expression {
public:
        Add(const Expression& e1, const Expression& e2): e1(e1), e2(e2) { }
private:
        const Expression& e1;
        const Expression& e2;
};

class Integer : public Expression {
public:
        Integer(const int& val): val(val) { }
private:
        const int& val;
};
