#include "ssagenerator.hpp"
#include "node.hpp"
#include "parser.hpp"
#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Constants.h"

extern Program* programRoot;

llvm::Value* SSAGenerator::generate() {
        return getValue(*programRoot);
}

SSAGenerator::SSAGenerator(llvm::LLVMContext* context_, llvm::IRBuilder<>* builder_) {
        builder = builder_;
        context = context_;
}

void SSAGenerator::visit(const Program& p) {
        returnValue(getValue(*p.e));
}

void SSAGenerator::visit(const Add& a) {
        returnValue((*builder).CreateAdd(getValue(*a.e1), getValue(*a.e2), "addtmp"));
}

void SSAGenerator::visit(const Integer& i) {
        returnValue(llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), i.val));
}
