#include "precompile.h"

#include "sphericmesh.h"
#include "rotatemesh.h"
#include "compositemesh.h"
#include "createmesh.h"
#include "elementsaseemble.h"
#include "outputmesh.h"

int main()
{
    value_type zw            = 5;
    value_type lw            = 3;
    value_type circle_radius = 1;
    value_type layer_height  = 10;
    vector_of_points lgr_points {point(1,0), point(2,0), point(3,0), point(4,0), point(5,0), point(6,0), point(7,0), point(8,0), point(50,0)};   // { point(1,0), point(2,0), point(3,0), point(4,0)};
    index_type segments         = 10; // > = 3  !!
    index_type cylinder_count   = 5;
    index_type mesh_count       = 1;

    // std::vector<point3d> resulted_mesh = make_composite_mesh(zw, lw, circle_radius, layer_height, lgr_points, segments, cylinder_count, mesh_count);

    mesh_points_vector resulted_mesh = get_mesh_nodes(zw, lw, circle_radius, layer_height, lgr_points, segments, cylinder_count, mesh_count);

    elements_vector elems = connect_elems(resulted_mesh, mesh_count, cylinder_count, lgr_points.size(), segments);

    form_vtk_file(resulted_mesh, elems);

    return 0;
}

