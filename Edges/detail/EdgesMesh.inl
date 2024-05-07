#pragma once

#include "../../DataTypes/Edge/Edge.h"
#include "../EdgesMesh.h"

void EdgesMesh::calculate_edges_list()
{
    // create_edges_at_well();
    // connect_spreical_points_with_well();
    // create_spherical_edges();
    // create_cylinder_sphere_connection(true);
    // create_edges_cylinder_by_equator_lines();
    // create_cylinder_longitude_edges();
    // create_cylidnder_to_mesh_edges();
    // create_cylinder_sphere_connection(false);
    write_edges_to_vtk();
}


void EdgesMesh::write_edges_to_vtk()
{
    std::ofstream file;

    file.open("edges.vtk");

    if(file.is_open())
    {
        size_t points_count = m_resulted_mesh.size();
        size_t elems_count  = edges.size();

        int vtk_mesh_type = 2;

        file << "# vtk DataFile Version 2.0" << std::endl;
        file << "Semi sphere mesh" << std::endl;
        file << "ASCII" << std::endl;
        file << "DATASET UNSTRUCTURED_GRID" << std::endl;
        file << "POINTS " << points_count << " float" << std::endl; 

        for (size_t i = 0; i < points_count; i++)
        {
            file << float(bg::get<0>(m_resulted_mesh[i])) << "\t"
                 << float(bg::get<1>(m_resulted_mesh[i])) << "\t"
                 << float(bg::get<2>(m_resulted_mesh[i])) << std::endl;
        }

        file << "CELLS " << points_count + elems_count << "\t" << 2 * points_count + 3 * elems_count << std::endl;

        for(size_t i = 0; i < points_count; i++)
        {
            file << 1 << "\t" << i << std::endl;
        }

        for(size_t i = 0; i < elems_count; i++)
        {
            file << edges[i].get_edge().size() << "\t";
            
            for(auto idx : edges[i].get_edge())
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
            file << 3 << std::endl;
        }

        file.close();
    }
    else
    {
        throw std::runtime_error("File can not to be opened");
    }
}

/**
 * Create m_cylinder_count - 1 edge at well trajectory
*/
void EdgesMesh::create_edges_at_well()
{
    edges.reserve(m_cylinder_count); // Preallocate memory to avoid reallocations

    for(index_type i = 0; i < m_cylinder_count; ++i)
    {
        edges.emplace_back(i, i+1);
    }
    
}


/**
 * Connect polar points with well end's
*/
void EdgesMesh::connect_spreical_points_with_well()
{
    edges.reserve(2 * m_entry_mesh.size());

    for(size_t i = m_well_offset, j = 0; j < m_entry_mesh.size(); ++i, ++j)
    {
        // north pole
        edges.emplace_back(0, i);
        // south pole
        edges.emplace_back(m_well_offset - 1, i + m_spherical_offset);
    }
}


void EdgesMesh::create_spherical_edges()
{
    auto points_at_segments = m_entry_mesh.size() - m_entry_mesh.size() % m_segments_count;
    auto points_layers = points_at_segments / m_segments_count;

    // north polar
    // connect with polar node
    for(index_type i = 0; i < m_segments_count; i++)
    {
        Edge edge;

        edge.set_first(m_well_offset);
        edge.set_second(m_well_offset + i);

        edges.push_back(edge);
    }

    // connect points from polar to cylinder part
    for(index_type i = 0; i < points_layers - 1; i++)
    {
        for(index_type j = 0; j < m_segments_count + 1; j++)
        {
            Edge edge;

            edge.set_first(m_well_offset + j + i * m_segments_count);
            edge.set_second(m_well_offset + j + (i + 1) * m_segments_count);
            
            edges.push_back(edge);
        }
    }

    // conect segments
    for(index_type i = 0; i < points_layers; i++)
    {
        for(index_type j = 0; j < m_segments_count - 1; j++)
        {
            Edge edge;

            edge.set_first(m_well_offset + (j + 1) + i * m_segments_count);
            edge.set_second(m_well_offset + (j + 2) + i * m_segments_count);
            
            edges.push_back(edge);
        }

        Edge edge;

        edge.set_first(m_well_offset + m_segments_count + i * m_segments_count);
        edge.set_second(m_well_offset + i * m_segments_count + 1);
        
        edges.push_back(edge);
    }

    /*
        South Pole
    */

    // connect with polar node
    for(index_type i = 0; i < m_segments_count; i++)
    {
        Edge edge;

        edge.set_first(m_well_offset + m_spherical_offset);
        edge.set_second(m_well_offset + i + m_spherical_offset);

        edges.push_back(edge);
    }

    // connect points from polar to cylinder part
    for(index_type i = 0; i < points_layers - 1; i++)
    {
        for(index_type j = 0; j < m_segments_count + 1; j++)
        {
            Edge edge;

            edge.set_first(m_well_offset + j + i * m_segments_count + m_spherical_offset);
            edge.set_second(m_well_offset + j + (i + 1) * m_segments_count + m_spherical_offset);
            
            edges.push_back(edge);
        }
    }

    // conect segments
    for(index_type i = 0; i < points_layers; i++)
    {
        for(index_type j = 0; j < m_segments_count - 1; j++)
        {
            Edge edge;

            edge.set_first(m_well_offset + (j + 1) + i * m_segments_count + m_spherical_offset);
            edge.set_second(m_well_offset + (j + 2) + i * m_segments_count + m_spherical_offset);
            
            edges.push_back(edge);
        }

        Edge edge;

        edge.set_first(m_well_offset + m_segments_count + i * m_segments_count + m_spherical_offset);
        edge.set_second(m_well_offset + i * m_segments_count + 1 + m_spherical_offset);
        
        edges.push_back(edge);
    }

}


void EdgesMesh::create_cylinder_sphere_connection(bool north_pole)
{
    auto cylinder_offset = m_well_offset + 2 * m_spherical_offset;

    if(north_pole)
    {
        // connect nortn sphere part with cylinder
        for(index_type i = 0; i < m_segments_count; i++)
        {
            Edge edge;

            edge.set_first(m_spherical_offset - m_segments_count + m_well_offset + i - 1);
            edge.set_second(cylinder_offset + i);
            
            edges.push_back(edge);
        }    
    }
    else
    {
        for(index_type i = 0; i < m_segments_count; i++)
        {
            Edge edge;

            edge.set_first(2 * m_spherical_offset - m_segments_count + m_well_offset + i - 1);
            edge.set_second(cylinder_offset + m_segments_count * (m_cylinder_count - 1) + i);
            
            edges.push_back(edge);
        } 
    }
    
}


void EdgesMesh::create_edges_cylinder_by_equator_lines()
{
    auto cylinder_offset = m_well_offset + 2 * m_spherical_offset; // why + 3 ??

    // connect by segments side
    for(index_type i = 0; i < m_segments_count; i++)
    {
        if(i < m_segments_count - 1)
        {
            for(index_type j = 0; j < m_cylinder_count; j++)
            {
                Edge edge;

                edge.set_first(cylinder_offset + j * m_segments_count + i);
                edge.set_second(cylinder_offset + j * m_segments_count + (i + 1));
                
                edges.push_back(edge);
            }

            Edge edge;

            edge.set_first(cylinder_offset + m_cylinder_count * m_segments_count + i);
            edge.set_second(cylinder_offset + m_cylinder_count * m_segments_count + i);
            
            edges.push_back(edge);
        }
        else
        {
            for(index_type j = 0; j < m_cylinder_count; j++)
            {
                Edge edge;

                edge.set_first(cylinder_offset + j * m_segments_count + i);
                edge.set_second(cylinder_offset + j * m_segments_count);
                
                edges.push_back(edge);
            }
        }
    }
}


void EdgesMesh::create_cylinder_longitude_edges()
{
    auto cylinder_offset = m_well_offset + 2 * m_spherical_offset; // why + 3 ??

    for(index_type i = 0; i < m_segments_count; i++)
    {
        for(index_type j = 0; j < m_cylinder_count - 1; j++)
        {
            Edge edge;

            edge.set_first(cylinder_offset + j * m_segments_count + i);
            edge.set_second(cylinder_offset + (j + 1) * m_segments_count + i);
            
            edges.push_back(edge);
        }
    }
}


void EdgesMesh::create_cylidnder_to_mesh_edges()
{
    auto cylinder_offset = m_well_offset + 2 * m_spherical_offset; // why + 3 ??

    for(index_type i = 0; i < m_segments_count; i++)
    {
        for(index_type j = 0; j < m_cylinder_count; j++)
        {
            Edge edge;

            edge.set_first(cylinder_offset + j * m_segments_count + i);
            edge.set_second(j);
            
            edges.push_back(edge);
        }
    }
}