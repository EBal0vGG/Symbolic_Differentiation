#-------------------------
# Флаги сборки и линковки
#-------------------------

CXX = g++

CXXFLAGS =     \
	-std=c++20 \
	-Wall      \
	-Wextra    \
	-Werror

LDFLAGS =
EVAL =

ifeq ($(DEBUG),1)
	CXXFLAGS += -g
else
	CXXFLAGS  += -flto=auto -DNDEBUG
	LDFLAGS   += -flto=auto -fuse-linker-plugin
endif

#-------
# Цвета
#-------

BRED    = \033[1;31m
BGREEN  = \033[1;32m
BYELLOW = \033[1;33m
GREEN   = \033[1;35m
BCYAN   = \033[1;36m
RESET   = \033[0m

#-------
# Файлы
#-------

INCLUDES = \
	include/expression.hpp \
	include/lexer.hpp \
	include/parser.hpp

CXXFLAGS += -I $(abspath include)

SOURCES = \
	src/eval.cpp \
	src/expression.cpp \
	src/lexer.cpp \
	src/parser.cpp

OBJECTS = $(SOURCES:src/%.cpp=build/%.o)

EXECUTABLE = build/eval

#----------------
# Процесс сборки
#----------------

default: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@printf "$(BYELLOW)Linking executable $(BCYAN)$@$(RESET)\n"
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

build/%.o: src/%.cpp $(INCLUDES)
	@printf "$(BYELLOW)Building object file $(BCYAN)$@$(RESET)\n"
	@mkdir -p build
	$(CXX) -c $< $(CXXFLAGS) -o $@

#----------------------
# Вспомогательные цели
#----------------------

run: $(EXECUTABLE)
	@printf "$(BYELLOW)Running executable$(RESET)\n"
	@mkdir -p res
	@./$(EXECUTABLE) --eval "$(EVAL)"

clean:
	@printf "$(BYELLOW)Cleaning build and resource directories$(RESET)\n"
	rm -rf res
	rm -rf build

.PHONY: run clean default