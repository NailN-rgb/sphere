#include "../Elems.h"

void Elems::set_mesh_properties()
{
    m_cylinder_count = m_prop.m_cylinder_count;
    m_segments_count = m_prop.m_segments;
    m_well_offset    = m_prop.m_cylinder_count;
    m_mesh_count     = m_prop.m_mesh_count;
    m_south_sphere_offset = m_mesh_count * m_spherical_offset - std::accumulate(m_north_deleted.begin(), m_north_deleted.end(), 0);
}

void Elems::get_elems_list()
{
    get_elems_at_spherical_part();

    get_elems_at_cylinder_part();

    get_inner_elems();
}


void Elems::get_elems_at_spherical_part()
{
    auto sn = SphericalElems(m_edges, 0, m_north_deleted, m_prop, m_spherical_offset, true);
    sn.get_elems();

    auto sn_elems = sn.get_elems();
    m_elements.insert(m_elements.end(), sn_elems.begin(), sn_elems.end());


    auto ss = SphericalElems(m_edges, m_south_sphere_offset, m_south_deleted, m_prop, m_spherical_offset, true);
    ss.get_elems();

    auto ss_elems = ss.get_elems();
    m_elements.insert(m_elements.end(), ss_elems.begin(), ss_elems.end());
}

void Elems::get_elems_at_cylinder_part()
{
    // create cylinderElems class
}

void Elems::get_inner_elems()
{
    // create innerElems class
}


void Elems::form_vtk_file()
{
    std::ofstream file;

    file.open("elems.vtk");

    if(file.is_open())
    {
        size_t points_count = m_points.size();
        size_t elems_count  = m_elements.size();

        int vtk_mesh_type = 2;

        file << "# vtk DataFile Version 2.0" << std::endl;
        file << "Semi sphere mesh" << std::endl;
        file << "ASCII" << std::endl;
        file << "DATASET UNSTRUCTURED_GRID" << std::endl;
        file << "POINTS " << points_count << " float" << std::endl; 

        for (size_t i = 0; i < points_count; i++)
        {
            file << float(bg::get<0>(m_points[i])) << "\t"
                 << float(bg::get<1>(m_points[i])) << "\t"
                 << float(bg::get<2>(m_points[i])) << std::endl;
        }

        // get number of points at elems
        auto elems_number = 0;

        std::for_each(
            m_elements.begin(),
            m_elements.end(),
            [&elems_number](auto& elem)
            {
                elems_number += elem.get_element_size() + 1;
            }
        );

        file << "CELLS " << points_count + elems_count << "\t" << 2 * points_count + elems_number << std::endl;

        for(size_t i = 0; i < points_count; i++)
        {
            file << 1 << "\t" << i << std::endl;
        }

        for(size_t i = 0; i < elems_count; i++)
        {
            file << m_elements[i].get_element_size() << "\t";
            
            for(auto idx : m_elements[i].get_element())
            {
                file << idx << '\t'; 
            }

            file << std::endl;
        }


        file << "CELL_TYPES " << points_count + elems_count << std::endl;

        for(size_t i = 0; i < points_count; i++)
        {
            file << 2 << std::endl;
        }

        for(size_t i = 0; i < elems_count; i++)
        {
            if(m_elements[i].get_element_size() == 4)
            {
                file << 10 << std::endl;
            }

            if(m_elements[i].get_element_size() == 5)
            {
                file << 14 << std::endl;
            }

            if(m_elements[i].get_element_size() == 6)
            {
                file << 13 << std::endl;
            }

            if(m_elements[i].get_element_size() == 8)
            {
                file << 12 << std::endl;
            }
        }

        file.close();
    }
    else
    {
        throw std::runtime_error("File can not to be opened");
    }
}