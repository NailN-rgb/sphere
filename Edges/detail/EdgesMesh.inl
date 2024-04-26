#pragma once

#include "../../DataTypes/Edge/Edge.h"

void EdgesMesh::calculate_edges_list()
{
    create_edges_at_well();

    connect_polar_nodes();

    connect_side_nodes();

    connect_edges_with_well_trajectory();

    connect_segments();
}


void EdgesMesh::write_edges_to_vtk()
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
    for(index_type i = 0; i < m_cylinder_count - 1; i++)
    {
        Edge edge;

        edge.set_first(i);
        edge.set_second(i + 1);

        edges.push_back(edge);
    }
}


/**
 * Connect polar points with well end's
*/
void EdgesMesh::connect_polar_nodes()
{
    Edge north_pole_edge;
    Edge south_pole_edge;

    north_pole_edge.set_first(m_cylinder_count);
    north_pole_edge.set_second(0);

    south_pole_edge.set_first(m_cylinder_count + 1);
    south_pole_edge.set_second(m_cylinder_count - 1);

    edges.push_back(north_pole_edge);
    edges.push_back(south_pole_edge);
}


/**
 * Connect points at side with each other
*/
void EdgesMesh::connect_side_nodes()
{
    for(index_type k = 0; k < m_segments_count; k++)
    {
        // upside semisphere
        Edge edge;

        edge.set_first(m_cylinder_count);
        edge.set_second(m_cylinder_count + 2 + m_side * k);

        edges.push_back(edge);

        // all of the points between polar points
        for(index_type j = 0; j < 2 * m_points_count + m_cylinder_count - 1; j++)
        {
            edge.set_first(m_cylinder_count + 2 + m_side * k + j);
            edge.set_second(m_cylinder_count + 2 + m_side * k + (j + 1));

            edges.push_back(edge);
        }

        // down side semisphere
        edge.set_first(2 * m_cylinder_count + 1 + 2 * m_points_count + m_side * k);
        edge.set_second(m_cylinder_count + 1);

        edges.push_back(edge);
    }
    
}


void EdgesMesh::connect_edges_with_well_trajectory()
{
    for(index_type j = 0; j < m_segments_count; j++)
    {   
        // connect with north pole
        for(index_type k = 0; k < m_points_count; k++)
        {
            Edge edge;

            edge.set_first(m_cylinder_count + 2 + k + m_side * j);
            edge.set_second(0);

            edges.push_back(edge);
        }

        // connect with cylindic part
        for(index_type k = 0; k < m_cylinder_count; k++)
        {
            Edge edge; 

            edge.set_first(m_cylinder_count + 2 + m_points_count + k + m_side * j);
            edge.set_second(k);

            edges.push_back(edge);
        }

        // connect south pole
        for(index_type k = 0; k < m_points_count; k++)
        {
            Edge edge;

            edge.set_first(m_cylinder_count + 2 + m_points_count + m_cylinder_count + k + m_side * j);
            edge.set_second(m_cylinder_count - 1);

            edges.push_back(edge);
        }
    }
}



void EdgesMesh::connect_segments()
{
    for(index_type k = 0; k < m_segments_count; k++)
    {
        if(k < m_segments_count - 1)
        {
            for(index_type j = 0; j < m_side; j++)
            {
                Edge edge;

                edge.set_first(m_cylinder_count + 2 + j + m_side * k);
                edge.set_second(m_cylinder_count + 2 + j + m_side * (k + 1));

                edges.push_back(edge);
            }
        }
        else
        {
            for(index_type j = 0; j < m_side; j++)
            {
                Edge edge;

                edge.set_first(m_cylinder_count + 2 + j + m_side * k);
                edge.set_second(m_cylinder_count + 2 + j);

                edges.push_back(edge);
            }
        }
    }
}