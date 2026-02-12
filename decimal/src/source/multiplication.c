#include <stdint.h>

#include "decimal.h"

uint32_t div96by10(
    uint32_t bits[3]) {  // деление мантиссы на 10 с вовзратом остатка
  uint64_t remainder = 0;

  for (int i = 2; i >= 0; i--) {
    uint64_t cur = (remainder << 32) | bits[i];
    bits[i] = (uint32_t)(cur / 10);
    remainder = cur % 10;
  }
  return (uint32_t)remainder;
}

int add1to96(
    uint32_t bits[3]) {  // добавление 1 к мантиссе с отслеживанием переполнения
  int exit_code = 0;
  uint64_t tmp = (uint64_t)bits[0] + 1;
  bits[0] = (uint32_t)tmp;
  return exit_code;
}

int mul(decimal value_1, decimal value_2, decimal *result) {
  int exit_code = 0;
  init_decimal(result);

  uint32_t a[3] = {value_1.bits[0], value_1.bits[1], value_1.bits[2]};
  uint32_t b[3] = {value_2.bits[0], value_2.bits[1], value_2.bits[2]};
  uint64_t res[6] = {0};

  for (int i = 0; i < 3; i++) {
    uint64_t carry = 0;  // перенос в след. разряд
    for (int j = 0; j < 3; j++) {
      uint64_t tmp = res[i + j] + (uint64_t)a[i] * b[j] + carry;
      res[i + j] =
          (uint32_t)(tmp &
                     0xFFFFFFFFu);  // записываем младшие 32 бита результата
      carry = tmp >> 32;  // старшие 32 бита = перенос в след. разряд
    }
    res[i + 3] += carry;  // перенос на следующий блок
  }

  // Проверка переполнения (результат должен влезть в 96 бит)
  if (res[3] || res[4] || res[5]) {
    exit_code = 1;  // число слишком велико
  }

  result->bits[0] = (uint32_t)res[0];
  result->bits[1] = (uint32_t)res[1];
  result->bits[2] = (uint32_t)res[2];
  int sign = get_decimal_sign(value_1) ^ get_decimal_sign(value_2);
  int scale = get_decimal_scale(value_1) + get_decimal_scale(value_2);
  while (scale > 28 && !exit_code) {
    uint32_t remainder = div96by10((uint32_t *)&result->bits[0]);
    scale--;

    // Банковское округление
    if (remainder > 5 || (remainder == 5 && (result->bits[0] & 1))) {
      exit_code = add1to96(
          (uint32_t *)&result->bits[0]);  // Потом заменить на add ниже
      // exit_code = add(*result, (decimal) {{1,0,0,0}}, result);
    }

    // Проверяем, остались ли значащие цифры
    if (!result->bits[0] && !result->bits[1] && !result->bits[2]) {
      exit_code = 2;  // число слишком мало
    }
  }
  *result = set_decimal_scale(*result, scale);
  *result = set_decimal_sign(*result, sign);

  return exit_code;
}