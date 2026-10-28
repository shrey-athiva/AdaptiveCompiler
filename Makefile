# Build the compiler:  make        Run tests:  make test        Clean:  make clean
#
# Needs: g++ (C++17), flex, bison, make.
#   Ubuntu/Debian : sudo apt install g++ flex bison make
#   MSYS2 (UCRT64): pacman -S mingw-w64-ucrt-x86_64-gcc flex bison make

CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wno-unused-function
TARGET   := ircompiler

# On Windows/MSYS2, link statically so ircompiler.exe runs without the MSYS2
# runtime DLLs on PATH (this is what app.py's old MSYS2_BIN workaround was for).
ifeq ($(OS),Windows_NT)
LDFLAGS  += -static
endif

CPP_SRCS := semantic/symbol_table.cpp recovery/recovery.cpp ir/ir_generator.cpp

.PHONY: all test clean
all: $(TARGET)

parser/parser.tab.c parser/parser.tab.h: parser/parser.y
	bison -Wall -d -o parser/parser.tab.c $<

lexer/lex.yy.c: lexer/lexer.l parser/parser.tab.h
	flex -o $@ $<

# The generated .c files contain C++ (std::string), so compile them as C++.
$(TARGET): parser/parser.tab.c lexer/lex.yy.c $(CPP_SRCS) \
           semantic/symbol_table.h semantic/types.h recovery/recovery.h ir/ir_generator.h
	$(CXX) $(CXXFLAGS) -x c++ parser/parser.tab.c lexer/lex.yy.c -x c++ $(CPP_SRCS) $(LDFLAGS) -o $@

test: $(TARGET)
	python3 -m unittest discover -s tests -v

clean:
	rm -f $(TARGET) $(TARGET).exe parser/parser.tab.c parser/parser.tab.h lexer/lex.yy.c *.o
