#include <stdio.h>
#include "gcf.h"
#include "square.h"

int main() {
    int choice;
    while (1) {
        printf("Enter command (1 for GCF, 2 for Square, 0 to exit): ");
        scanf("%d", &choice);

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            int A, B;
            printf("Enter two numbers: ");
            scanf("%d %d", &A, &B);
            printf("GCF: %d\n", GCF(A, B));
        } else if (choice == 2) {
            float A, B;
            printf("Enter two sides: ");
            scanf("%f %f", &A, &B);
            printf("Square: %f\n", Square(A, B));
        } else {
            printf("Invalid command\n");
        }
    }
    return 0;
}