#ifndef MATH_MOD_H_
#define MATH_MOD_H_

int mod_multiply(int num1, int num2, int mod);
int mod_subtract(int num1, int num2, int mod);
int mod_add(int num1, int num2, int mod);
int is_mod_multiple(int a, int b, int mod);
void convert_to_mod(int number, int mod, int *buffer, int length);
int mod_inner_product(int *a, int *b, int length, int mod);
int is_prime(int number);


#endif
