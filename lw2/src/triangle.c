#include "triangle.h"
#include "point.h"
#include <math.h>

double triangle_area(Point a, Point b, Point c) {
    double ab = point_distance(a, b);
    double bc = point_distance(b, c);
    double ca = point_distance(c, a);
    double s = (ab + bc + ca) / 2;
    return sqrt(s * (s - ab) * (s - bc) * (s - ca));
}