#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

typedef int (*GCF_func)(int, int);
typedef float (*Square_func)(float, float);

void* handle = NULL;
GCF_func GCF = NULL;
Square_func Square = NULL;

void load_library(const char* lib_path) {
    if (handle) {
        dlclose(handle);
    }
    handle = dlopen(lib_path, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Error: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    GCF = (GCF_func)dlsym(handle, "GCF");
    Square = (Square_func)dlsym(handle, "Square");
    if (!GCF || !Square) {
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(handle);
        exit(EXIT_FAILURE);
    }
}

int main() {
    int choice;
    while (1) {
        printf("Enter command (0 to switch library, 1 for GCF, 2 for Square, 0 to exit): ");
        scanf("%d", &choice);

        if (choice == 0) {
            char lib_path[256];
            printf("Enter library path: ");
            scanf("%s", lib_path);
            load_library(lib_path);
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
    if (handle) {
        dlclose(handle);
    }
    return 0;
}