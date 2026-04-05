#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
using namespace llvm;

Function *createNewAdd(Module &M, LLVMContext &Ctx)
{
  Type *i32 = Type::getInt32Ty(Ctx);
  FunctionType *FT = FunctionType::get(i32, {i32, i32}, false);
  Function *F = Function::Create(FT, Function::ExternalLinkage, "NewAdd", &M);
  BasicBlock *entry = BasicBlock::Create(Ctx, "entry", F);
  BasicBlock *trueBB = BasicBlock::Create(Ctx, "true", F);
  BasicBlock *falseBB = BasicBlock::Create(Ctx, "false", F);
  BasicBlock *end = BasicBlock::Create(Ctx, "end", F);
  IRBuilder<> builder(entry);
  Value *a = F->getArg(0);
  Value *b = F->getArg(1);
  Value *sum = builder.CreateAdd(a, b, "sum");
  Value *cmp = builder.CreateICmpSGT(sum, ConstantInt::get(i32, 100), "cmp");
  builder.CreateCondBr(cmp, trueBB, falseBB);
  builder.SetInsertPoint(trueBB);
  Value *sub = builder.CreateSub(sum, ConstantInt::get(i32, 100), "sub");
  builder.CreateBr(end);
  builder.SetInsertPoint(falseBB);
  builder.CreateBr(end);
  builder.SetInsertPoint(end);
  PHINode *phi = builder.CreatePHI(i32, 2, "retval");
  phi->addIncoming(sub, trueBB);
  phi->addIncoming(sum, falseBB);
  builder.CreateRet(phi);
  return F;
}

void replaceAddWithCall(Module &M, Function *NewAdd)
{
  for (auto &F : M)
  {
    if (F.isDeclaration() || &F == NewAdd)
      continue;
    for (auto &BB : F)
    {
      for (auto it = BB.begin(); it != BB.end();)
      {
        Instruction &I = *it++;
        if (I.getOpcode() == Instruction::Add && I.getType()->isIntegerTy(32))
        {
          IRBuilder<> builder(&I);
          Value *call = builder.CreateCall(NewAdd, {I.getOperand(0), I.getOperand(1)});
          I.replaceAllUsesWith(call);
          I.eraseFromParent();
        }
      }
    }
  }
}

int main(int argc, char **argv)
{
  LLVMContext Ctx;
  SMDiagnostic Err;
  std::unique_ptr<Module> M = parseIRFile(argv[1], Err, Ctx);
  if (!M)
    return 1;
  Function *NewAdd = createNewAdd(*M, Ctx);
  replaceAddWithCall(*M, NewAdd);
  std::error_code EC;
  raw_fd_ostream out(argv[2], EC);
  M->print(out, nullptr);
  return 0;
}