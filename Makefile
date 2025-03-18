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

GTFLAGS=-lgtest -lgtest_main -lpthread
PATH_TO_GTEST=/wsl.localhost/Ubuntu/usr

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
	include/utils.hpp \
	include/expression.hpp \
	include/lexer.hpp \
	include/parser.hpp

CXXFLAGS += -I $(abspath include)

SOURCES = \
	src/eval.cpp \
	src/expression.cpp \
	src/lexer.cpp \
	src/parser.cpp \
	src/test_lib.cpp

OBJECTS = $(SOURCES:src/%.cpp=build/%.o)

EX_OBJECTS = $(filter-out build/test_lib.o, $(OBJECTS))
TEST_OBJECTS = $(filter-out build/eval.o, $(OBJECTS))

EXECUTABLE = build/differentiator
TESTS = build/tests

#----------------
# Процесс сборки
#----------------

default: $(EXECUTABLE)

$(EXECUTABLE): $(EX_OBJECTS)
	@printf "$(BYELLOW)Linking executable $(BCYAN)$@$(RESET)\n"
	$(CXX) $(LDFLAGS) $(EX_OBJECTS) -o $@

$(TESTS): $(TEST_OBJECTS)
	@printf "$(BYELLOW)Linking executable $(BCYAN)$@$(RESET)\n"
	$(CXX) $(LDFLAGS) $(TEST_OBJECTS) -o $@ $(GTFLAGS)

build/%.o: src/%.cpp $(INCLUDES)
	@printf "$(BYELLOW)Building object file $(BCYAN)$@$(RESET)\n"
	@mkdir -p build
	$(CXX) -c $< $(CXXFLAGS) -o $@

#----------------------
# Вспомогательные цели
#----------------------

eval: $(EXECUTABLE)
	@printf "$(BYELLOW)Running in eval mode$(RESET)\n"
	@./$(EXECUTABLE) --eval "$(expression)" $(variables)

diff: $(EXECUTABLE)
	@printf "$(BYELLOW)Running in diff mode$(RESET)\n"
	@./$(EXECUTABLE) --diff "$(expression)" --by $(variable)

test: $(TESTS)
	@printf "$(BYELLOW)Testing functions$(RESET)\n"
	@./$(TESTS)

clean:
	@printf "$(BYELLOW)Cleaning build and resource directories$(RESET)\n"
	rm -rf res
	rm -rf build

.PHONY: run clean default eval diff