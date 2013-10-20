#include "math_mod.h"

int mod_subtract(int num1, int num2, int mod)
{
	if (num1 >= num2)
		return num1 - num2;

	return mod - num1 + num2;
}

int mod_multiply(int num1, int num2, int mod)
{
    return ((num1 % mod) * (num2 % mod) % mod);
}

int mod_add(int num1, int num2, int mod)
{
    return ((num1 + num2) % mod);
}

int is_mod_multiple(int a, int b, int mod)
{
    for (int i = 1; i < mod; i++)
    {
        if (mod_multiply(i, b, mod) == a)
        {
            return i;
        }
    }
    
    return 0;
}

void convert_to_mod(int number, int mod, int *buffer, int length)
{
    int remainder;
    int quotient;
    
    for (int i = 0; i < length; i++)
    {
        remainder = number % mod;
        quotient = number / mod;
        
        buffer[length - 1 - i] = remainder;
        
        number = quotient;
    }
}

