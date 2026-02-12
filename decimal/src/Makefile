# Компилятор и флаги
CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11 -Wpedantic -I.
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage

# Пути и файлы
SRC_DIR = source
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:.c=.o)
HEADER = decimal.h

LIB_NAME = decimal.a

TESTS_DIR = tests
TESTS_SRC = $(wildcard $(TESTS_DIR)/*.c)
TEST_BIN = test_decimal

GCOV_DIR = gcov_report
SRC_LINTER = ../materials/linters/.clang-format

# Флаги для check
CHECK_FLAGS = -lcheck -lm -lsubunit -lpthread -lrt

# Определение ОС для корректных флагов
OS = $(shell uname)
ifeq ($(OS), Darwin)
	CHECK_FLAGS = -lcheck -lm -I /opt/homebrew/Cellar/check/0.15.2/include -L /opt/homebrew/Cellar/check/0.15.2/lib
endif

# Сборка библиотеки
all: $(LIB_NAME)

$(LIB_NAME): $(OBJ)
	ar rc $@ $^
	ranlib $@

# Сборка object-файлов
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(FLAGS) -c $< -o $@

# Сборка и запуск тестов
test: $(LIB_NAME)
	$(CC) $(FLAGS) $(TESTS_SRC) source/*.o -o $(TEST_BIN) $(CHECK_FLAGS) -I. 
	./$(TEST_BIN)  true

# Покрытие
gcov_report: $(LIB_NAME)
	$(CC) $(FLAGS) $(COVERAGE_FLAGS) $(TESTS_SRC) $(SRC) -o $(TEST_BIN) $(CHECK_FLAGS) -I.
	./$(TEST_BIN)  true
	mkdir -p $(GCOV_DIR)
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory $(GCOV_DIR)
	
# Линтер
linter:
	cp $(SRC_LINTER) .clang-format
	clang-format -i $(SRC) $(HEADER) $(TESTS_SRC)

check:
	cp $(SRC_LINTER) .clang-format
	clang-format -n $(SRC) $(HEADER) $(TESTS_SRC)

# Очистка
clean:
	rm -f .clang-format $(TEST_BIN) coverage.info
	rm -rf $(GCOV_DIR)
	rm -f *.gcda *.gcno
	rm -f $(SRC_DIR)/*.o
	rm -f $(LIB_NAME)

clean_test:
	rm -f $(TEST_BIN) *.gcda *.gcno coverage.info

.PHONY: all test gcov_report linter check clean clean_test