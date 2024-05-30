#pragma once

#include "../precompile.h"

#include "../DataTypes/Edge/Edge.h"
#include "../DataTypes/Element/Element.h"
#include "../MeshProperties/MeshProperties.h"


class EdgesMesh
{
    using edges_list = std::vector<Edge>;
private:
    mesh_points_vector m_entry_mesh;
    mesh_points_vector m_resulted_mesh;
    index_type m_cylinder_count;
    index_type m_segments_count;
    index_type m_side;
    index_type m_well_offset;
    index_type m_spherical_offset;
    index_type m_cylinder_offset;
    index_type m_mesh_count;

private:
    std::vector<bool> north_pole_intersected;
    vector_of_indexes north_deleted_points;
    std::vector<bool> south_pole_intersected;
    vector_of_indexes south_deleted_points;

private:
    vector_of_indexes north_pole_points_depth;
    vector_of_indexes south_pole_points_depth;

private:
    vector_of_indexes north_pole_radia_count;
    vector_of_indexes south_pole_radia_count;

public:
    edges_list edges;

public:
    EdgesMesh(
        mesh_points_vector const& resulted_mesh,
        MeshProperties const& prop,
        mesh_points_vector const& entry_mesh,
        std::vector<bool> const& north_deleted, // TODO: create struct for this datas
        std::vector<bool> const& south_deleted,
        vector_of_indexes const& north_deleted_count,
        vector_of_indexes const& south_deleted_count,
        vector_of_indexes const& north_pole_depth,
        vector_of_indexes const& south_pole_depth
    ) :
    m_entry_mesh(entry_mesh),
    m_resulted_mesh(resulted_mesh),
    m_cylinder_count(prop.m_cylinder_count),
    m_segments_count(prop.m_segments),
    m_side(static_cast<index_type>(2 * prop.m_points_size + prop.m_cylinder_count)),
    m_well_offset(m_cylinder_count),
    m_spherical_offset(static_cast<index_type>(m_entry_mesh.size())),
    m_cylinder_offset(m_cylinder_count * m_segments_count),
    m_mesh_count(prop.m_mesh_count),
    north_pole_intersected(north_deleted),
    north_deleted_points(north_deleted_count),
    south_pole_intersected(south_deleted),
    south_deleted_points(south_deleted_count),
    north_pole_points_depth(north_pole_depth),
    south_pole_points_depth(south_pole_depth)
    {
        get_spherical_radial_count();
    }

public:
    void calculate_edges_list();

public:
    void write_edges_to_vtk();

private:
    void create_edges_at_well();

private: 
    void create_cylinder_edges(index_type mesh_index);

private:
    void connect_spherical_points_with_well_at_north_pole(index_type layer);

private:
    void connect_spreical_points_with_well_at_south_pole(index_type mesh_index);

private:
    void create_cylinder_sphere_connection(bool north_pole, index_type mesh_index);

private:
    void create_spherical_edges_at_north_pole(index_type mesh_index);

private:
    void create_spherical_edges_at_south_pole(index_type mesh_index); 

private:
    void create_edges_cylinder_by_equator_lines(index_type mesh_index);

private:
    void create_cylinder_longitude_edges(index_type mesh_index);

private:
    void create_cylidnder_to_well_edges(index_type mesh_index);

private:
    void create_outer_mesh_edges_at_north();

private:
    void create_outer_mesh_edges_at_south();

// Utils

private:
    index_type get_number_of_spherical_points();
    index_type total_deleted_points_north(index_type mesh_layer = 0);
    index_type total_deleted_points_south(index_type mesh_layer = 0);
    index_type total_deleted_points_south();
    index_type total_deleted_points_to_layer_north(index_type mesh_layer);
    void get_spherical_radial_count();

};

#include "detail/EdgesMesh.inl"