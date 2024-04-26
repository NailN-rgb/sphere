#pragma once

#include "../precompile.h"

#include "../DataTypes/Edge/Edge.h"
#include "../DataTypes/Element/Element.h"

/**
 * This class create a list of capsular mesh edges
 * TODO: Wire realization for more than one layer mesh
*/

class EdgesMesh
{
    typedef bg::model::point<double, 3, bg::cs::cartesian> point3d;
    
    typedef double value_type;
    typedef int index_type;
    
    typedef std::vector<value_type> vector_of_values;
    typedef std::vector<value_type> vector_of_indexes;

    typedef std::vector<point3d> mesh_points_vector;

    typedef std::vector<Edge> edges_list;

private:
    mesh_points_vector m_entry_mesh;
    index_type m_cylinder_count;
    index_type m_points_count;
    index_type m_segments_count;
    index_type m_side;


public:
    edges_list edges;

public:
    EdgesMesh(
        const mesh_points_vector& entry_mesh,
        index_type cylinder_count,
        index_type points_count,
        index_type segments_count
    ) :
    m_entry_mesh(entry_mesh),
    m_cylinder_count(cylinder_count),
    m_points_count(points_count),
    m_segments_count(segments_count),
    m_side(2 * points_count + cylinder_count)
    {}


public:
    EdgesMesh(
        const mesh_points_vector& entry_mesh,
        MeshProperties prop
    ) :
    m_entry_mesh(entry_mesh),
    m_cylinder_count(prop.m_cylinder_count),
    m_points_count(prop.m_points_size),
    m_segments_count(prop.m_segments),
    m_side(2 * prop.m_points_size + prop.m_cylinder_count)
    {}

public:
    void calculate_edges_list();

public:
    void write_edges_to_vtk();

private:
    void create_edges_at_well();

private:
    void connect_edges_with_well_trajectory();

private:
    void connect_polar_nodes();

private:
    void connect_side_nodes();

private:
    void connect_segments();
};

#include "detail/EdgesMesh.inl"