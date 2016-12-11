cmake_minimum_required(VERSION 3.5.1)

set(SOURCE_FILES Utils.h OpCodes.h OpCodes.cpp)
add_executable(Interpreter ${SOURCE_FILES} InterpreterMain.cpp Interpreter.cpp Interpreter.h)
add_executable(BytecodeTranslator ${SOURCE_FILES} BytecodeTranslatorMain.cpp BytecodeTranslator.cpp BytecodeTranslator.h)
add_executable(Compiler ${SOURCE_FILES} CompilerMain.cpp Compiler.h Compiler.cpp)