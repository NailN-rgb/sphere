#pragma once

#include "../precompile.h"

/**
 * This class create a list of capsular mesh edges
*/

class Edges
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
    typedef std::vector<vector_of_values> elements_vector;

    typedef std::vector<vector_of_values> edges_list;

private:
    mesh_points_vector m_entry_mesh;
    index_type m_cylinder_count;
    index_type m_points_count;
    index_type m_segments_count;
    index_type m_side;
    index_type m_segment;

    index_type m_current_position = 0;

public:
    edges_list edges;

public:
    Edges(
        mesh_points_vector entry_mesh,
        index_type cylinder_count,
        index_type points_count,
        index_type segments_count
    ) :
    m_entry_mesh(entry_mesh),
    m_cylinder_count(cylinder_count),
    m_points_count(points_count),
    m_segments_count(segments_count)
    {
        m_side = 2 * points_count + cylinder_count; 
    }

public:
    void get_edges_list();

public:
    void write_edges_to_vtk();

private:
    void create_edges_around_well();

private:
    void connect_polar_nodes();

private:
    void connect_side_nodes();

private:
    void connect_segments();
};

#include "detail/Edges.inl"