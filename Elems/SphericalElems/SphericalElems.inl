#include "SphericalElems.h"


void SphericalElems::get_property_fields()
{
    m_segments_count = m_prop.m_segments;
    m_mesh_count     = m_prop.m_mesh_count;
    m_well_offset    = m_prop.m_mesh_count;
    m_cylinder_offset = m_prop.m_cylinder_count;

    m_well_node = is_north_pole ? 0 : m_cylinder_offset - 1;
}

void SphericalElems::get_spheric_elems()
{
    get_property_fields();

    // loop by mesh layers

    size_t nonintersected_spheres = std::find_if(
        m_deleted_nodes.begin(),
        m_deleted_nodes.end(),
        [](index_type i)
        {
            return i > 0;
        }) - m_deleted_nodes.begin();

    get_unintersected_sphere_elems(nonintersected_spheres);

    get_intersected_sphere_elems(nonintersected_spheres);
}


void SphericalElems::get_unintersected_sphere_elems(size_t nonintersected_spheres)
{
    for(index_type i = 0; i < nonintersected_spheres; i++)
    {
        if(i == 0) // special case for first layer
        {
            // connect polar point
            
            // rewrite past of code by this example
            std::vector<Element> new_elems(m_segments_count - 1);
            for(index_type j = 0; j < m_segments_count - 1; j++)
            {
                new_elems[j].add_to_element(m_well_node);
                new_elems[j].add_to_element(m_to_sphere_offset + m_cylinder_offset);
                new_elems[j].add_to_element(m_to_sphere_offset + m_cylinder_offset + j + 1);
                new_elems[j].add_to_element(m_to_sphere_offset + m_cylinder_offset + j + 2);
            }

            Element elem(4);
            elem.add_to_element(0);
            elem.add_to_element(m_to_sphere_offset + m_cylinder_offset);
            elem.add_to_element(m_to_sphere_offset + m_cylinder_offset + m_segments_count);
            elem.add_to_element(m_to_sphere_offset + m_cylinder_offset + 1);
            
            m_elems.insert(m_elems.end(), new_elems.begin(), new_elems.end());
            m_elems.push_back(elem);
            
            // connect well with 1 layer

            auto radial_layers = (m_spherical_offset - 1) / m_segments_count; 
            for(index_type k = 0; k < radial_layers - 1; k++)
            {
                for(index_type j = 0; j < m_segments_count - 1; j++)
                {
                    Element elem(5);

                    elem.add_to_element(m_to_sphere_offset + m_cylinder_offset + k * m_segments_count + j + 1);
                    elem.add_to_element(m_to_sphere_offset + m_cylinder_offset + k * m_segments_count + j + 2);
                    elem.add_to_element(m_to_sphere_offset + m_cylinder_offset + (k + 1) * m_segments_count + j + 2);
                    elem.add_to_element(m_to_sphere_offset + m_cylinder_offset + (k + 1) * m_segments_count + j + 1);

                    elem.add_to_element(0);


                    m_elems.push_back(elem);
                }
            }
        }
        else
        {
            for(index_type j = 0; j < m_segments_count - 1; j++)
            {
                Element elem(6);

                elem.add_to_element(m_to_sphere_offset + (i - 1) * m_spherical_offset + m_cylinder_offset);
                elem.add_to_element(m_to_sphere_offset + (i - 1) * m_spherical_offset + m_cylinder_offset + j + 1);
                elem.add_to_element(m_to_sphere_offset + (i - 1) * m_spherical_offset + m_cylinder_offset + j + 2);

                elem.add_to_element(m_to_sphere_offset + i * m_spherical_offset + m_cylinder_offset);
                elem.add_to_element(m_to_sphere_offset + i * m_spherical_offset + m_cylinder_offset + j + 1);
                elem.add_to_element(m_to_sphere_offset + i * m_spherical_offset + m_cylinder_offset + j + 2);

                m_elems.push_back(elem);
            }

            auto radial_layers = (m_spherical_offset - 1) / m_segments_count; 

            for(index_type k = 0; k < radial_layers - 1; k++)
            {
                for(index_type j = 0; j < m_segments_count - 1; j++)
                {
                    Element elem(8);

                    elem.add_to_element(m_to_sphere_offset + (i - 1) * m_spherical_offset + k * m_segments_count + m_cylinder_offset + j + 1);
                    elem.add_to_element(m_to_sphere_offset + (i - 1) * m_spherical_offset + k * m_segments_count + m_cylinder_offset + j + 2);
                    elem.add_to_element(m_to_sphere_offset + (i - 1) * m_spherical_offset + (k + 1) * m_segments_count + m_cylinder_offset + j + 2);
                    elem.add_to_element(m_to_sphere_offset + (i - 1) * m_spherical_offset + (k + 1) * m_segments_count + m_cylinder_offset + j + 1);


                    elem.add_to_element(m_to_sphere_offset + i * m_spherical_offset + k * m_segments_count + m_cylinder_offset + j + 1);
                    elem.add_to_element(m_to_sphere_offset + i * m_spherical_offset + k * m_segments_count + m_cylinder_offset + j + 2);
                    elem.add_to_element(m_to_sphere_offset + i * m_spherical_offset + (k + 1) * m_segments_count + m_cylinder_offset + j + 2);
                    elem.add_to_element(m_to_sphere_offset + i * m_spherical_offset + (k + 1) * m_segments_count + m_cylinder_offset + j + 1);


                    m_elems.push_back(elem);
                }
            }

        }
    }
}


void SphericalElems::get_intersected_sphere_elems(size_t nonintersected_spheres)
{
    for(size_t i = nonintersected_spheres; i < m_mesh_count; i++)
    {
        auto to_layer_offset = m_cylinder_offset + i * m_spherical_offset - std::accumulate(m_deleted_nodes.begin(), m_deleted_nodes.begin() + i, 0);

        auto radial_layers = (m_spherical_offset - m_deleted_nodes[i]) / m_segments_count;

        for(index_type j = 0; j < radial_layers - 1; j++)
        {
            for(index_type k = 0; k < m_segments_count - 1; k++)
            {
                Element elem(8);

                elem.add_to_element(m_to_sphere_offset + to_layer_offset + j * m_segments_count + k);
                elem.add_to_element(m_to_sphere_offset + to_layer_offset + j * m_segments_count + k + 1);
                elem.add_to_element(m_to_sphere_offset + to_layer_offset + (j + 1) * m_segments_count + k + 1);
                elem.add_to_element(m_to_sphere_offset + to_layer_offset + (j + 1) * m_segments_count + k);

                elem.add_to_element(m_to_sphere_offset + to_layer_offset - m_spherical_offset + m_deleted_nodes[i] + j * m_segments_count + k);
                elem.add_to_element(m_to_sphere_offset + to_layer_offset - m_spherical_offset + m_deleted_nodes[i] + j * m_segments_count + k + 1);
                elem.add_to_element(m_to_sphere_offset + to_layer_offset - m_spherical_offset + m_deleted_nodes[i] + (j + 1) * m_segments_count + k + 1);
                elem.add_to_element(m_to_sphere_offset + to_layer_offset - m_spherical_offset + m_deleted_nodes[i] + (j + 1) * m_segments_count + k);

                m_elems.push_back(elem);
            }
        }

    }
}
