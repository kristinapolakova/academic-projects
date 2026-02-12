bits 32

section .multiboot
align 4
multiboot_header:
    dd 0x1BADB002              ; Multiboot 1
    dd 0x00000003              ; выравнивание+карта памяти?
    dd -(0x1BADB002 + 0x00000003) ; Контрольная сумма

global _start
extern kernel_main

section .text
_start:
    ; Сохраняем multiboot информацию
    mov edi, ebx               ; структура мультизагрузки
    
    ; Очистка направления строк
    cld
    
    ; Настройка стека
    mov esp, stack_top
    
    ; Проверяем, поддерживает ли процессор long mode (64-bit)
    call check_long_mode
    
    ; Настраиваем страничную организацию памяти
    call setup_page_tables
    call enable_paging
    
    ; Загружаем GDT для 64-bit
    lgdt [gdt64.pointer]
    
    ; Переход в 64-bit режим
    jmp gdt64.code:long_mode_start

bits 64
long_mode_start:
    ; Обновляем сегментные регистры
    mov ax, gdt64.data
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ; Обновляем указатель стека для 64-bit
    mov rsp, stack_top
    
    ; Вызываем основную функцию ядра
    call kernel_main
    
    ; Если ядро вернется
    cli
.hang:
    hlt
    jmp .hang

bits 32
check_long_mode:
    ; Проверка поддержки extended processor info
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jb .no_long_mode
    
    ; Проверка поддержки long mode
    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29
    jz .no_long_mode
    ret
    
.no_long_mode:
    mov al, "L"
    jmp error

setup_page_tables:
    ; Очистка таблиц страниц
    mov edi, page_table_l4
    mov ecx, 4096
    xor eax, eax
    rep stosb
    
    ; Настройка L4 таблицы
    mov eax, page_table_l3
    or eax, 0b11 ; Present + Writable
    mov [page_table_l4], eax
    
    ; Настройка L3 таблицы
    mov eax, page_table_l2
    or eax, 0b11 ; Present + Writable
    mov [page_table_l3], eax
    
    ; Настройка L2 таблицы (identity mapping 2MB)
    mov ecx, 0
.map_l2_table:
    mov eax, 0x200000  ; 2MB
    mul ecx
    or eax, 0b10000011 ; Present + Writable + Huge page
    mov [page_table_l2 + ecx * 8], eax
    
    inc ecx
    cmp ecx, 512
    jne .map_l2_table
    
    ret

enable_paging:
    ; Загружаем адрес L4 таблицы в cr3
    mov eax, page_table_l4
    mov cr3, eax
    
    ; Включаем PAE (Physical Address Extension)
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax
    
    ; Включаем long mode
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr
    
    ; Включаем пейджинг
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax
    
    ret

error:
    ; Вывод сообщения об ошибке в VGA
    mov dword [0xB8000], 0x4F524F45
    mov dword [0xB8004], 0x4F3A4F52
    mov dword [0xB8008], 0x4F204F20
    mov byte [0xB800A], al
    hlt

section .bss
align 4096
page_table_l4:
    resb 4096
page_table_l3:
    resb 4096
page_table_l2:
    resb 4096

stack_bottom:
    resb 16384
stack_top:

section .rodata
gdt64:
    dq 0 ; нулевой дескриптор
.code: equ $ - gdt64
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53) ; код
.data: equ $ - gdt64
    dq (1 << 44) | (1 << 47) | (1 << 41) ; данные
.pointer:
    dw $ - gdt64 - 1
    dq gdt64