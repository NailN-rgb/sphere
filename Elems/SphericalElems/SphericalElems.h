#pragma once

#include "../../precompile.h"

#include "../../DataTypes/Edge/Edge.h"
#include "../../DataTypes/Element/Element.h"
#include "../../MeshProperties/MeshProperties.h"

class SphericalElems
{
    using edges_list = std::vector<Edge>;
    using elems_list = std::vector<Element>;
private:
    edges_list m_edges;
    index_type m_to_sphere_offset;
    vector_of_indexes m_deleted_nodes;
    MeshProperties m_prop;

private:
    index_type m_segments_count;
    index_type m_mesh_count;
    index_type m_spherical_offset;
    index_type m_well_offset;
    index_type m_cylinder_offset;
    bool is_north_pole;

    index_type m_well_node;

private:
    elems_list m_elems;

public:
    SphericalElems(
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
        get_spheric_elems();
    }

private:
    void get_spheric_elems();

private:
    void get_unintersected_sphere_elems(size_t intersected_spheres);

private:
    void get_intersected_sphere_elems(size_t intersected_spheres);

private:
    void get_property_fields();

public:
    elems_list get_elems() const
    {
        return m_elems;
    };
};

#include "SphericalElems.inl"