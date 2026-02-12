CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11 -Wpedantic -I.
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage

SRC_DIR = .
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:.c=.o)
HEADER = matrix.h

LIB_NAME = matrix.a

TESTS_DIR = test
TESTS_SRC = test/test_main.c \
           test/test_eq_matrix.c \
           test/test_sum_sub.c \
           test/test_mult.c \
           test/test_transpose.c \
           test/test_determinant.c \
           test/test_calc_complements.c \
           test/test_inverse.c \
           test/test_create_remove.c
TEST_BIN = test_matrix

GCOV_DIR = gcov_report
SRC_LINTER = ../materials/linters/.clang-format

CHECK_FLAGS = -lcheck -lm -lsubunit -lpthread -lrt

OS = $(shell uname)
ifeq ($(OS), Darwin)
	CHECK_FLAGS = -lcheck -lm
endif

all: $(LIB_NAME)

$(LIB_NAME): $(OBJ)
	ar rc $@ $^
	ranlib $@

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

test: $(LIB_NAME)
	$(CC) $(FLAGS) $(TESTS_SRC) *.o -o $(TEST_BIN) $(CHECK_FLAGS) -I. 
	./$(TEST_BIN)

gcov_report: $(LIB_NAME)
	$(CC) $(FLAGS) $(COVERAGE_FLAGS) $(TESTS_SRC) $(SRC) -o $(TEST_BIN) $(CHECK_FLAGS) -I.
	./$(TEST_BIN)
	mkdir -p $(GCOV_DIR)
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory $(GCOV_DIR)

linter:
	cp $(SRC_LINTER) .clang-format
	clang-format -i $(SRC) $(HEADER) $(TESTS_SRC)

check:
	cp $(SRC_LINTER) .clang-format
	clang-format -n $(SRC) $(HEADER) $(TESTS_SRC)

clean:
	rm -f .clang-format $(TEST_BIN) coverage.info
	rm -rf $(GCOV_DIR)
	rm -f *.gcda *.gcno
	rm -f *.o
	rm -f $(LIB_NAME)

clean_test:
	rm -f $(TEST_BIN) *.gcda *.gcno coverage.info

.PHONY: all test gcov_report linter check clean clean_test