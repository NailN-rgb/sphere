#pragma once

#include "../precompile.h"

#include "../DataTypes/Edge/Edge.h"
#include "../DataTypes/Element/Element.h"
#include "../MeshProperties/MeshProperties.h"

#include "SphericalElems/SphericalElems.h"

class Elems
{
    using edges_list = std::vector<Edge>;
    using elems_list = std::vector<Element>;
// entry edges list
private:
    edges_list m_edges;

private:
    vector_3d_points m_points;

private:
    elems_list m_elements;

private:
    index_type m_cylinder_count;
    index_type m_segments_count;
    index_type m_well_offset;
    index_type m_spherical_offset;
    index_type m_cylinder_offset;
    index_type m_mesh_count;
    index_type m_south_sphere_offset;

    MeshProperties m_prop;

private:
    vector_of_indexes m_north_deleted;
    vector_of_indexes m_south_deleted;

public:
    Elems(
        edges_list edges,
        mesh_points_vector const& entry_mesh,
        MeshProperties prop,
        vector_of_indexes const& north_deleted,
        vector_of_indexes const& south_deleted,
        vector_3d_points const& pts // for vtk forming only
    ) :
    m_edges(edges),
    m_prop(prop),
    m_spherical_offset(static_cast<index_type>(entry_mesh.size())),
    m_cylinder_offset(m_cylinder_count * m_segments_count),
    m_north_deleted(north_deleted),
    m_south_deleted(south_deleted),
    m_points(pts)
    {
        set_mesh_properties();
        get_elems_list();
        form_vtk_file();
    }

public:
    void form_vtk_file();

public:
    void get_elems_list();

private:
    void set_mesh_properties();

private:
    void get_elems_at_spherical_part();

private:
    void get_elems_at_cylinder_part();

private:
    void get_inner_elems();
};

#include "detail/Elems.inl"