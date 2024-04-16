#include "precompile.h"

#include "sphericmesh.h"
#include "rotatemesh.h"
#include "compositemesh.h"
#include "createmesh.h"
#include "elementsaseemble.h"
#include "outputmesh.h"

int main()
{
    double zw            = 5;
    double lw            = 3;
    double circle_radius = 1;
    double layer_height  = 10;
    vector_of_points lgr_points {point(1,0), point(2,0), point(3,0), point(4,0), point(5,0), point(6,0), point(7,0), point(8,0), point(50,0)};   // { point(1,0), point(2,0), point(3,0), point(4,0)};
    int segments         = 10; // > = 3  !!
    int cylinder_count   = 5;
    int mesh_count       = 1;

    // std::vector<point3d> resulted_mesh = make_composite_mesh(zw, lw, circle_radius, layer_height, lgr_points, segments, cylinder_count, mesh_count);

    std::vector<point3d> resulted_mesh = get_mesh_nodes(zw, lw, circle_radius, layer_height, lgr_points, segments, cylinder_count, mesh_count);

    std::vector<std::vector<double>> elems = connect_elems(resulted_mesh, mesh_count, cylinder_count, lgr_points.size(), segments);

    form_vtk_file(resulted_mesh, elems);

    return 0;
}

