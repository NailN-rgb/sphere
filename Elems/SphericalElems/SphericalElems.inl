#include "SphericalElems.h"


void SphericalElems::get_property_fields()
{
    m_segments_count = m_prop.m_segments;
    m_mesh_count     = m_prop.m_mesh_count;
    m_well_offset    = m_prop.m_mesh_count;
}

void SphericalElems::get_spheric_elems()
{
    // loop by mesh layers
    for(index_type i = 0; i < m_mesh_count; i++)
    {
        if(i == 0)
        {
            if(m_deleted_nodes[i] == 0)
            {
                auto north_from_well_edges_offset = m_mesh_count * m_spherical_offset 
                                                  - std::accumulate(
                                                        m_deleted_nodes.begin(),
                                                        m_deleted_nodes.end(),
                                                        0
                                                    );
                // polar part of mesh
                for(index_type j = 0; j < m_segments_count; j++)
                {
                    Element elem(6);

                    elem.add_to_element(m_well_offset - 1);
                    elem.add_to_element(m_well_offset + j);
                    elem.add_to_element(m_well_offset + j + 1);

                    elem.add_to_element(north_from_well_edges_offset + j);
                    elem.add_to_element(north_from_well_edges_offset + j + 1);
                    elem.add_to_element(north_from_well_edges_offset + m_segments_count + j);

                    m_elems.push_back(elem);
                }
            }
            else
            {

            }
        }
        else
        {

        }
    }
}
