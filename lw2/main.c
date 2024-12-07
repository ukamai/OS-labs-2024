#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include "point.h"
#include "triangle.h"
#include "thread_pool.h"

typedef struct {
    Point a, b, c;
    double area;
} TriangleResult;

void calculate_triangle_area(void* arg) {
    TriangleResult* result = (TriangleResult*)arg;
    result->area = triangle_area(result->a, result->b, result->c);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <max_threads> <input_file>\n", argv[0]);
        return 1;
    }

    int max_threads = atoi(argv[1]);
    char* input_file = argv[2];

    FILE* file = fopen(input_file, "r");
    if (!file) {
        perror("Failed to open input file");
        return 1;
    }

    Point points[1000];
    int point_count = 0;
    while (fscanf(file, "%lf %lf %lf", &points[point_count].x, &points[point_count].y, &points[point_count].z) == 3) {
        point_count++;
        if (point_count >= 1000) {
            fprintf(stderr, "Too many points in the input file. Maximum is 1000.\n");
            fclose(file);
            return 1;
        }
    }
    fclose(file);

    if (point_count == 0) {
        fprintf(stderr, "No points found in the input file.\n");
        return 1;
    }

    ThreadPool pool;
    thread_pool_init(&pool, max_threads);

    TriangleResult* results = malloc(point_count * (point_count - 1) * (point_count - 2) / 6 * sizeof(TriangleResult));
    if (!results) {
        perror("Failed to allocate memory for results");
        thread_pool_destroy(&pool);
        return 1;
    }

    int result_count = 0;

    for (int i = 0; i < point_count; ++i) {
        for (int j = i + 1; j < point_count; ++j) {
            for (int k = j + 1; k < point_count; ++k) {
                TriangleResult* result = &results[result_count++];
                result->a = points[i];
                result->b = points[j];
                result->c = points[k];
                thread_pool_enqueue(&pool, calculate_triangle_area, result);
            }
        }
    }

    thread_pool_wait(&pool);

    TriangleResult* max_triangle = NULL;
    for (int i = 0; i < result_count; ++i) {
        if (!max_triangle || results[i].area > max_triangle->area) {
            max_triangle = &results[i];
        }
    }

    if (max_triangle) {
        printf("Maximum area triangle: (%lf, %lf, %lf), (%lf, %lf, %lf), (%lf, %lf, %lf) with area %lf\n",
               max_triangle->a.x, max_triangle->a.y, max_triangle->a.z,
               max_triangle->b.x, max_triangle->b.y, max_triangle->b.z,
               max_triangle->c.x, max_triangle->c.y, max_triangle->c.z,
               max_triangle->area);
    } else {
        printf("No triangles found.\n");
    }

    free(results);
    thread_pool_destroy(&pool);

    return 0;
}