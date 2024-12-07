#ifndef POINT_H
#define POINT_H

typedef struct {
    double x, y, z;
} Point;

#ifdef __cplusplus
extern "C" {
#endif

double point_distance(Point a, Point b);

#ifdef __cplusplus
}
#endif

#endif // POINT_H