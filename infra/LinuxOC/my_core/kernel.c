typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ADDRESS 0xB8000

volatile uint16_t *vga_buffer = (uint16_t *)VGA_ADDRESS;

enum vga_color
{
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHT_GRAY = 7,
    DARK_GRAY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    YELLOW = 14,
    WHITE = 15
};

int cursor_x = 0;
int cursor_y = 0;

// 1. Вычисление длины строки
int strlen(const char *str)
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

// 2. Сравнение строк
int strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

// 3. Копирование строки
char *strcpy(char *dest, const char *src)
{
    char *d = dest;
    while ((*d++ = *src++))
        ;
    return dest;
}

// 4. Копирование памяти
void *memcpy(void *dest, const void *src, int n)
{
    char *d = (char *)dest;
    const char *s = (const char *)src;
    for (int i = 0; i < n; i++)
    {
        d[i] = s[i];
    }
    return dest;
}

// 5. Заполнение памяти
void *memset(void *ptr, int value, int num)
{
    unsigned char *p = (unsigned char *)ptr;
    for (int i = 0; i < num; i++)
    {
        p[i] = (unsigned char)value;
    }
    return ptr;
}

// Получение цвета VGA
uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | (bg << 4);
}

// Создание символа VGA
uint16_t vga_entry(char c, uint8_t color)
{
    return (uint16_t)c | (uint16_t)color << 8;
}

// Очистка экрана
void clear_screen(enum vga_color bg)
{
    uint8_t color = vga_entry_color(WHITE, bg);
    uint16_t blank = vga_entry(' ', color);

    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
    {
        vga_buffer[i] = blank;
    }
    cursor_x = 0;
    cursor_y = 0;
}

// Вывод символа
void putchar(char c, enum vga_color fg, enum vga_color bg)
{
    if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    else if (c == '\t')
    {
        cursor_x = (cursor_x + 8) & ~7;
    }
    else if (c >= ' ')
    {
        uint8_t color = vga_entry_color(fg, bg);
        vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(c, color);
        cursor_x++;
    }

    if (cursor_x >= VGA_WIDTH)
    {
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y >= VGA_HEIGHT)
    {
        // прокрутка экрана и смещение курсора
        for (int i = 0; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++)
        {
            vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
        }
        for (int i = VGA_WIDTH * (VGA_HEIGHT - 1); i < VGA_WIDTH * VGA_HEIGHT; i++)
        {
            vga_buffer[i] = vga_entry(' ', vga_entry_color(WHITE, bg));
        }
        cursor_y = VGA_HEIGHT - 1;
    }
}

// Вывод строки
void print_string(const char *str, enum vga_color fg, enum vga_color bg)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        putchar(str[i], fg, bg);
    }
}

// Преобразование числа в строку
void int_to_str(int num, char *buffer)
{
    if (num == 0)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    int i = 0;
    int is_negative = 0;

    if (num < 0)
    {
        is_negative = 1;
        num = -num;
    }

    while (num > 0)
    {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    if (is_negative)
    {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    // Разворот строки
    int start = 0;
    int end = i - 1;
    while (start < end)
    {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
}

void demo_functions()
{
    char buffer[50];
    char str1[] = "Hello";
    char str2[] = "World";
    char str3[] = "Hello";
    char copy_buffer[50];

    print_string("\n=== Function Demonstration ===\n", CYAN, BLACK);

    // 1. strlen
    print_string("admin>> strlen(\"Hello\") = ", YELLOW, BLACK);
    int len = strlen(str1);
    int_to_str(len, buffer);
    print_string(buffer, LIGHT_GREEN, BLACK);

    // 2. strcmp
    print_string("\nadmin>> strcmp(\"Hello\", \"World\") = ", YELLOW, BLACK);
    int cmp1 = strcmp(str1, str2);
    int_to_str(cmp1, buffer);
    print_string(buffer, LIGHT_GREEN, BLACK);

    print_string("\nadmin>> strcmp(\"Hello\", \"Hello\") = ", YELLOW, BLACK);
    int cmp2 = strcmp(str1, str3);
    int_to_str(cmp2, buffer);
    print_string(buffer, LIGHT_GREEN, BLACK);

    // 3. strcpy
    print_string("\nadmin>> strcpy: ", YELLOW, BLACK);
    strcpy(copy_buffer, "Copied string");
    print_string("Result: \"", WHITE, BLACK);
    print_string(copy_buffer, LIGHT_GREEN, BLACK);
    print_string("\"", WHITE, BLACK);

    // 4. memcpy
    print_string("\nadmin>> memcpy: ", YELLOW, BLACK);
    char src[] = "Memory test";
    char dest[20];
    memcpy(dest, src, strlen(src) + 1);
    print_string("Result: \"", WHITE, BLACK);
    print_string(dest, LIGHT_GREEN, BLACK);
    print_string("\"", WHITE, BLACK);

    // 5. memset
    print_string("\nadmin>> memset: ", YELLOW, BLACK);
    char arr[10];
    memset(arr, 'X', 9);
    arr[9] = '\0';
    print_string("Result: \"", WHITE, BLACK);
    print_string(arr, LIGHT_GREEN, BLACK);
    print_string("\"", WHITE, BLACK);
}

void kernel_main()
{
    // Очистка экрана
    clear_screen(BLACK);

    print_string("System: x86_64 Kernel\n", GREEN, BLACK);
    print_string("Author: Kristina Polyakova IS-341\n", GREEN, BLACK);
    print_string("Lab: creating a core\n\n", GREEN, BLACK);

    print_string("Implemented functions:\n", MAGENTA, BLACK);
    print_string("  1. strlen() - String length\n", WHITE, BLACK);
    print_string("  2. strcmp() - String comparison\n", WHITE, BLACK); // сравнение
    print_string("  3. strcpy() - String copy\n", WHITE, BLACK);
    print_string("  4. memcpy() - Memory copy\n", WHITE, BLACK);
    print_string("  5. memset() - Memory set\n\n", WHITE, BLACK);

    demo_functions();

    print_string("\n\n========================================\n", CYAN, BLACK);
    print_string("    Kernel execution successful!      \n", YELLOW, BLACK);

    while (1)
    {
        __asm__ volatile("pause");
    }
}