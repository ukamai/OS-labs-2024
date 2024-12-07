#include "gcf.h"

int GCF(int A, int B) {
    int gcf = 1;
    for (int i = 1; i <= A && i <= B; i++) {
        if (A % i == 0 && B % i == 0) {
            gcf = i;
        }
    }
    return gcf;
}