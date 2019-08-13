#include "node.hpp"
#include "visitor.hpp"

template<class valueType>
class ValueVisitor : public Visitor {
public:
        valueType getValue(const Node& node) {
                node.accept(this);
                return value;
        }
        void returnValue(valueType value_) {
                value = value_;
        }
private:
        valueType value;
};
