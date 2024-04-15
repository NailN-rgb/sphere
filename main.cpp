#include "outputmesh.h"
#include "rotatemesh.h"
#include "sphericmesh.h"
#include "compositemesh.h"

int main()
{
    double zw            = 5;
    double lw            = 3;
    double circle_radius = 1;
    double layer_height  = 10;
    std::vector<point> lgr_points{point(3,0), point(4,0), point(5,0), point(6,0), point(7,0), point(8,0)};   // { point(1,0), point(2,0), point(3,0), point(4,0)};
    int segments         = 35; // > = 3  !!
    int cylinder_count   = 15;
    int mesh_count       = 1;

    make_composite_mesh(zw, lw, circle_radius, layer_height, lgr_points, segments, cylinder_count, mesh_count);


    return 0;
}

