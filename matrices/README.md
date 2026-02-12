# matrix

Реализация собственной библиотеки для работы с матрицами на языке C.

## Описание проекта

Библиотека предоставляет набор функций для создания, обработки и выполнения операций над числовыми матрицами. Матрицы являются базовой структурой данных в программировании и широко применяются в компьютерной графике, вычислительных задачах и нейронных сетях.

### Структура матрицы на языке C:

```c
typedef struct matrix_struct {
    double** matrix;
    int rows;
    int columns;
} matrix_t;
```

## Операции над матрицами

Все операции (кроме сравнения матриц) должны возвращать результирующий код:  
- 0 — OK;
- 1 — Ошибка, некорректная матрица;
- 2 — Ошибка вычисления (несовпадающие размеры матриц; матрица, для которой нельзя провести вычисления и т. д.).

### Создание матриц (create_matrix)

```c
int create_matrix(int rows, int columns, matrix_t *result);
```

### Очистка матриц (remove_matrix)

```c
void remove_matrix(matrix_t *A);
```

### Сравнение матриц (eq_matrix)

```c
#define SUCCESS 1
#define FAILURE 0

int eq_matrix(matrix_t *A, matrix_t *B);
```

Сравнение должно происходит вплоть до шестого знака после запятой включительно.

### Сложение (sum_matrix) и вычитание матриц (sub_matrix)

```c
int sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
```

### Умножение матрицы на число (mult_number). Умножение двух матриц (mult_matrix)

```c
int mult_number(matrix_t *A, double number, matrix_t *result);
int mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
```

### Транспонирование матрицы (transpose)

```c
int transpose(matrix_t *A, matrix_t *result);
```

### Минор матрицы и матрица алгебраических дополнений (calc_complements)

```c
int calc_complements(matrix_t *A, matrix_t *result);
```

### Определитель матрицы (determinant)

```c
int determinant(matrix_t *A, double *result);
```

### Обратная матрица (inverse_matrix)

```c
int inverse_matrix(matrix_t *A, matrix_t *result);
```

# Сборка и тестирование

```bash
# Сборка статической библиотеки
make matrix.a

# Сборка и запуск тестов
make test

# Запуск с отчётом о покрытии
make gcov_report
```

## Цели Makefile:

    all — полная сборка

    clean — очистка

    test — запуск unit-тестов (библиотека Check)

    matrix.a — сборка статической библиотеки

    gcov_report — формирование HTML-отчёта о покрытии кода

## Пример использования:


```c
matrix_t A, B, result;
create_matrix(3, 3, &A);
create_matrix(3, 3, &B);

// Заполнение матриц...
A.matrix[0][0] = 1; A.matrix[0][1] = 2; A.matrix[0][2] = 3;
// ...

sum_matrix(&A, &B, &result);
// Работа с result...

remove_matrix(&A);
remove_matrix(&B);
remove_matrix(&result);
```