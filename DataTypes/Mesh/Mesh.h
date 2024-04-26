#pragma once 

#include "../../precompile.h"
#include "../../CapsuleMesh.h"
#include "../Edge/Edge.h"
#include "../Element/Element.h"
#include "../../Edges/EdgesMesh.h"
#include "../../DualFVMesh/DualFVMesh.h"
#include "../../MeshProperties/MeshProperties.h"
#include "../../MeshTools/ElemsByEdges/ElemsByEdges.h"



/**
 * This class mesthod create mesh from entry data
*/
class Mesh
{
    typedef bg::model::point<double, 2, bg::cs::cartesian> point;
    typedef bg::model::point<double, 3, bg::cs::cartesian> point3d;

    typedef bg::model::polygon<point> polygon;
    typedef bg::model::segment<point> segment;
    
    typedef double value_type;
    typedef int index_type;
    typedef size_t size_type;

    typedef std::vector<value_type> vector_of_values;

    typedef std::vector<point3d> mesh_points_vector;
    typedef std::vector<point> vector_of_points;
    typedef std::vector<Element> elements_vector;

    typedef std::vector<Edge> edges_list;


private: 
    mesh_points_vector m_mesh_points;
    edges_list m_edges_mesh;
    mesh_points_vector m_dual_mesh;
    MeshProperties m_properties;

// main function
public: 
    void create_mesh_data();

private:
    void create_mesh_points();

private:
    void create_mesh_edges();

private:
    elements_vector get_mesh_elements_by_edges();
    
private:
    void get_dual_mesh();

};