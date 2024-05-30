#include "../Elems.h"

void Elems::set_mesh_properties()
{
    m_cylinder_count = m_prop.m_cylinder_count;
    m_segments_count = m_prop.m_segments;
    m_well_offset    = m_prop.m_cylinder_count;
    m_mesh_count     = m_prop.m_mesh_count;
}

void Elems::get_elems_list()
{
    get_elems_at_spherical_part();

    get_elems_at_cylinder_part();

    get_inner_elems();
}


void Elems::get_elems_at_spherical_part()
{
    SphericalElems se = SphericalElems(m_edges, 0, m_north_deleted, m_prop, m_spherical_offset);
    se.get_elems();
}

void Elems::get_elems_at_cylinder_part()
{
    // create cylinderElems class
}

void Elems::get_inner_elems()
{
    // create innerElems class
}