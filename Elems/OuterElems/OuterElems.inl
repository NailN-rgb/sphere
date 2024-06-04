#include "../OuterElems.h"


void OuterElems::get_property_fields()
{
    m_segments_count = m_prop.m_segments;
    m_mesh_count     = m_prop.m_mesh_count;
    m_well_offset    = m_prop.m_mesh_count;
    m_cylinder_offset = m_prop.m_cylinder_count;

    m_well_node = is_north_pole ? 0 : m_cylinder_offset - 1;

    m_outer_nodes_offset = m_well_offset + 2 * m_spherical_offset * m_mesh_count + m_cylinder_offset * m_segments_count - 
        std::accumulate(m_north_deleted.begin(), m_north_deleted.end(), 0) - std::accumulate(m_south_deleted.begin(), m_south_deleted.end(), 0); 
}


void get_outer_elems()
{
    size_t nonintersected_spheres = std::distance(
        m_deleted_nodes.begin(),
        std::find_if(
            m_deleted_nodes.begin(),
            m_deleted_nodes.end(),
            [](index_type i)
            {
                return i > 0;
            }
        )
    );

    get_north_outer_elems(nonintersected_spheres);

    get_south_outer_elems(nonintersected_spheres);
}


void get_north_outer_elems(size_t nonintersected_spheres)
{
    // get last nonintersected sphere part

    for(size_t i = 0; i < m_segments_count - 1; i++)
    {
        Element elem(4);

        elem.add_to_element(m_outer_nodes_offset);
        elem.add_to_element(m_outer_nodes_offset + 1 + i);
        elem.add_to_element(m_outer_nodes_offset + 2 + i);
        
        elem.add_to_element(m_well_node + (nonintersected_spheres - 1) * m_mesh_count);
        elem.add_to_element(m_well_node + (nonintersected_spheres - 1) * m_mesh_count + 1 + i);
        elem.add_to_element(m_well_node + (nonintersected_spheres - 1) * m_mesh_count + 2 + i);

        m_elems.push_pack(elem);
    }


    
}


void get_south_outer_elems(size_t intersected_spheres)
{

}
