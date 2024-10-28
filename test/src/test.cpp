#include <stdio.h>

int Factorial(int number) {
    int out = 1;

    for (int i = 1; i <= number; ++i) {
        out *= i;
    }

    return out;
}

int PrintFactorial(int number) {
    int nFactorial = Factorial(number);

    printf("number = %d", nFactorial);
    return nFactorial;
}

int main() {
    int number = 5;
    int out;

    out = PrintFactorial(number);

    return out;
}
