#pragma once

#include "../../DataTypes/Edge/Edge.h"
#include "../EdgesMesh.h"
#include "EdgesMeshUtils.inl"

void EdgesMesh::calculate_edges_list()
{   
    // connect well nodes with each other
    create_edges_at_well();

    for(index_type i = 0; i < m_mesh_count; i++)
    {
        // connect well polar points with nodes at first mesh node
        //connect_spherical_points_with_well_at_north_pole(i);
        //connect_spreical_points_with_well_at_south_pole(i);

        // connect nodes at spheres
        create_spherical_edges_at_north_pole(i);
        create_spherical_edges_at_south_pole(i);

        //create_cylinder_edges(i);
    }

    // create_outer_mesh_edges_at_north();
    // create_outer_mesh_edges_at_south();

    write_edges_to_vtk();
}


void EdgesMesh::create_cylinder_edges(index_type mesh_index)
{
    create_cylinder_sphere_connection(true, mesh_index);
    
    // create horizontal directed edges at cylindric part
    create_edges_cylinder_by_equator_lines(mesh_index);

    // create edges between cylinder and well
    create_cylinder_longitude_edges(mesh_index);

    // create edges betweem well points and cylindric points
    create_cylidnder_to_well_edges(mesh_index);
    
    create_cylinder_sphere_connection(false, mesh_index);
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
void EdgesMesh::connect_spherical_points_with_well_at_north_pole(const index_type mesh_layer)
{
    if(mesh_layer == 0)
    {
        edges.reserve(2 * m_entry_mesh.size());
        // if there is a intersection at north side of mesh, we create edges from well_nodes_count indexes to end of sphirical mesh part
        for(size_t i = 0; i < m_spherical_offset - north_deleted_points[mesh_layer]; i++)
        {
            edges.emplace_back(0, m_well_offset + i);
        }
        
    }
    else
    {                                                            
        // first - find index at inner mesh layer
        // then find index at outer layer
        // connect by layers
        auto outer_index = m_well_offset + m_spherical_offset * mesh_layer - total_deleted_points_north(mesh_layer);
        auto inner_index = outer_index - m_spherical_offset + north_deleted_points[mesh_layer];

        for (size_t i = 0; i < m_spherical_offset - north_deleted_points[mesh_layer]; i++)
        {
            edges.emplace_back(inner_index + i, outer_index + i);
        }
    }
}


void EdgesMesh::connect_spreical_points_with_well_at_south_pole(const index_type mesh_layer)
{
    // count of files deleted at north pole intersection
    auto south_pole_offset    = m_well_offset + m_mesh_count * m_spherical_offset - total_deleted_points_north(m_mesh_count);
    auto points_at_south_pole = m_mesh_count * m_spherical_offset - total_deleted_points_south(m_mesh_count);

    if(mesh_layer == 0)
    {
        edges.reserve(points_at_south_pole);

        // if there is a intersection at north side of mesh, we create edges from well_nodes_count indexes to end of sphirical mesh part
        for(size_t i = 0; i < m_spherical_offset - south_deleted_points[mesh_layer]; i++)
        {
            edges.emplace_back(m_well_offset - 1, south_pole_offset + i);
        }
    }
    else
    {
        // first - find index at inner mesh layer
        // then find index at outer layer
        // connect by layers
        auto outer_index = south_pole_offset + m_spherical_offset * mesh_layer - total_deleted_points_south(mesh_layer);
        auto inner_index = outer_index - m_spherical_offset + south_deleted_points[mesh_layer];

        for (size_t i = 0; i < m_spherical_offset - south_deleted_points[mesh_layer]; i++)
        {
            edges.emplace_back(inner_index + i, outer_index + i);
        }
    }
}



void EdgesMesh::create_spherical_edges_at_north_pole(index_type mesh_index)
{
    auto points_at_segments = (m_spherical_offset - 1) / m_segments_count;

    #ifdef DEBUG
        std::cout << "points at segments: " << points_at_segments << std::endl;
    #endif

    // connect with polar node
    if(north_pole_intersected[mesh_index])
    {
        // TODO: code
    }
    else
    {
        for(index_type j = 0; j < m_segments_count; j++)
        {   
            //                  polar point       polar point + segment-mesh offset
            edges.emplace_back(
                m_well_offset + m_spherical_offset * mesh_index,
                m_well_offset + j + 1 + m_spherical_offset * mesh_index
            );
        }
        
    }

    // connect points from polar to cylinder part
    if (north_pole_intersected[mesh_index])
    {
        // TODO: without mesh layers
        auto radial_layers_count = (m_spherical_offset - north_deleted_points[mesh_index]) / m_segments_count;

        for(size_t i = 0; i < m_segments_count; i++)
        {
            for(size_t j = 0; j < radial_layers_count - 1; j++)
            {
                edges.emplace_back(
                    m_well_offset + j * m_segments_count + i + mesh_index * m_spherical_offset - total_deleted_points_north(mesh_index),
                    m_well_offset + (j + 1) * m_segments_count + i  + mesh_index * m_spherical_offset - total_deleted_points_north(mesh_index)
                );
            }
        }
    }
    else
    {
        auto radial_layers_count = (m_spherical_offset - 1) / m_segments_count;

        for(index_type i = 0; i < m_segments_count; i++)
        {
            for(index_type j = 0; j < points_at_segments - 1; j++)
            {
                // first point: well_offset + offset by mesh_layer + offset by segment + offset to neighboor element

                edges.emplace_back(
                    m_well_offset + j * m_segments_count + i + mesh_index * m_spherical_offset,
                    m_well_offset + (j + 1) * m_segments_count + i + mesh_index * m_spherical_offset
                );
            }
        }
    }
    
    auto radial_layers_count = (m_spherical_offset - north_deleted_points[mesh_index]) / m_segments_count;

    // conect segments
    if(north_pole_intersected[mesh_index])
    {
        for(index_type i = 0; i < m_segments_count - 1; i++)
        {
            for(index_type j = 0; j < radial_layers_count; j++)
            {
                // first point: well_offset + offset by mesh_layer + offset by vertical sphere direction + offset to neighboor element

                edges.emplace_back(
                    m_well_offset + j * m_segments_count + i + mesh_index * m_spherical_offset - total_deleted_points_north(mesh_index),
                    m_well_offset + j * m_segments_count + (i + 1) + mesh_index * m_spherical_offset - total_deleted_points_north(mesh_index)
                );
            }
        }

        for(index_type i = 0; i < radial_layers_count; i++)
        {
            // add last edge at horizontal layer
            edges.emplace_back(
                m_well_offset + i * (m_segments_count - 1) + mesh_index * m_spherical_offset - total_deleted_points_north(mesh_index) + i,
                m_well_offset + (i + 1) * (m_segments_count - 1) + mesh_index * m_spherical_offset - total_deleted_points_north(mesh_index) + i
            );
        }
    }
    else
    {
        for(index_type i = 0; i < m_segments_count - 1; i++)
        {
            for(index_type j = 0; j < radial_layers_count; j++)
            {
                // first point: well_offset + offset by mesh_layer + offset by vertical sphere direction + offset to neighboor element

                edges.emplace_back(
                    m_well_offset + j * m_segments_count + i + mesh_index * m_spherical_offset - total_deleted_points_north(mesh_index) + north_pole_intersected[mesh_index] + 1,
                    m_well_offset + j * m_segments_count + (i + 1) + mesh_index * m_spherical_offset - total_deleted_points_north(mesh_index) + north_pole_intersected[mesh_index] + 1
                );
            }

            // add last edge at horizontal layer
            edges.emplace_back(
                m_well_offset + i * (m_segments_count - 1) + mesh_index * m_spherical_offset + 1 + i,
                m_well_offset + (i + 1) * (m_segments_count - 1) + mesh_index * m_spherical_offset + 1 + i
            );
        }
    }
}


void EdgesMesh::create_spherical_edges_at_south_pole(index_type mesh_index)
{
    /*
        ---------------------------------------------------------------------------------------------------------------------------------
        -----------------------------------------SOUTH POLE------------------------------------------------------------------------------
        ---------------------------------------------------------------------------------------------------------------------------------
    */

    index_type deleted_segments_south = south_deleted_points[mesh_index] / m_segments_count + south_pole_intersected[mesh_index];

    index_type south_pole_offest = m_well_offset + m_mesh_count * m_spherical_offset - total_deleted_points_north(m_mesh_count);

    auto points_at_segments = (m_spherical_offset - 1) / m_segments_count;

    // connect with polar node
    if(south_pole_intersected[mesh_index])
    {
        // TODO: code
    }
    else
    {
        for(index_type j = 0; j < m_segments_count; j++)
        {   
            //                  polar point       polar point + segment-mesh offset
            edges.emplace_back(
                south_pole_offest + m_spherical_offset * mesh_index,
                south_pole_offest + j + 1 + m_spherical_offset * mesh_index
            );
        }
        
    }

    // connect points from polar to cylinder part
    if (south_pole_intersected[mesh_index])
    {
        // TODO: without mesh layers
        auto radial_layers_count = (m_spherical_offset - south_deleted_points[mesh_index]) / m_segments_count;

        for(size_t i = 0; i < m_segments_count; i++)
        {
            for(size_t j = 0; j < radial_layers_count - 1; j++)
            {
                edges.emplace_back(
                    south_pole_offest + j * m_segments_count + i + mesh_index * m_spherical_offset - total_deleted_points_south(mesh_index),
                    south_pole_offest + (j + 1) * m_segments_count + i  + mesh_index * m_spherical_offset - total_deleted_points_south(mesh_index)
                );
            }
        }
    }
    else
    {
        auto radial_layers_count = (m_spherical_offset - 1) / m_segments_count;

        for(index_type i = 0; i < m_segments_count; i++)
        {
            for(index_type j = 0; j < points_at_segments - 1; j++)
            {
                // first point: well_offset + offset by mesh_layer + offset by segment + offset to neighboor element

                edges.emplace_back(
                    south_pole_offest + j * m_segments_count + i + mesh_index * m_spherical_offset,
                    south_pole_offest + (j + 1) * m_segments_count + i + mesh_index * m_spherical_offset
                );
            }
        }
    }

    auto radial_layers_count = (m_spherical_offset - south_deleted_points[mesh_index]) / m_segments_count;

    // conect segments
    if(south_pole_intersected[mesh_index])
    {
        for(index_type i = 0; i < m_segments_count - 1; i++)
        {
            for(index_type j = 0; j < radial_layers_count; j++)
            {
                // first point: well_offset + offset by mesh_layer + offset by vertical sphere direction + offset to neighboor element

                edges.emplace_back(
                    south_pole_offest + j * m_segments_count + i + mesh_index * m_spherical_offset - total_deleted_points_south(mesh_index),
                    south_pole_offest + j * m_segments_count + (i + 1) + mesh_index * m_spherical_offset - total_deleted_points_south(mesh_index)
                );
            }
        }

        for(index_type i = 0; i < radial_layers_count; i++)
        {
            // add last edge at horizontal layer
            edges.emplace_back(
                south_pole_offest + i * (m_segments_count - 1) + mesh_index * m_spherical_offset - total_deleted_points_south(mesh_index) + i,
                south_pole_offest + (i + 1) * (m_segments_count - 1) + mesh_index * m_spherical_offset - total_deleted_points_south(mesh_index) + i
            );
        }
    }
    else
    {
        for(index_type i = 0; i < m_segments_count - 1; i++)
        {
            for(index_type j = 0; j < radial_layers_count; j++)
            {
                // first point: well_offset + offset by mesh_layer + offset by vertical sphere direction + offset to neighboor element

                edges.emplace_back(
                    south_pole_offest + j * m_segments_count + i + mesh_index * m_spherical_offset - total_deleted_points_south(mesh_index) + south_pole_intersected[mesh_index] + 1,
                    south_pole_offest + j * m_segments_count + (i + 1) + mesh_index * m_spherical_offset - total_deleted_points_south(mesh_index) + south_pole_intersected[mesh_index] + 1
                );
            }

            // add last edge at horizontal layer
            edges.emplace_back(
                south_pole_offest + i * (m_segments_count - 1) + mesh_index * m_spherical_offset + 1 + i,
                south_pole_offest + (i + 1) * (m_segments_count - 1) + mesh_index * m_spherical_offset + 1 + i
            );
        }
    }
}


void EdgesMesh::create_outer_mesh_edges_at_north()
{
    // we need to find index of outer mesh start point,
    // connect "outer" lgr layer with outer mesh
    // then connect outer mesh with each other

    // -to outer mesh nodes offset
    auto outer_north_offset = m_well_offset + 2 * m_mesh_count * m_spherical_offset + m_mesh_count * m_cylinder_offset  
                                                    - total_deleted_points_north(m_mesh_count) - total_deleted_points_south(m_mesh_count);

    // connect lgr nodes with outer mesh nodes.
    // get number of segment layers at last lgr mesh
    auto radial_layers_count = (m_spherical_offset - north_deleted_points[m_mesh_count - 1]) / m_segments_count;

    for(index_type i = 0; i < radial_layers_count; i++)
    {
        for(index_type j = 0; j < m_segments_count; j++)
        {
            edges.emplace_back(
                outer_north_offset + north_deleted_points[m_mesh_count - 1] + j + i * m_segments_count,
                m_well_offset + m_mesh_count * m_spherical_offset - total_deleted_points_north(m_mesh_count) - radial_layers_count * m_segments_count + j + i * m_segments_count
            );
        }
    }

    // outer mesh points connectioon

    // connect by polar 

    for(index_type i = 1; i < m_segments_count + 1; i++)
    {
        edges.emplace_back(
            outer_north_offset,
            outer_north_offset + i
        );
    }

    auto points_at_segments = (m_spherical_offset - 1) / m_segments_count;

    // connect onder radial parts of mesh
    for(index_type i = 1; i < m_segments_count + 1; i++)
    {
        for(index_type j = 0; j < points_at_segments - 1; j++)
        {
            edges.emplace_back(
                outer_north_offset + j * m_segments_count + i,
                outer_north_offset + (j + 1) * m_segments_count + i
            );
        }
    }


    // Third step: connect outer mesh points by circles

    for(index_type j = 0; j < points_at_segments; j++)
    {
        for(index_type i = 0; i < m_segments_count - 1; i++)
        {
            edges.emplace_back(
                outer_north_offset + j  + i * m_segments_count + 1,
                outer_north_offset + (j + 1) + i * m_segments_count + 1
            );
        }

        edges.emplace_back(
            outer_north_offset + j * m_segments_count + 1,
            outer_north_offset + m_segments_count - 1 + 1 + j * m_segments_count
        );
    }
}


void EdgesMesh::create_outer_mesh_edges_at_south()
{
    // we need to find index of outer mesh start point,
    // connect "outer" lgr layer with outer mesh
    // then connect outer mesh with each other

    // -to outer mesh nodes offset
    auto outer_south_offset = m_well_offset + 2 * m_mesh_count * m_spherical_offset + m_mesh_count * m_cylinder_offset  
                                                    - total_deleted_points_north(m_mesh_count) - total_deleted_points_south(m_mesh_count) + m_spherical_offset;

    // connect lgr nodes with outer mesh nodes.
    // get number of segment layers at last lgr mesh
    auto radial_layers_count = (m_spherical_offset - south_deleted_points[m_mesh_count - 1]) / m_segments_count;

    for(index_type i = 0; i < radial_layers_count; i++)
    {
        for(index_type j = 0; j < m_segments_count; j++)
        {
            edges.emplace_back(
                outer_south_offset + south_deleted_points[m_mesh_count - 1] + j + i * m_segments_count,
                m_well_offset + 2 * m_mesh_count * m_spherical_offset - total_deleted_points_north(m_mesh_count) - total_deleted_points_south(m_mesh_count)
                                                                                             - radial_layers_count * m_segments_count + j + i * m_segments_count
            );
        }
    }

    // outer mesh points connectioon

    // connect by polar 

    for(index_type i = 1; i < m_segments_count + 1; i++)
    {
        edges.emplace_back(
            outer_south_offset,
            outer_south_offset + i
        );
    }

    auto points_at_segments = (m_spherical_offset - 1) / m_segments_count;

    // connect onder radial parts of mesh
    for(index_type i = 1; i < m_segments_count + 1; i++)
    {
        for(index_type j = 0; j < points_at_segments - 1; j++)
        {
            edges.emplace_back(
                outer_south_offset + j * m_segments_count + i,
                outer_south_offset + (j + 1) * m_segments_count + i
            );
        }
    }


    // Third step: connect outer mesh points by circles

    for(index_type j = 0; j < points_at_segments; j++)
    {
        for(index_type i = 0; i < m_segments_count - 1; i++)
        {
            edges.emplace_back(
                outer_south_offset + j  + i * m_segments_count + 1,
                outer_south_offset + (j + 1) + i * m_segments_count + 1
            );
        }

        edges.emplace_back(
            outer_south_offset + j * m_segments_count + 1,
            outer_south_offset + m_segments_count - 1 + 1 + j * m_segments_count
        );
    }
}


void EdgesMesh::create_cylinder_sphere_connection(bool north_pole, index_type mesh_index)
{
    if(north_pole)
    {
        auto sphere_node_offset = m_well_offset + (mesh_index + 1) * m_spherical_offset - total_deleted_points_north(mesh_index + 1) - m_segments_count;

        auto cylinder_offset = m_well_offset + 2 * m_mesh_count * m_spherical_offset + mesh_index * m_cylinder_offset - total_deleted_points_north(m_mesh_count) -   
                                                                                            total_deleted_points_south(m_mesh_count);


        for(index_type i = 0; i < m_segments_count; i++)
        {
            edges.emplace_back(sphere_node_offset + i, cylinder_offset + i);
        }
    }
    else
    {
        auto sphere_node_offset = m_well_offset + m_mesh_count * m_spherical_offset - total_deleted_points_north(m_mesh_count) + 
                                                (mesh_index + 1) * m_spherical_offset - total_deleted_points_south(mesh_index + 1) - m_segments_count;

        auto cylinder_offset = m_well_offset + 2 * m_mesh_count * m_spherical_offset + (mesh_index + 1) * m_cylinder_offset - total_deleted_points_north(m_mesh_count) -   
                                                                                            total_deleted_points_south(m_mesh_count) - m_segments_count;


        for(index_type i = 0; i < m_segments_count; i++)
        {
            edges.emplace_back(sphere_node_offset + i, cylinder_offset + i);
        }
    }
}


void EdgesMesh::create_edges_cylinder_by_equator_lines(index_type mesh_index)
{
    // connect by segments side
    auto deleted_points_offset = m_well_offset + 2 * m_mesh_count * m_spherical_offset - total_deleted_points_south(m_mesh_count)
                                                                                           - total_deleted_points_north(m_mesh_count)
                                                                                           + mesh_index * m_segments_count * m_cylinder_count;

    for(index_type i = 1; i < m_cylinder_count - 1; i++)
    {
        for(index_type j = 0; j < m_segments_count - 1; j++)
        {
            edges.emplace_back(
                deleted_points_offset + j + i * m_segments_count,
                deleted_points_offset + (j + 1) + i * m_segments_count
            );
        }

        edges.emplace_back(
            deleted_points_offset + i * m_segments_count,
            deleted_points_offset + m_segments_count - 1 + i * m_segments_count
        );
    }
    
}


void EdgesMesh::create_cylinder_longitude_edges(index_type mesh_index)
{
    auto deleted_points_offset = m_well_offset + 2 * m_mesh_count * m_spherical_offset - total_deleted_points_south(m_mesh_count)
                                                                                           - total_deleted_points_north(m_mesh_count)
                                                                                           + mesh_index * m_segments_count * m_cylinder_count;

    for (index_type i = 0; i < m_segments_count; i++)
    {
        for(index_type j = 0; j < m_cylinder_count - 1; j++)
        {
            edges.emplace_back(
                deleted_points_offset + j * m_segments_count + i,
                deleted_points_offset + (j + 1) * m_segments_count + i 
            );
        }
    }
}


void EdgesMesh::create_cylidnder_to_well_edges(index_type mesh_index)
{
    auto deleted_points_offset = m_well_offset + 2 * m_mesh_count * m_spherical_offset - total_deleted_points_south(m_mesh_count)
                                                                                        - total_deleted_points_north(m_mesh_count)
                                                                                        + mesh_index * m_segments_count * m_cylinder_count;
    if(mesh_index == 0)
    {
        for(index_type i = 0; i < m_segments_count; i++)
        {
            for(index_type j = 0; j < m_cylinder_count; j++)
            {
                edges.emplace_back(
                    j,
                    deleted_points_offset + j * m_segments_count + i
                );
            }
        }
    }
    else
    {
        for(index_type i = 0; i < m_segments_count; i++)
        {
            for(index_type j = 0; j < m_cylinder_count; j++)
            {
                edges.emplace_back(
                    deleted_points_offset + j * m_segments_count + i - m_cylinder_offset,
                    deleted_points_offset + j * m_segments_count + i
                );
            }
        }
    }
    

}