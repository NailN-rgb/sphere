#include "precompile.h"

// #include "sphericmesh.h"
// #include "rotatemesh.h"
// #include "compositemesh.h"
// #include "createmesh.h"
// #include "elementsaseemble.h"
// #include "outputmesh.h"

//#include "CapsuleMesh.h"
//#include "Edges/EdgesMesh.h"

#include "2DMesh/2DMesh.h"
#include "MeshProperties/MeshProperties.h"
#include "NodeMesh/NodeMesh.h"
#include "Edges/EdgesMesh.h"

int main()
{
    value_type zw               = 3;
    value_type lw               = 3;
    value_type circle_radius    = 1;
    value_type layer_height     = 10;
    vector_of_points lgr_points {point(1,0), point(2,0)}; //{point(1,0), point(2,0), point(3,0), point(4,0), point(5,0), point(6,0), point(7,0), point(8,0), point(20,0), point(50,0)};   // { point(1,0), point(2,0), point(3,0), point(4,0)};
    index_type segments         = 12; // > = 3  !!
    index_type cylinder_count   = 10;
    index_type mesh_count       = 1;
    value_type max_radius       = 2.;
    fs::path path_to_2d_mesh    = "../data/mesh.txt";

    // create MeshProperty
    MeshProperties mp(zw, 
        lw,
        circle_radius, 
        layer_height, 
        lgr_points,
        segments,
        cylinder_count,
        mesh_count,
        max_radius
    );

    // read data array and well params

    TwoDMesh mesh_read(path_to_2d_mesh);

    // create nodes list by 2d mesh

    NodeMesh node_mesh(mesh_read.get_points_list(), mp);
    node_mesh.create_Nodes();

    EdgesMesh edges_mesh(node_mesh.m_nodes,
                         mp, 
                         mesh_read.get_points_list(), 
                         node_mesh.north_deleted, 
                         node_mesh.south_deleted, 
                         node_mesh.north_sphere_deleted_count, 
                         node_mesh.south_sphere_deleted_count);

    edges_mesh.calculate_edges_list();

    return 0;
}

