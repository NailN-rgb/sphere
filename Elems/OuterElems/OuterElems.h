#pragma once

#include "../../precompile.h"

#include "../../DataTypes/Edge/Edge.h"
#include "../../DataTypes/Element/Element.h"
#include "../../MeshProperties/MeshProperties.h"

class OuterElems
{
    using edges_list = std::vector<Edge>;
    using elems_list = std::vector<Element>;
private:
    edges_list m_edges;
    index_type m_to_sphere_offset;
    vector_of_indexes m_north_deleted;
    vector_of_indexes m_south_deleted;
    MeshProperties m_prop;

private:
    index_type m_segments_count;
    index_type m_mesh_count;
    index_type m_spherical_offset;
    index_type m_well_offset;
    index_type m_cylinder_offset;
    bool is_north_pole;

    index_type m_well_node;

    index_type m_outer_nodes_offset;

private:
    elems_list m_elems;

public:
    OuterElems(
        edges_list const& edges,
        index_type to_sphere_offset,
        vector_of_indexes const& deleted_nodes,
        MeshProperties const& prop,
        index_type spherical_offset,
        bool north_pole
    )
    :
    m_edges(edges),
    m_to_sphere_offset(to_sphere_offset),
    m_deleted_nodes(deleted_nodes),
    m_prop(prop),
    m_spherical_offset(spherical_offset),
    is_north_pole(north_pole)
    {
        get_outer_elems();
    }

private:
    void get_outer_elems();

private:
    void get_north_outer_elems(size_t intersected_spheres);

private:
    void get_south_outer_elems(size_t intersected_spheres);

private:
    void get_property_fields();

public:
    elems_list get_elems() const
    {
        return m_elems;
    };
};

#include "SphericalElems.inl"