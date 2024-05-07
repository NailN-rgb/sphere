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

private:
    bool north_pole_intersected;
    index_type north_deleted_points;
    bool south_pole_intersected;
    index_type south_deleted_points;

public:
    edges_list edges;

public:
    EdgesMesh(
        const mesh_points_vector& resulted_mesh,
        MeshProperties prop,
        const mesh_points_vector& entry_mesh,
        bool north_deleted, // TODO: create struct for this datas
        bool south_deleted,
        index_type north_deleted_count,
        index_type south_deleted_count
    ) :
    m_resulted_mesh(resulted_mesh),
    m_entry_mesh(entry_mesh),
    m_cylinder_count(prop.m_cylinder_count),
    m_segments_count(prop.m_segments),
    m_side(2 * prop.m_points_size + prop.m_cylinder_count),
    m_well_offset(m_cylinder_count),
    m_spherical_offset(m_entry_mesh.size()),
    m_cylinder_offset(m_cylinder_count * m_segments_count),
    north_pole_intersected(north_deleted),
    south_pole_intersected(south_deleted),
    north_deleted_points(north_deleted_count - north_deleted_count % m_segments_count),
    south_deleted_points(north_deleted_count - south_deleted_count % m_segments_count)
    {}

public:
    void calculate_edges_list();

public:
    void write_edges_to_vtk();

private:
    void create_edges_at_well();

private:
    void create_spherical_edges();

private:
    void connect_spreical_points_with_well();

private:
    void create_cylinder_sphere_connection(bool north_pole);

private:
    void create_edges_cylinder_by_equator_lines();

private:
    void create_cylinder_longitude_edges();

private:
    void create_cylidnder_to_mesh_edges();
};

#include "detail/EdgesMesh.inl"