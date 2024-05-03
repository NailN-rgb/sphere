#pragma once

#include "../precompile.h"
#include "../2DMesh/2DMesh.h"
#include "../MeshProperties/MeshProperties.h"

class NodeMesh
{
private:
    // entry data
    vector_3d_points m_entry_2d_mesh; // full list of entry 2d points with z coord taken by zero
    vector_of_indexes m_atrributes_list; // list of boundary point indexes from m_entry_2d_mesh
    MeshProperties m_prop;

    vector_3d_points m_nodes; // resulted mesh points

private:
    point3d pwN; // well north pheel
    point3d pwS; // well south pheel

    vector_of_values m_log_mesh;

public:
    NodeMesh(
        const vector_3d_points &entry_2d_mesh, 
        const MeshProperties &prop
    ) :
    m_entry_2d_mesh(entry_2d_mesh),
    m_prop(prop)
    {
        // TODO: create it at non-zero pw coordinates
        pwN = bg::make<point3d>(
            0.,
            0.,
            m_prop.m_zw - m_prop.m_lw / 2
        );

        pwS = bg::make<point3d>(
            0.,
            0.,
            m_prop.m_zw + m_prop.m_lw / 2
        );

        create_log_mesh();
    }

public:
    void create_Nodes()
    {
        create_well_nodes();
        create_semisphere_points();
        create_cylindrical_nodes();
        form_vtk_file();
    }

private:
    void create_log_mesh()
    {
        for(index_type i = 0; i < m_prop.m_cylinder_count; i++)
        {
            m_log_mesh.push_back(std::exp(i * (std::log(m_prop.m_max_radius) / m_prop.m_cylinder_count)));
        }
    }

private:
    void create_semisphere_points()
    {           
        // create 3d segment
        std::for_each(
            m_entry_2d_mesh.begin(),
            m_entry_2d_mesh.end(),
            [this](point3d p)
            {
                // find direction
                // alpha = atan(dz/dx)
                auto alpha_angle = std::atan((bg::get<2>(p) - bg::get<2>(pwN))/(std::abs(bg::get<0>(p) - bg::get<0>(pwN))));
                auto betta_angle = std::atan((bg::get<1>(p) - bg::get<1>(pwN))/(std::abs(bg::get<0>(p) - bg::get<0>(pwN))));

                // loop by log mesh points 
                for(index_type i = 0; i < m_prop.m_cylinder_count; i++)
                {
                    // if z projection of logmesh part + pwN.z <= p.Z
                    if(bg::get<2>(pwN) + m_log_mesh[i] * std::sin(betta_angle) <= bg::get<2>(p))
                    {
                        auto newx = bg::get<0>(pwN) + m_log_mesh[i] * std::sin(alpha_angle);
                        auto newy = bg::get<1>(pwN) + m_log_mesh[i] * std::cos(alpha_angle);
                        auto newz = bg::get<2>(pwN) + m_log_mesh[i] * std::sin(betta_angle);
                        
                        m_nodes.push_back(bg::make<point3d>(
                            newx,
                            newy,
                            newz
                        ));
                    }
                    else
                    {
                        // get length of segment from pwN to intersection with plane
                        auto segment_length = (bg::get<2>(pwN) - bg::get<2>(p)) / std::sin(betta_angle);

                        // after that we may find other coordinates of intersection point
                        
                        m_nodes.push_back(bg::make<point3d>(
                            bg::get<0>(pwN) + segment_length * std::sin(alpha_angle),
                            bg::get<1>(pwN) + segment_length * std::cos(alpha_angle),
                            bg::get<2>(pwN) + segment_length * std::sin(betta_angle)
                        ));

                        // exit from loop
                        return;
                    }
                }
            }
        );

        // create downside part of mesh

        std::for_each(
            m_entry_2d_mesh.begin(),
            m_entry_2d_mesh.end(),
            [this](point3d p)
            {
                // find direction
                // alpha = atan(dz/dx)
                auto alpha_angle = std::atan((bg::get<2>(p) - bg::get<2>(pwS))/(std::abs(bg::get<0>(p) - bg::get<0>(pwS))));
                auto betta_angle = std::atan((bg::get<1>(p) - bg::get<1>(pwS))/(std::abs(bg::get<0>(p) - bg::get<0>(pwS))));

                // loop by log mesh points 
                for(index_type i = 0; i < m_prop.m_cylinder_count; i++)
                {
                    // if z projection of logmesh part + pwS.z <= p.Z
                    if(bg::get<2>(pwS) + m_log_mesh[i] * std::sin(betta_angle) <= bg::get<2>(p))
                    {
                        auto newx = bg::get<0>(pwS) + m_log_mesh[i] * std::sin(alpha_angle);
                        auto newy = bg::get<1>(pwS) + m_log_mesh[i] * std::cos(alpha_angle);
                        auto newz = bg::get<2>(pwS) + m_log_mesh[i] * std::sin(betta_angle);
                        
                        m_nodes.push_back(bg::make<point3d>(
                            newx,
                            newy,
                            newz
                        ));
                    }
                    else
                    {
                        // get length of segment from pwN to intersection with plane
                        auto segment_length = (bg::get<2>(pwS) - bg::get<2>(p)) / std::sin(betta_angle);

                        // after that we may find other coordinates of intersection point
                        
                        m_nodes.push_back(bg::make<point3d>(
                            bg::get<0>(pwS) + segment_length * std::sin(alpha_angle),
                            bg::get<1>(pwS) + segment_length * std::cos(alpha_angle),
                            bg::get<2>(pwS) + segment_length * std::sin(betta_angle)
                        ));

                        // exit from loop
                        return;
                    }
                }
            }
        );
    }


private:
    void create_well_nodes()
    {
        m_nodes.push_back(pwN);

        for(index_type i = 1; i < m_prop.m_cylinder_count - 1; i++)
        {
            m_nodes.push_back(
                bg::make<point3d>(
                        bg::get<0>(pwN),
                        bg::get<1>(pwN),
                        bg::get<2>(pwN) + i * (m_prop.m_lw / m_prop.m_cylinder_count) 
                    )
                );
        }

        m_nodes.push_back(pwS);
    }

private:
    void create_cylindrical_nodes()
    {
        auto h_angle  = 360 / m_prop.m_segments * std::atan(1.) / 45; // angle at radians 
        auto h_at_cyl = m_prop.m_lw / m_prop.m_cylinder_count;

        for(index_type i = 0; i < m_prop.m_cylinder_count; i++)
        {
            for(index_type j = 0; j < m_prop.m_cylinder_count; j++)
            {
                for(index_type k = 0; k < m_prop.m_segments; k++)
                {
                    auto x_coord_of_created_point = m_log_mesh[i] * std::cos(k * h_angle);
                    auto y_coord_of_created_point = m_log_mesh[i] * std::sin(k * h_angle);
                    auto z_coord_of_created_point = bg::get<2>(pwN) + j * h_at_cyl;

                    m_nodes.push_back(
                        bg::make<point3d>(
                            x_coord_of_created_point,
                            y_coord_of_created_point,
                            z_coord_of_created_point
                        )
                    );
                }
            }
        }
    }

private:
    void form_vtk_file()
    {
        std::ofstream file;

        file.open("nodes.vtk");

        if(file.is_open())
        {
            size_t points_count = m_nodes.size();

            int vtk_mesh_type = 2;

            file << "# vtk DataFile Version 2.0" << std::endl;
            file << "Semi sphere mesh" << std::endl;
            file << "ASCII" << std::endl;
            file << "DATASET UNSTRUCTURED_GRID" << std::endl;
            file << "POINTS " << points_count << " float" << std::endl; 

            for (size_t i = 0; i < points_count; i++)
            {
                file << float(bg::get<0>(m_nodes[i])) << "\t" << float(bg::get<1>(m_nodes[i])) << "\t" << float(bg::get<2>(m_nodes[i])) << std::endl;
            }

            file << "CELL_TYPES " << points_count  << std::endl;

            for(size_t i = 0; i < points_count; i++)
            {
                file << 2 << std::endl;
            }

            file.close();
        }
        else
        {
            throw std::runtime_error("File can not to be opened");
        }
    }



};