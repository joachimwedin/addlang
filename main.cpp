#include "cxxopts.hpp"
#include "node.hpp"
#include "parser.hpp"
#include "ssagenerator.hpp"

#include "llvm/IR/Verifier.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/Argument.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "gtest/gtest.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

extern FILE* yyin;
extern int yyparse();
extern int yylex();
extern Program* programRoot;

int createAndRunJIT(std::unique_ptr<llvm::Module>* module, llvm::Function* F) {
        llvm::InitializeNativeTarget();
        LLVMInitializeNativeAsmPrinter();
        LLVMInitializeNativeAsmParser();
        LLVMLinkInMCJIT();

        std::string err_str;
        llvm::ExecutionEngine* EE = llvm::EngineBuilder(std::move(*module)).setErrorStr(&err_str).create();
        if (!EE) {
                std::cout << "Could not create ExecutionEngine:"
                          << err_str << std::endl;
                return 1;
        }
        std::vector<llvm::GenericValue> noargs;
        llvm::GenericValue gv = EE->runFunction(F, noargs);
        llvm::outs() << gv.IntVal << "\n";

        delete EE;

        return 0;
}

llvm::Function* buildIR(llvm::LLVMContext* context, std::unique_ptr<llvm::Module>* module, llvm::IRBuilder<>* builder, llvm::Value* ssa) {
        llvm::FunctionType* FT = llvm::FunctionType::get(llvm::Type::getInt32Ty(*context), false);
        llvm::Function* F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "f", module->get());
        llvm::BasicBlock* BB = llvm::BasicBlock::Create(*context, "entry", F);
        builder->SetInsertPoint(BB);
        builder->CreateRet(ssa);
        if (!verifyFunction(*F)) {
                return F;
        }
        return nullptr;
}

void tokenize() {
        int yychar;
        while((yychar = yylex())) {
                const char* tokenstr = token_string(yychar);
                cout << tokenstr << " ";
        }
        cout << endl;
}

int main(int argc, char* argv[]) {
        string file;
        if (argc < 2) {
                cout << "Too few arguments. You must supply an input file." << endl;
                return -1;
        } else {
                file = argv[argc-1];
        }
        FILE *pt = fopen(file.c_str(), "r" );
        if(!pt) {
                cout << "Bad Input. Nonexistent file" << endl;
                return -1;
        }
        yyin = pt;

        cxxopts::Options options("A compiler for addlang", "A description");
        options.add_options()
                ("d,debug", "Enable Bison debugging")
                ("t,tokenize", "")
                ("e,emit-llvm", "")
                ;
        auto result = options.parse(argc, argv);

        if (result.count("tokenize")) {
                tokenize();
                return 0;
        } else if (result.count("debug")) {
                yydebug = true;
        }

        yyparse();

        llvm::LLVMContext context;
        llvm::IRBuilder<> builder(context);
        std::unique_ptr<llvm::Module> module = llvm::make_unique<llvm::Module>("module", context);

        SSAGenerator ssaGenerator(&context, &builder);
        llvm::Value* ssa = ssaGenerator.generate();
        if (!ssa) {
                std::cout << "Failed to build SSA form" << std::endl;
                return 1;
        }

        llvm::Function* F = buildIR(&context, &module, &builder, ssa);
        if (F == nullptr) {
                std::cout << "Failed to build LLVM IR" << std::endl;
                return 1;
        }
        if (result.count("emit-llvm")) {
                module->print(llvm::errs(), nullptr);
                return 0;
        }

        int status = createAndRunJIT(&module, F);

        llvm::llvm_shutdown();
        delete programRoot;

        return status;
}
