#pragma once

#include "precompile.h"
#include "FlatMesh.h"


class CapsuleMesh
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

    // entry data
private:
    vector_of_points m_flat_mesh;
    vector_of_points m_well_mesh;

    index_type m_segments_count;
    index_type m_cylinder_size;
    index_type m_points_size;
    index_type m_capsule_count;

    value_type m_max_radius;

    // resulted mesh
public:
    mesh_points_vector capsule_mesh;
    elements_vector mesh_elements;

    // flat mesh params
private:
    value_type m_zw               = 5;
    value_type m_lw               = 3;
    value_type m_circle_radius    = 1;
    value_type m_layer_height     = 10;
    vector_of_points m_lgr_points {point(1,0), point(2,0), point(3,0), point(4,0), point(5,0), point(6,0), point(7,0), point(8,0), point(20,0), point(50,0)};   // { point(1,0), point(2,0), point(3,0), point(4,0)};
    index_type m_cylinder_count   = 10;
    index_type m_mesh_count       = 4;

public:
    CapsuleMesh(
        const vector_of_points &flat_mesh,
        const vector_of_points &well_mesh,
        index_type segments_count,
        index_type cylinder_size,
        index_type poins_size,
        index_type capsule_count,
        value_type max_radius
    ) :
    m_flat_mesh(flat_mesh),
    m_well_mesh(well_mesh),
    m_segments_count(segments_count),
    m_cylinder_size(cylinder_size),
    m_points_size(poins_size),
    m_capsule_count(capsule_count),
    m_max_radius(max_radius)
    {
    }

public:
    void set_flat_mesh_params(

    );

public: 
    void form_vtk_file();

public:
    void get_inner_nodes();

public:
    void get_capsular_elements();

public:
    void get_capsular_nodes();

private:
    mesh_points_vector get_capsular_from_flat_mesh();
    
private:
    void add_well_trajectory_to_mesh();

private:
    mesh_points_vector CapsuleMesh::get_mesh_by_layers();

private:
    mesh_points_vector CapsuleMesh::get_capsular_mesh_at_layer(
        value_type circle_radius
    );

private:
    mesh_points_vector CapsuleMesh::rotate_mesh(
        const vector_of_points& mesh,
        int rotations,
        bool is_mesh_with_polar = true
    );

private:
    elements_vector CapsuleMesh::connect_elems();

private:
    elements_vector last_lgr_connection_with_well();

private:
    elements_vector CapsuleMesh::create_polar_mesh_around_well_segment(index_type n_side);

private:
    elements_vector connect_with_pervious_lgr(
        index_type current_lgr_idx
    );

private:
    elements_vector CapsuleMesh::create_between_layer_polar_connection(
        index_type n_side,
        index_type n_layer,
        index_type current_lgr_idx
    );

private:
    elements_vector CapsuleMesh::create_between_layer_side_connection(
        index_type n_side,
        index_type n_layer,
        index_type current_lgr_idx
    );

private:
    elements_vector create_prizmoidal_mesh_around_well(
        index_type n_side
    );

private:
    vector_of_points get_inner_points(
        bool is_upperside_points
    );

};