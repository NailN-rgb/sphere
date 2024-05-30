#include "precompile.h"

#include "2DMesh/2DMesh.h"
#include "MeshProperties/MeshProperties.h"
#include "NodeMesh/NodeMesh.h"
#include "Edges/EdgesMesh.h"

int main()
{
    value_type zw               = 5;
    value_type lw               = 0.5;
    value_type circle_radius    = 1;
    value_type layer_height     = 10;
    vector_of_points lgr_points {point(1,0), point(2,0)}; //{point(1,0), point(2,0), point(3,0), point(4,0), point(5,0), point(6,0), point(7,0), point(8,0), point(20,0), point(50,0)};   // { point(1,0), point(2,0), point(3,0), point(4,0)};
    index_type segments         = 12; // > = 3  !!
    index_type cylinder_count   = 5;
    index_type mesh_count       = 5;
    value_type max_radius       = 15.0636;
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
                         node_mesh.south_sphere_deleted_count,
                         node_mesh.north_pole_points_depth,
                         node_mesh.south_pole_points_depth);

    edges_mesh.calculate_edges_list();

    return 0;
}

