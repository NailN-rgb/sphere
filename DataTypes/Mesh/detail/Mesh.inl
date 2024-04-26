#include "../Mesh.h"


void Mesh::create_mesh_data()
{
    create_mesh_points();
}


void Mesh::create_mesh_points()
{
    FlatMesh flat_m(m_properties, true);

    flat_m.get_mesh();

    CapsuleMesh caps_m(flat_m, flat_m.well_points, m_properties, 3);

    caps_m.get_capsular_nodes();

    m_mesh_points = caps_m.capsule_mesh;
}


void Mesh::create_mesh_edges()
{
    EdgesMesh mesh_edges(m_mesh_points, m_properties);

    mesh_edges.calculate_edges_list();

    m_edges_mesh = mesh_edges.edges;
}


void Mesh::get_dual_mesh()
{
    // get edge - element matrix

    // by element get edges list

    // by elemet edge stake element points 

    // calculate mass center of element and edge

    // build dual mesh
}



elements_vector Mesh::get_mesh_elements_by_edges()
{
    ElemsByEdges elems(m_edges_mesh);

    elems.get_elems_by_edges(); // elems_by_edges

    return elems.elems_by_edges;
}
