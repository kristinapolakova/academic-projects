#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "decimal.h"

void check_decimal_from_int(int src, decimal *dec, int expected_sign,
                            int expected_scale);
void check_decimal_float(float src, decimal *dec, int expected_sign,
                         int expected_scale);
Suite *suite_from_int_to_decimal(void);
Suite *suite_from_float_to_decimal(void);

Suite *suite_from_decimal_to_int(void);
Suite *suite_from_decimal_to_float(void);

Suite *suite_mul(void);
Suite *suite_sub(void);

Suite *compare_suite(void);
Suite *suite_extra_functions(void);

Suite *suite_add(void);
