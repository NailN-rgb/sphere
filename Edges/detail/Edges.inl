#pragma once

#include "../Edges.h"

void Edges::get_edges_list()
{
    create_edges_around_well();

    connect_polar_nodes();

    connect_side_nodes();

    connect_segments();
}


void Edges::write_edges_to_vtk()
{
    std::ofstream file;

    file.open("edges.vtk");

    if(file.is_open())
    {
        size_t points_count = m_entry_mesh.size();
        size_t elems_count  = edges.size();

        int vtk_mesh_type = 2;

        file << "# vtk DataFile Version 2.0" << std::endl;
        file << "Semi sphere mesh" << std::endl;
        file << "ASCII" << std::endl;
        file << "DATASET UNSTRUCTURED_GRID" << std::endl;
        file << "POINTS " << points_count << " float" << std::endl; 

        for (size_t i = 0; i < points_count; i++)
        {
            file << float(bg::get<0>(m_entry_mesh[i])) << "\t"
                 << float(bg::get<1>(m_entry_mesh[i])) << "\t"
                 << float(bg::get<2>(m_entry_mesh[i])) << std::endl;
        }

        file << "CELLS " << points_count + elems_count << "\t" << 2 * points_count + 3 * elems_count << std::endl;

        for(size_t i = 0; i < points_count; i++)
        {
            file << 1 << "\t" << i << std::endl;
        }

        for(size_t i = 0; i < elems_count; i++)
        {
            file << edges[i].size() << "\t";
            
            for(auto idx : edges[i])
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


void Edges::create_edges_around_well()
{
    for(index_type i = 0; i < m_cylinder_count - 1; i++)
    {
        vector_of_values edge;

        edge.push_back(i);
        edge.push_back(i + 1);

        edges.push_back(edge);
    }
}


void Edges::connect_polar_nodes()
{
    vector_of_values north_pole;
    vector_of_values south_pole;

    north_pole.push_back(m_cylinder_count);
    north_pole.push_back(0);

    south_pole.push_back(m_cylinder_count + 1);
    south_pole.push_back(m_cylinder_count - 1);

    edges.push_back(north_pole);
    edges.push_back(south_pole);
}


void Edges::connect_side_nodes()
{
    for(index_type k = 0; k < m_segments_count; k++)
    {
        // upside semisphere
        vector_of_values edge;

        edge.push_back(m_cylinder_count);
        edge.push_back(m_cylinder_count + 2 + m_side * k);

        edges.push_back(edge);

        edge.clear();

        for(index_type j = 0; j < 2 * m_points_count + m_cylinder_count - 1; j++)
        {
            edge.push_back(m_cylinder_count + 2 + m_side * k + j);
            edge.push_back(m_cylinder_count + 2 + m_side * k + (j + 1));

            edges.push_back(edge);
            edge.clear();
        }

        edge.push_back(2 * m_cylinder_count + 1 + 2 * m_points_count + m_side * k);
        edge.push_back(m_cylinder_count + 1);

        edges.push_back(edge);
    }
    
}


void Edges::connect_segments()
{
    for(index_type k = 0; k < m_segments_count; k++)
    {
        if(k < m_segments_count - 1)
        {
            for(index_type j = 0; j < m_side; j++)
            {
                vector_of_values edge;

                edge.push_back(m_cylinder_count + 2 + j + m_side * k);
                edge.push_back(m_cylinder_count + 2 + j + m_side * (k + 1));

                edges.push_back(edge);
            }
        }
        else
        {
            for(index_type j = 0; j < m_side; j++)
            {
                vector_of_values edge;

                edge.push_back(m_cylinder_count + 2 + j + m_side * k);
                edge.push_back(m_cylinder_count + 2 + j);

                edges.push_back(edge);
            }
        }
    }
}