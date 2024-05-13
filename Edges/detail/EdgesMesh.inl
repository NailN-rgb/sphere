#pragma once

#include "../../DataTypes/Edge/Edge.h"
#include "../EdgesMesh.h"

void EdgesMesh::calculate_edges_list()
{   
    // connect well nodes with each other
    create_edges_at_well();

    for(index_type i = 0; i < m_mesh_count; i++)
    {
        // connect well polar points with nodes at first mesh node
        connect_spreical_points_with_well_at_north_pole(i);
        connect_spreical_points_with_well_at_south_pole(i);
        // connect spherical layers with each other
        // create_sphere_mesh_connection(i);

        // // connect nodes at spheres
        // create_spherical_edges(i);

        // //create_cylinder_sphere_connection(true);
        
        // // create horizontal directed edges at cylindric part
        // create_edges_cylinder_by_equator_lines(i);

        // // create edges between cylinder and well
        // create_cylinder_longitude_edges(i);

        // // create edges betweem well points and cylindric points
        // create_cylidnder_to_well_edges(i);

        
        // create_cylinder_sphere_connection(false);
    }


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

    for(index_type i = 0; i < m_cylinder_count - 1; ++i)
    {
        edges.emplace_back(i, i+1);
    }
    
}


/**
 * Connect polar points with well end's
*/
void EdgesMesh::connect_spreical_points_with_well_at_north_pole(const index_type mesh_layer)
{
    if(mesh_layer == 0)
    {
        edges.reserve(2 * m_entry_mesh.size());
        // if there is a intersection at north side of mesh, we create edges from well_nodes_count indexes to end of sphirical mesh part
        if(north_pole_intersected[mesh_layer])
        {
            for(size_t i = 0; i < m_entry_mesh.size() - north_deleted_points[mesh_layer]; ++i)
            {
                edges.emplace_back(0, m_well_offset + i);
            }
        }
        else
        {
            for(size_t i = m_well_offset; i < m_mesh_count * m_entry_mesh.size(); i += m_mesh_count)
            {
                edges.emplace_back(0, i);
            }
        }
    }
    else
    {                                                            
        if(north_pole_intersected[mesh_layer])
        {
            for(size_t i = north_deleted_points[mesh_layer]; i < (m_mesh_count - 1) * m_spherical_offset; i += m_mesh_count)
            {
                edges.emplace_back(m_well_offset + i, m_well_offset + i + 1);
            }
        }
        else
        { 
            // if we dont have intersection at current layer, we dont have it at all of layers before
            for(size_t i = 0; i < m_mesh_count * m_entry_mesh.size(); i += m_mesh_count)
            {
                edges.emplace_back(m_well_offset + i, m_well_offset + i + 1);
            }
        }
    }
}


void EdgesMesh::connect_spreical_points_with_well_at_south_pole(const index_type mesh_layer)
{
    auto south_polar_offset = [this]() // get number of points at north spherical part after intersection
    {
        auto north_polar_points = 0;
        std::for_each(
            north_deleted_points.begin(),
            north_deleted_points.end(),
            [&north_polar_points, this](auto& x) 
            { 
                north_polar_points += m_spherical_offset - x;
            }
        );

        return north_polar_points - 1;
    };

    auto deleted_points_at_south_sphere = std::accumulate(south_deleted_points.begin(),
                                                        south_deleted_points.end(), 0);

    if(mesh_layer == 0)
    {
        edges.reserve(2 * m_entry_mesh.size());
        // if there is a intersection at north side of mesh, we create edges from well_nodes_count indexes to end of sphirical mesh part
        if(south_pole_intersected[mesh_layer])
        {
            for(size_t i = 1; i < m_mesh_count * m_spherical_offset - deleted_points_at_south_sphere; i+=m_mesh_count)
            {
                edges.emplace_back(m_well_offset - 1, m_well_offset + south_polar_offset() + i);
            }
        }
        else
        {
            for(size_t i = 0; i < m_mesh_count * m_spherical_offset; i += m_mesh_count)
            {
                edges.emplace_back(m_well_offset - 1, m_well_offset + south_polar_offset() + i + !north_pole_intersected[mesh_layer]);
            }
        }
    }
    else
    {
        if(south_pole_intersected[mesh_layer])
        {
            for(size_t i = south_deleted_points[mesh_layer];
                i < m_mesh_count * m_spherical_offset - deleted_points_at_south_sphere;
                i+=m_mesh_count)
            {
                edges.emplace_back(m_well_offset + south_polar_offset() + i - 1, 
                                   m_well_offset + south_polar_offset() + i);
            }
        }
        else
        { 
            // if we dont have intersection at current layer, we dont have it at all of layers before
            for(size_t i = south_deleted_points[mesh_layer]; i < m_mesh_count * m_spherical_offset; i += m_mesh_count)
            {
                edges.emplace_back(m_well_offset + i + south_polar_offset()     - !north_pole_intersected[mesh_layer],
                                   m_well_offset + i + south_polar_offset() + 1 - !north_pole_intersected[mesh_layer]);
            }
        }
    }
}



void EdgesMesh::create_spherical_edges(index_type mesh_index)
{
    auto points_at_segments = m_entry_mesh.size() - m_entry_mesh.size() % m_segments_count;
    auto points_layers = points_at_segments / m_segments_count; // how much segments

    int deleted_segments_north = north_deleted_points[mesh_index] / m_segments_count + north_pole_intersected[mesh_index]; 

    if(points_layers == deleted_segments_north)
    {
        throw std::runtime_error("There no points at polar part");
    }

    // north polar
    // connect with polar node
    if(north_pole_intersected[mesh_index])
    {
        if(north_deleted_points[mesh_index] - m_entry_mesh.size() != 0)
        {
            // if there is a intersection we add new polar point instead a north pole 
            for(index_type i = 0; i < m_segments_count; i++)
            {
                auto deleted_mesh_offset = 2 * m_entry_mesh.size();// - north_deleted_points - south_deleted_points;

                Edge edge;
                
                edge.set_first(m_well_offset + deleted_mesh_offset + m_cylinder_offset + i - north_deleted_points[mesh_index] + 1);

                edge.set_second(m_well_offset + i);

                edges.push_back(edge);
            }
        }
    }
    else
    {
        for(index_type i = 0; i < m_segments_count; i++)
        {
            Edge edge;

            edge.set_first(m_well_offset);

            edge.set_second(m_well_offset + i);

            edges.push_back(edge);
        }
    }

    // connect points from polar to cylinder part
    for(index_type i = 0; i < points_layers - deleted_segments_north - 1; i++)
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
    for(index_type i = 0; i < points_layers - deleted_segments_north; i++)
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
        ---------------------------------------------------------------------------------------------------------------------------------
        -----------------------------------------SOUTH POLE------------------------------------------------------------------------------
        ---------------------------------------------------------------------------------------------------------------------------------
    */

    index_type deleted_segments_south = south_deleted_points[mesh_index] / m_segments_count + south_pole_intersected[mesh_index]; 


    // connect with polar node

    if(south_pole_intersected[mesh_index])
    {
        if(south_deleted_points[mesh_index] - m_entry_mesh.size() != 0)
        {
            // if there is a intersection we add new polar point instead a north pole 
            for(index_type i = 0; i < m_segments_count; i++)
            {
                auto deleted_mesh_offset = 2 * m_entry_mesh.size() - north_deleted_points[mesh_index] - south_deleted_points[mesh_index];
                auto north_deleted_offset = m_entry_mesh.size() - north_deleted_points[mesh_index]; // how much points at north pole

                Edge edge;

                edge.set_first(m_well_offset + deleted_mesh_offset + m_cylinder_offset + m_entry_mesh.size() + (i + 1));

                edge.set_second(m_well_offset + north_deleted_offset + i);

                edges.push_back(edge);
            }
        }
    }
    else
    {
        for(index_type i = 0; i < m_segments_count; i++)
        {
            Edge edge;

            edge.set_first(m_well_offset + m_spherical_offset - north_deleted_points[mesh_index] - south_deleted_points[mesh_index]);

            edge.set_second(m_well_offset + i + m_spherical_offset - north_deleted_points[mesh_index] - south_deleted_points[mesh_index]);

            edges.push_back(edge);
        }  
        
    }

    // connect points from polar to cylinder part
    for(index_type i = 0; i < points_layers - deleted_segments_south - 1; i++)
    {
        for(index_type j = 0; j < m_segments_count; j++)
        {
            Edge edge;

            edge.set_first(m_well_offset + j + i * m_segments_count + m_spherical_offset - north_deleted_points[mesh_index] - south_deleted_points[mesh_index]);
            edge.set_second(m_well_offset + j + (i + 1) * m_segments_count + m_spherical_offset - north_deleted_points[mesh_index] - south_deleted_points[mesh_index]);
            
            edges.push_back(edge);
        }
    }

    // conect segments
    for(index_type i = 0; i < points_layers - deleted_segments_south; i++)
    {
        for(index_type j = 0; j < m_segments_count - 1; j++)
        {
            Edge edge;

            edge.set_first(m_well_offset + (j + 1) + i * m_segments_count + m_spherical_offset - north_deleted_points[mesh_index] - south_deleted_points[mesh_index]);
            edge.set_second(m_well_offset + (j + 2) + i * m_segments_count + m_spherical_offset - north_deleted_points[mesh_index] - south_deleted_points[mesh_index]);
            
            edges.push_back(edge);
        }

        Edge edge;

        edge.set_first(m_well_offset + m_segments_count + i * m_segments_count + m_spherical_offset - north_deleted_points[mesh_index] - south_deleted_points[mesh_index]);
        edge.set_second(m_well_offset + i * m_segments_count + 1 + m_spherical_offset - north_deleted_points[mesh_index] - south_deleted_points[mesh_index]);
        
        edges.push_back(edge);
    }

}


void EdgesMesh::create_cylinder_sphere_connection(bool north_pole, index_type mesh_index)
{
    auto cylinder_offset = m_well_offset + 2 * m_spherical_offset - north_deleted_points[mesh_index] - south_deleted_points[mesh_index];

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


void EdgesMesh::create_edges_cylinder_by_equator_lines(index_type mesh_index)
{
    auto cylinder_offset = m_well_offset + 2 * m_spherical_offset - north_deleted_points[mesh_index] - south_deleted_points[mesh_index]; // why + 3 ??

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


void EdgesMesh::create_cylinder_longitude_edges(index_type mesh_index)
{
    auto cylinder_offset = m_well_offset + 2 * m_spherical_offset - north_deleted_points[mesh_index] - south_deleted_points[mesh_index]; // why + 3 ??

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


void EdgesMesh::create_cylidnder_to_well_edges(index_type mesh_index)
{
    auto cylinder_offset = m_well_offset + 2 * m_spherical_offset - north_deleted_points[mesh_index] - south_deleted_points[mesh_index]; // why + 3 ??

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