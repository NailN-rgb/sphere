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
        connect_spherical_points_with_well_at_north_pole(i);
        connect_spreical_points_with_well_at_south_pole(i);

        // connect nodes at spheres
        create_spherical_edges_at_north_pole(i);
        create_spherical_edges_at_south_pole(i);

        create_cylinder_nodes(i);
    }

    create_outer_mesh_edges_at_north();
    create_outer_mesh_edges_at_south();

    write_edges_to_vtk();
}


void EdgesMesh::create_cylinder_nodes(index_type mesh_index)
{
    create_cylinder_sphere_connection(true, mesh_index);
    
    // create horizontal directed edges at cylindric part
    create_edges_cylinder_by_equator_lines(mesh_index);

    // // create edges between cylinder and well
    create_cylinder_longitude_edges(mesh_index);

    // // create edges betweem well points and cylindric points
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
                for(size_t j = 0; j < m_mesh_count - 1; j++)
                {
                    edges.emplace_back(m_well_offset + j + i, m_well_offset + i + j + 1);
                }
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
                edges.emplace_back(m_well_offset - 1, m_well_offset + south_polar_offset() + i + 1);
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
            for(size_t i = 0; i < m_mesh_count * m_spherical_offset; i += m_mesh_count)
            {
                // edges.emplace_back(m_well_offset + i + south_polar_offset()     - !north_pole_intersected[mesh_layer],
                //                    m_well_offset + i + south_polar_offset() + 1 - !north_pole_intersected[mesh_layer]);

                for(size_t j = 0; j < m_mesh_count - 1; j++)
                {
                    edges.emplace_back(m_well_offset + south_polar_offset() + i + j + 1,
                                       m_well_offset + south_polar_offset() + i + j + 2);
                }
            }
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
        for(index_type i = 0; i < m_mesh_count; i++)
        {
            for(index_type j = 0; j < m_segments_count; j++)
            {   
                //                  polar point       polar point + segment-mesh offset
                edges.emplace_back(m_well_offset + i, m_well_offset + (j + 1) * m_mesh_count + i);
            }
        }
    }

    // connect points from polar to cylinder part
    if (north_pole_intersected[mesh_index])
    {
        // TODO: without mesh layers
        auto radial_layers_count = (m_spherical_offset - north_deleted_points[mesh_index]) / m_segments_count;

        for(index_type i = 0; i < m_segments_count; i++)
        {
            for(index_type j = 0; j < radial_layers_count - 1; j++)
            {
                edges.emplace_back(
                    m_well_offset + j * m_segments_count + i,
                    m_well_offset + (j + 1) * m_segments_count + i
                );
            }
        }
    }
    else
    {
        for(index_type i = 0; i < m_mesh_count; i++)
        {
            for(index_type j = 0; j < m_segments_count; j++)
            {
                for(index_type k = 0; k < points_at_segments - 1; k++)
                {
                    // first point: well_offset + offset by mesh_layer + offset by segment + offset to neighboor element

                    edges.emplace_back(
                        m_well_offset + i + (j + 1) * m_mesh_count + m_mesh_count * m_segments_count * k,      
                        m_well_offset +  i +(j + 1) * m_mesh_count + m_mesh_count * m_segments_count * (k + 1)
                    );
                }
            }
        }
    }
    
    
    // conect segments
    if(north_pole_intersected[mesh_index])
    {
        auto radial_layers_count = (m_spherical_offset - north_deleted_points[mesh_index]) / m_segments_count;

        for(index_type i = 0; i < radial_layers_count - 1; i++)
        {
            for(index_type j = 1; j < m_segments_count; j++)
            {
                // first point: well_offset + offset by mesh_layer + offset by vertical sphere direction + offset to neighboor element

                edges.emplace_back(
                    m_well_offset + j + i * m_segments_count - 1,
                    m_well_offset + (j + 1) + i * m_segments_count - 1
                );
            }

            // add last edge at horizontal layer
            edges.emplace_back(
                m_well_offset + i * m_segments_count,
                m_well_offset + (i + 1) * m_segments_count - 1
            );
        }
    }
    else
    {
        for(index_type i = 0; i < m_mesh_count; i++)
        {
            for(index_type k = 0; k < points_at_segments - 1; k++)
            {
                for(index_type j = 1; j < m_segments_count; j++)
                {
                    // first point: well_offset + offset by mesh_layer + offset by vertical sphere direction + offset to neighboor element

                    edges.emplace_back(
                        m_well_offset + i + k * m_segments_count * m_mesh_count + j * m_mesh_count,      
                        m_well_offset + i + k * m_segments_count * m_mesh_count + (j + 1) * m_mesh_count
                    );
                }

                // add last edge at horizontal layer
                edges.emplace_back(
                        m_well_offset + k * m_segments_count * m_mesh_count + m_mesh_count + i,      
                        m_well_offset + k * m_segments_count * m_mesh_count + m_mesh_count * m_segments_count + i
                    );
            }
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

    index_type total_north_sphere_offset = m_well_offset + m_mesh_count * m_spherical_offset - total_deleted_points_north();

    auto points_at_segments = (m_spherical_offset - 1) / m_segments_count;

    // connect with polar node
    if(south_pole_intersected[mesh_index])
    {
        // TODO: code here
    }
    else
    {
        for(index_type i = 0; i < m_mesh_count; i++)
        {
            for(index_type j = 0; j < m_segments_count; j++)
            {   
                //                  polar point       polar point + segment-mesh offset
                edges.emplace_back(total_north_sphere_offset + i, total_north_sphere_offset +  (j + 1) * m_mesh_count + i);
            }
        }
    }

    // connect points from polar to cylinder part
    if(south_pole_intersected[mesh_index])
    {
        auto south_sphere_offset = m_well_offset + m_spherical_offset - north_deleted_points[mesh_index];

        // TODO: without mesh layers
        auto radial_layers_count = (m_spherical_offset - south_deleted_points[mesh_index]) / m_segments_count;

        for(index_type i = 0; i < m_segments_count; i++)
        {
            for(index_type j = 0; j < radial_layers_count - 1; j++)
            {
                edges.emplace_back(
                    south_sphere_offset + j * m_segments_count + i,
                    south_sphere_offset + (j + 1) * m_segments_count + i
                );
            }
        }
        
    }
    else
    {
        for(index_type i = 0; i < m_mesh_count; i++)
        {
            for(index_type j = 0; j < m_segments_count; j++)
            {
                for(index_type k = 0; k < points_at_segments - 1; k++)
                {
                    // first point: well_offset + offset by mesh_layer + offset by segment + offset to neighboor element

                    edges.emplace_back(
                        total_north_sphere_offset + i + (j + 1) * m_mesh_count + m_mesh_count * m_segments_count * k,      
                        total_north_sphere_offset +  i +(j + 1) * m_mesh_count + m_mesh_count * m_segments_count * (k + 1)
                    );
                }
            }
        }
    }

    // conect segments
    if(south_pole_intersected[mesh_index])
    {
        auto radial_layers_count = (m_spherical_offset - south_deleted_points[mesh_index]) / m_segments_count;

        auto south_sphere_offset = m_well_offset + m_spherical_offset - north_deleted_points[mesh_index];

        for(index_type i = 0; i < radial_layers_count - 1; i++)
        {
            for(index_type j = 1; j < m_segments_count; j++)
            {
                // first point: well_offset + offset by mesh_layer + offset by vertical sphere direction + offset to neighboor element

                edges.emplace_back(
                    south_sphere_offset + j + i * m_segments_count - 1,
                    south_sphere_offset + (j + 1) + i * m_segments_count - 1
                );
            }

            // add last edge at horizontal layer
            edges.emplace_back(
                south_sphere_offset + i * m_segments_count,
                south_sphere_offset + (i + 1) * m_segments_count - 1
            );
        }
    }
    else
    {
        for(index_type i = 0; i < m_mesh_count; i++)
        {
            for(index_type k = 0; k < points_at_segments - 1; k++)
            {
                for(index_type j = 1; j < m_segments_count; j++)
                {
                    // first point: well_offset + offset by mesh_layer + offset by vertical sphere direction + offset to neighboor element

                    edges.emplace_back(
                        total_north_sphere_offset + i + k * m_segments_count * m_mesh_count + j * m_mesh_count,      
                        total_north_sphere_offset + i + k * m_segments_count * m_mesh_count + (j + 1) * m_mesh_count
                    );
                }

                // add last edge at horizontal layer
                edges.emplace_back(
                        total_north_sphere_offset + k * m_segments_count * m_mesh_count + m_mesh_count + i,      
                        total_north_sphere_offset + k * m_segments_count * m_mesh_count + m_mesh_count * m_segments_count + i
                    );
            }
        }
    }
}


void EdgesMesh::create_outer_mesh_edges_at_north()
{
    if(total_deleted_points_to_layer(m_mesh_count - 1) != 0)
    {
        auto outer_mesh_offset = m_well_offset + m_mesh_count * (2 * m_spherical_offset + m_cylinder_count * m_segments_count) - total_deleted_points_to_layer(m_mesh_count - 1);    
        auto points_at_segments = (m_spherical_offset - 1) / m_segments_count;

        // first step: connect lgr with outer nodes
        for(index_type i = 0; i < m_spherical_offset - total_deleted_points_to_layer(m_mesh_count - 1); i++)
        {
            edges.emplace_back(
                outer_mesh_offset + i,
                m_well_offset + (i + 1)* m_mesh_count - 1 - north_pole_intersected[m_mesh_count - 1]
            );
        }

        // Second step: connect outer mesh nodes with edges from polar point(0,0,0) to cylinder part of mesh

        // connect polar part of mesh
        for(index_type i = 1; i < m_segments_count + 1; i++)
        {
            edges.emplace_back(
                outer_mesh_offset,
                outer_mesh_offset + i
            );
        }

        // connect onder radial parts of mesh
        for(index_type i = 1; i < m_segments_count + 1; i++)
        {
            for(index_type j = 0; j < points_at_segments - 1; j++)
            {
                edges.emplace_back(
                    outer_mesh_offset + j * m_segments_count + i,
                    outer_mesh_offset + (j + 1) * m_segments_count + i
                );
            }
        }

        // Third step: connect outer mesh points by circles

        for(index_type j = 0; j < points_at_segments; j++)
        {
            for(index_type i = 0; i < m_segments_count - 1; i++)
            {
                edges.emplace_back(
                    outer_mesh_offset + j  + i * m_segments_count + 1,
                    outer_mesh_offset + (j + 1) + i * m_segments_count + 1
                );
            }

            edges.emplace_back(
                outer_mesh_offset + j * m_segments_count + 1,
                outer_mesh_offset + m_segments_count - 1 + 1 + j * m_segments_count
            );
        }
    }
    else
    {
        /*
            In case when intersection point list is empty:

            first we need to connect last lgr layer with outer mesh nodes
            index of first outer node may be found like
            idx = (2 * m_spherical_offset + m_cylinder_count * m_segments_count) * m_mesh_count
            
            Numeration of outer nodes is similar to numeration at entry points cloud. Here we 
            must create a code like a connection of lgr layers

            Next step connect 'neareset' points. This algorithm is similar to connection of points at 2D case(?). 
        */

        auto outer_mesh_offset = (2 * m_spherical_offset + m_cylinder_count * m_segments_count) * m_mesh_count + m_well_offset;
        auto points_at_segments = (m_spherical_offset - 1) / m_segments_count;

        // first step: connett lg with outer nodes
        for(index_type i = 0; i < m_spherical_offset; i++)
        {

            edges.emplace_back(
                outer_mesh_offset + i,
                m_well_offset + (i + 1)* m_mesh_count - 1
            );
        }

        // Second step: connect outer mesh nodes with edges from polar point(0,0,0) to cylinder part of mesh

        // connect polar part of mesh
        for(index_type i = 1; i < m_segments_count + 1; i++)
        {
            edges.emplace_back(
                outer_mesh_offset,
                outer_mesh_offset + i
            );
        }

        // connect onder radial parts of mesh
        for(index_type i = 1; i < m_segments_count + 1; i++)
        {
            for(index_type j = 0; j < points_at_segments - 1; j++)
            {
                edges.emplace_back(
                    outer_mesh_offset + j * m_segments_count + i,
                    outer_mesh_offset + (j + 1) * m_segments_count + i
                );
            }
        }

        // Third step: connect outer mesh points by circles

        for(index_type j = 0; j < points_at_segments; j++)
        {
            for(index_type i = 0; i < m_segments_count - 1; i++)
            {
                edges.emplace_back(
                    outer_mesh_offset + j  + i * m_segments_count + 1,
                    outer_mesh_offset + (j + 1) + i * m_segments_count + 1
                );
            }

            edges.emplace_back(
                outer_mesh_offset + j * m_segments_count + 1,
                outer_mesh_offset + m_segments_count - 1 + 1 + j * m_segments_count
            );
        }
    }
}


void EdgesMesh::create_outer_mesh_edges_at_south()
{
    index_type total_north_sphere_offset = m_well_offset + m_mesh_count * m_spherical_offset - total_deleted_points_north();

    if(total_deleted_points_to_layer(m_mesh_count - 1))
    {
        auto outer_mesh_offset = m_well_offset + m_mesh_count * (3 * m_spherical_offset + m_cylinder_count * m_segments_count) - total_deleted_points_to_layer(m_mesh_count - 1);    
        auto points_at_segments = (m_spherical_offset - 1) / m_segments_count;

        // first step: connect lgr with outer nodes
        for(index_type i = 0; i < m_spherical_offset - total_deleted_points_to_layer(m_mesh_count - 1); i++)
        {
            edges.emplace_back(
                outer_mesh_offset + i + south_pole_intersected[m_mesh_count - 1],
                // for one mesh layer only
                m_well_offset + m_spherical_offset * m_mesh_count - total_deleted_points_to_layer(m_mesh_count - 1) + (i + 1) * m_mesh_count - !south_pole_intersected[m_mesh_count -1]
            );
        }

        // Second step: connect outer mesh nodes with edges from polar point(0,0,h) to cylinder part of mesh

        // connect polar part of mesh
        for(index_type i = 1; i < m_segments_count + 1; i++)
        {
            edges.emplace_back(
                outer_mesh_offset,
                outer_mesh_offset + i
            );
        }

        // connect onder radial parts of mesh
        for(index_type i = 1; i < m_segments_count + 1; i++)
        {
            for(index_type j = 0; j < points_at_segments - 1; j++)
            {
                edges.emplace_back(
                    outer_mesh_offset + j * m_segments_count + i,
                    outer_mesh_offset + (j + 1) * m_segments_count + i
                );
            }
        }

        // Third step: connect outer mesh points by circles

        for(index_type j = 0; j < points_at_segments; j++)
        {
            for(index_type i = 0; i < m_segments_count - 1; i++)
            {
                edges.emplace_back(
                    outer_mesh_offset + j  + i * m_segments_count + 1,
                    outer_mesh_offset + (j + 1) + i * m_segments_count + 1
                );
            }

            edges.emplace_back(
                outer_mesh_offset + j * m_segments_count + 1,
                outer_mesh_offset + m_segments_count - 1 + 1 + j * m_segments_count
            );
        }
    }
    else
    {
        auto outer_mesh_offset_south = (2 * m_spherical_offset + m_cylinder_count * m_segments_count) * m_mesh_count + m_well_offset + m_spherical_offset;
        auto points_at_segments = (m_spherical_offset - 1) / m_segments_count;

        // first step: connett lg with outer nodes
        for(index_type i = 0; i < m_spherical_offset; i++)
        {

            edges.emplace_back(
                outer_mesh_offset_south + i,
                total_north_sphere_offset + (i + 1)* m_mesh_count - 1
            );
        }

        // Second step: connect outer mesh nodes with edges from polar point(0,0,0) to cylinder part of mesh

        // connect polar part of mesh
        for(index_type i = 1; i < m_segments_count + 1; i++)
        {
            edges.emplace_back(
                outer_mesh_offset_south,
                outer_mesh_offset_south + i
            );
        }

        // connect onder radial parts of mesh
        for(index_type i = 1; i < m_segments_count + 1; i++)
        {
            for(index_type j = 0; j < points_at_segments - 1; j++)
            {
                edges.emplace_back(
                    outer_mesh_offset_south + j * m_segments_count + i,
                    outer_mesh_offset_south + (j + 1) * m_segments_count + i
                );
            }
        }

        // Third step: connect outer mesh points by circles

        for(index_type j = 0; j < points_at_segments; j++)
        {
            for(index_type i = 0; i < m_segments_count - 1; i++)
            {
                edges.emplace_back(
                    outer_mesh_offset_south + j  + i * m_segments_count + 1,
                    outer_mesh_offset_south + (j + 1) + i * m_segments_count + 1
                );
            }

            edges.emplace_back(
                outer_mesh_offset_south + j * m_segments_count + 1,
                outer_mesh_offset_south + m_segments_count - 1 + 1 + j * m_segments_count
            );
        }
    }
}


void EdgesMesh::create_cylinder_sphere_connection(bool north_pole, index_type mesh_index)
{

    if(total_deleted_points_to_layer(mesh_index) != 0)
    {
        auto deleted_points_offset = m_well_offset + (mesh_index + 1) * 2 * m_spherical_offset - total_deleted_points_to_layer(mesh_index);

        if(north_pole)
        {
            for(index_type i = 0; i < m_segments_count; i++)
            {
                edges.emplace_back(
                    //                                                     offset to 1 layer 
                    m_well_offset + m_spherical_offset - north_deleted_points[mesh_index]  - (m_segments_count * 2) + i * m_mesh_count + mesh_index,
                    deleted_points_offset + mesh_index * m_cylinder_count * m_segments_count + i

                );
            }
        }
        else
        {
            for(index_type i = 0; i < m_segments_count; i++)
            {
                edges.emplace_back(
                    //                                                     offset to 1 layer 
                    deleted_points_offset - (m_segments_count * 2) + i * m_mesh_count + mesh_index,
                    deleted_points_offset + m_segments_count * (m_cylinder_count - 1) + i

                );
            } 
        }
    }
    else
    {
        auto cylinder_offset = 2 * m_spherical_offset * m_mesh_count + m_well_offset;

        if(north_pole)
        {
            for(index_type i = 0; i < m_segments_count; i++)
            {
                edges.emplace_back(
                    //                                                     offset to 1 layer 
                    m_well_offset + m_mesh_count * m_spherical_offset - (m_segments_count * 2) + i * m_mesh_count + mesh_index,
                    cylinder_offset + mesh_index * m_cylinder_count * m_segments_count + i

                );
            }
        }
        else
        {
            for(index_type i = 0; i < m_segments_count; i++)
            {
                Edge edge;

                edge.set_first(2 * m_spherical_offset - m_segments_count + m_well_offset + i);
                edge.set_second(cylinder_offset + m_segments_count * (m_cylinder_count - 1) + i);
                
                edges.push_back(edge);
            } 
        }
    }

    
    
}


void EdgesMesh::create_edges_cylinder_by_equator_lines(index_type mesh_index)
{
    // connect by segments side
    if(total_deleted_points_to_layer(mesh_index) != 0)
    {
        auto deleted_points_offset = m_well_offset + (mesh_index + 1) * 2 * m_spherical_offset - total_deleted_points_to_layer(mesh_index);

        for(index_type i = 1; i < m_cylinder_count - 1; i++)
        {
            for(index_type j = 0; j < m_segments_count - 1; j++)
            {
                edges.emplace_back(
                    deleted_points_offset + j + i * m_segments_count + mesh_index * m_segments_count * m_cylinder_count,
                    deleted_points_offset + (j + 1) + i * m_segments_count + mesh_index * m_segments_count * m_cylinder_count
                );
            }

            edges.emplace_back(
                deleted_points_offset + i * m_segments_count + mesh_index * m_segments_count * m_cylinder_count,
                deleted_points_offset + m_segments_count - 1 + i * m_segments_count + mesh_index * m_segments_count * m_cylinder_count
            );
        }
    }
    else
    {
        auto cylinder_offset = 2 * m_spherical_offset * m_mesh_count + m_well_offset;

        // connect by segments side
        for(index_type i = 0; i < m_cylinder_count; i++)
        {
            for(index_type j = 0; j < m_segments_count - 1; j++)
            {
                edges.emplace_back(
                    cylinder_offset + j + i * m_segments_count + mesh_index * m_segments_count * m_cylinder_count,
                    cylinder_offset + (j + 1) + i * m_segments_count + mesh_index * m_segments_count * m_cylinder_count
                );
            }

            edges.emplace_back(
                cylinder_offset + i * m_segments_count + mesh_index * m_segments_count * m_cylinder_count,
                cylinder_offset + m_segments_count - 1 + i * m_segments_count + mesh_index * m_segments_count * m_cylinder_count
            );
        }
    }

    
}


void EdgesMesh::create_cylinder_longitude_edges(index_type mesh_index)
{
    if(north_pole_intersected[mesh_index])
    {
        auto deleted_points_offset = m_well_offset + (mesh_index + 1) * 2 * m_spherical_offset - total_deleted_points_to_layer(mesh_index);

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
    else
    {
        auto cylinder_offset = m_well_offset + 2 * m_spherical_offset - north_deleted_points[mesh_index] - south_deleted_points[mesh_index]; // why + 3 ??

        for (index_type i = 0; i < m_segments_count; i++)
        {
            for(index_type j = 0; j < m_cylinder_count - 1; j++)
            {
                edges.emplace_back(
                    cylinder_offset + mesh_index * m_cylinder_count * m_segments_count + j * m_segments_count + i,
                    cylinder_offset + mesh_index * m_cylinder_count * m_segments_count + (j + 1) * m_segments_count + i 
                );
            }
        }
    }
}


void EdgesMesh::create_cylidnder_to_well_edges(index_type mesh_index)
{
    if(north_pole_intersected[mesh_index])
    {
        auto deleted_points_offset = m_well_offset + (mesh_index + 1) * 2 * m_spherical_offset - total_deleted_points_to_layer(mesh_index);

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
        auto cylinder_offset = m_well_offset + 2 * m_spherical_offset - north_deleted_points[mesh_index] - south_deleted_points[mesh_index]; // why + 3 ??

        for(index_type i = 0; i < m_segments_count; i++)
        {
            for(index_type j = 0; j < m_cylinder_count; j++)
            {
                edges.emplace_back(
                    j,
                    cylinder_offset + mesh_index * m_cylinder_count * m_segments_count + j * m_segments_count + i
                );
            }
        }
    }
}