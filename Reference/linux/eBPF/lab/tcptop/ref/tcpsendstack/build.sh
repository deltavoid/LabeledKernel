
target=TCPSendStack
echo ${target}


CXX_FLAGS="-Wall  -isystem /usr/lib/llvm-3.7/include -O3 -DNDEBUG   -std=gnu++11"

CXX_DEFINES= 

CXX_INCLUDES="-I/usr/lib/llvm-3.7/include/../tools/clang/include -I/home/zqy/WorkSpace/LabeledKernel/Reference/linux/eBPF/bcc/src/cc -I/home/zqy/WorkSpace/LabeledKernel/Reference/linux/eBPF/bcc/src/cc/api "



LLVM_LIB="/usr/lib/llvm-3.7/lib/libclangFrontend.a /usr/lib/llvm-3.7/lib/libclangSerialization.a /usr/lib/llvm-3.7/lib/libclangDriver.a /usr/lib/llvm-3.7/lib/libclangParse.a /usr/lib/llvm-3.7/lib/libclangSema.a /usr/lib/llvm-3.7/lib/libclangCodeGen.a /usr/lib/llvm-3.7/lib/libclangAnalysis.a /usr/lib/llvm-3.7/lib/libclangRewrite.a /usr/lib/llvm-3.7/lib/libclangEdit.a /usr/lib/llvm-3.7/lib/libclangAST.a /usr/lib/llvm-3.7/lib/libclangLex.a /usr/lib/llvm-3.7/lib/libclangBasic.a /usr/lib/llvm-3.7/lib/libLLVMLTO.a /usr/lib/llvm-3.7/lib/libLLVMX86CodeGen.a /usr/lib/llvm-3.7/lib/libLLVMX86Desc.a /usr/lib/llvm-3.7/lib/libLLVMX86Info.a /usr/lib/llvm-3.7/lib/libLLVMMCDisassembler.a /usr/lib/llvm-3.7/lib/libLLVMX86AsmPrinter.a /usr/lib/llvm-3.7/lib/libLLVMX86Utils.a /usr/lib/llvm-3.7/lib/libLLVMPasses.a /usr/lib/llvm-3.7/lib/libLLVMipo.a /usr/lib/llvm-3.7/lib/libLLVMVectorize.a /usr/lib/llvm-3.7/lib/libLLVMOption.a /usr/lib/llvm-3.7/lib/libLLVMObjCARCOpts.a /usr/lib/llvm-3.7/lib/libLLVMMCJIT.a /usr/lib/llvm-3.7/lib/libLLVMExecutionEngine.a /usr/lib/llvm-3.7/lib/libLLVMRuntimeDyld.a /usr/lib/llvm-3.7/lib/libLLVMLinker.a /usr/lib/llvm-3.7/lib/libLLVMIRReader.a /usr/lib/llvm-3.7/lib/libLLVMAsmParser.a /usr/lib/llvm-3.7/lib/libLLVMDebugInfoDWARF.a /usr/lib/llvm-3.7/lib/libLLVMBPFCodeGen.a /usr/lib/llvm-3.7/lib/libLLVMSelectionDAG.a /usr/lib/llvm-3.7/lib/libLLVMBPFDesc.a /usr/lib/llvm-3.7/lib/libLLVMBPFInfo.a /usr/lib/llvm-3.7/lib/libLLVMBPFAsmPrinter.a /usr/lib/llvm-3.7/lib/libLLVMAsmPrinter.a /usr/lib/llvm-3.7/lib/libLLVMCodeGen.a /usr/lib/llvm-3.7/lib/libLLVMTarget.a /usr/lib/llvm-3.7/lib/libLLVMScalarOpts.a /usr/lib/llvm-3.7/lib/libLLVMProfileData.a /usr/lib/llvm-3.7/lib/libLLVMObject.a /usr/lib/llvm-3.7/lib/libLLVMMCParser.a /usr/lib/llvm-3.7/lib/libLLVMBitReader.a /usr/lib/llvm-3.7/lib/libLLVMInstCombine.a /usr/lib/llvm-3.7/lib/libLLVMInstrumentation.a /usr/lib/llvm-3.7/lib/libLLVMTransformUtils.a /usr/lib/llvm-3.7/lib/libLLVMipa.a /usr/lib/llvm-3.7/lib/libLLVMMC.a /usr/lib/llvm-3.7/lib/libLLVMAnalysis.a /usr/lib/llvm-3.7/lib/libLLVMBitWriter.a /usr/lib/llvm-3.7/lib/libLLVMCore.a /usr/lib/llvm-3.7/lib/libLLVMSupport.a"

BCC_PATH="/home/zqy/WorkSpace/LabeledKernel/Reference/linux/eBPF/bcc"



/usr/bin/c++  $CXX_DEFINES $CXX_INCLUDES $CXX_FLAGS -o $target.o -c $target.cc


/usr/bin/c++   -Wall  -isystem /usr/lib/llvm-3.7/include -O3 -DNDEBUG  -rdynamic $target.o  -o $target $BCC_PATH/src/cc/libbcc.a $BCC_PATH/src/cc/frontends/b/libb_frontend.a $BCC_PATH/src/cc/frontends/clang/libclang_frontend.a $BCC_PATH/src/cc/libbpf.a  $LLVM_LIB -lz -lpthread -lffi -ledit -ltinfo -ldl -lm -lelf $BCC_PATH/src/cc/api/libapi-static.a $BCC_PATH/src/cc/usdt/libusdt-static.a $BCC_PATH/src/cc/libbcc-loader-static.a -lelf 

