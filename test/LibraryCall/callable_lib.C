//------------------------------------------------------------------------------
// CLING - the C++ LLVM-based InterpreterG :)
//
// This file is dual-licensed: you can choose to license it under the University
// of Illinois Open Source License or the GNU Lesser General Public License. See
// LICENSE.TXT for details.
//------------------------------------------------------------------------------

// RUN: clang -shared %S/call_lib.c -olibcall_lib%shlibext
// RUN: cat %s | %cling | FileCheck %s
// Callable cannot call into libraries yet: XFAIL: *

.L libcall_lib
extern "C" int cling_testlibrary_function();

#include "cling/Interpreter/Interpreter.h"
#include "cling/Interpreter/Value.h"
#include "cling/Interpreter/Callable.h"
#include "clang/Sema/Sema.h"
#include "clang/AST/ASTContext.h"

const clang::Decl* TU = gCling->Sema().getASTContext().getTranslationUnitDecl();
const clang::FunctionDecl* F = gCling->lookupFunctionProto(TU, "cling_testlibrary_function", "");
assert(F && "cling_testlibrary_function() decl not found");
cling::Callable Call(*F, *gCling);
Call.isValid() // CHECK: (_Bool const) true
std::vector<llvm::GenericValue> ArgVs;
cling::Value V;
Call.Invoke(std::vector<llvm::GenericValue>(), &V);
V // CHECK: (cling::Value) boxes [(int) 66]
