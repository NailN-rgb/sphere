#include "precompile.h"

// #include "sphericmesh.h"
// #include "rotatemesh.h"
// #include "compositemesh.h"
// #include "createmesh.h"
// #include "elementsaseemble.h"
// #include "outputmesh.h"

#include "CapsuleMesh.h"
#include "Edges/EdgesMesh.h"

int main()
{
    value_type zw            = 5;
    value_type lw            = 3;
    value_type circle_radius = 1;
    value_type layer_height  = 10;
    vector_of_points lgr_points {point(1,0), point(2,0)}; //{point(1,0), point(2,0), point(3,0), point(4,0), point(5,0), point(6,0), point(7,0), point(8,0), point(20,0), point(50,0)};   // { point(1,0), point(2,0), point(3,0), point(4,0)};
    index_type segments         = 4; // > = 3  !!
    index_type cylinder_count   = 3;
    index_type mesh_count       = 1;

    // read data array and well params

    // create nodes list by 2d mesh

    // call tetGen

    // form VTK

    // get DualMesh Tools

    return 0;
}

