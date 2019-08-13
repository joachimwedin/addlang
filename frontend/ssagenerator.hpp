#ifndef SSA_GENERATOR_H
#define SSA_GENERATOR_H

#include "visitor.hpp"
#include "valuevisitor.hpp"
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"

class SSAGenerator : public ValueVisitor<llvm::Value*> {
public:
        SSAGenerator(llvm::LLVMContext*, llvm::IRBuilder<>*);
        void visit(const Program&);
        void visit(const Add&);
        void visit(const Integer&);
        llvm::Value* generate();
private:
        llvm::LLVMContext* context;
        llvm::IRBuilder<>* builder;
};

#endif // SSA_GENERATOR_H
