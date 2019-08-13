#ifndef VISITOR_H
#define VISITOR_H

class Program;
class Add;
class Integer;

class Visitor {
public:
        virtual void visit(const Program& p) = 0;
        virtual void visit(const Add& a) = 0;
        virtual void visit(const Integer& i) = 0;
};

#endif // VISITOR_H
