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
    index_type m_deleted_points = 0;

public:
    vector_3d_points m_nodes; // resulted mesh points
    std::vector<bool> north_deleted;
    std::vector<bool> south_deleted;
    vector_of_indexes north_sphere_deleted_count;
    vector_of_indexes south_sphere_deleted_count;

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

        for(index_type i = 0; i < m_prop.m_mesh_count; i++)
        {
            north_sphere_deleted_count.push_back(0);
            south_sphere_deleted_count.push_back(0);

            north_deleted.push_back(false);
            south_deleted.push_back(false);
        }
    }

public:
    void create_Nodes()
    {
        create_well_nodes();
        create_semisphere_points();
        create_cylindrical_nodes();
        create_inner_mesh();
        form_vtk_file();

        std::cout << "2D points count: " << m_entry_2d_mesh.size() << std::endl;
        std::cout << "2D points count * log_mesh_size: " << m_entry_2d_mesh.size() * m_log_mesh.size() << std::endl;


        std::cout << "Deleted " << m_deleted_points << " points" << std::endl;
    }

private:
    void create_log_mesh()
    {
        for(index_type i = 1; i <= m_prop.m_mesh_count; i++)
        {
            m_log_mesh.push_back(std::exp(i * (std::log(m_prop.m_max_radius) / m_prop.m_mesh_count)));
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
                auto dx = bg::get<0>(p) - bg::get<0>(pwN);
                auto dy = bg::get<1>(p) - bg::get<1>(pwN);
                auto dz = bg::get<2>(pwN) - bg::get<2>(p);

                auto r_at_xy = std::sqrt(dx * dx + dy * dy);

                auto alpha_angle = std::atan(dy/dx);

                value_type betta_angle;

                if(r_at_xy == 0)
                {
                    betta_angle = std::atan(1.) * 2;
                }
                else
                {
                    betta_angle = std::atan(dz/r_at_xy);
                }

                // loop by log mesh points 
                for(index_type i = 0; i < m_prop.m_mesh_count; i++)
                {
                    // if z projection of logmesh part + pwN.z <= p.Z
                    if(bg::get<2>(pwN) - m_log_mesh[i] * std::sin(betta_angle) >= bg::get<2>(p)) // bg::get 2 (p) is 0 
                    {
                        auto dist_from_p_to_pnW = bg::distance(pwN, p);

                        auto ratio_quef = m_log_mesh[i] / dist_from_p_to_pnW;

                        auto newx = bg::get<0>(pwN) + (bg::get<0>(p) - bg::get<0>(pwN)) * ratio_quef;
                        auto newy = bg::get<1>(pwN) + (bg::get<1>(p) - bg::get<1>(pwN)) * ratio_quef;
                        auto newz = bg::get<2>(pwN) - (bg::get<2>(pwN) - bg::get<2>(p)) * ratio_quef;

                        m_nodes.push_back(bg::make<point3d>(
                            newx,
                            newy,
                            newz
                        ));
                    }
                    else
                    {
                        north_deleted[i] = true;
                        north_sphere_deleted_count[i]++;
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
                auto dx = bg::get<0>(p) - bg::get<0>(pwS);
                auto dy = bg::get<1>(p) - bg::get<1>(pwS);
                auto dz = bg::get<2>(p) + m_prop.m_layer_height - bg::get<2>(pwS);

                auto r_at_xy = std::sqrt(dx * dx + dy * dy);

                auto alpha_angle = std::atan(dy/dx);
                value_type betta_angle;

                if(r_at_xy == 0)
                {
                    betta_angle = std::atan(1.) * 2;
                }
                else
                {
                    betta_angle = std::atan(dz/r_at_xy);
                }

                // loop by log mesh points 
                for(index_type i = 0; i < m_prop.m_mesh_count; i++)
                {
                    // if z projection of logmesh part + pwS.z <= p.Z
                    if(bg::get<2>(pwS) + m_log_mesh[i] * std::sin(betta_angle) <= bg::get<2>(p) + m_prop.m_layer_height)
                    {
                        auto dist_from_p_to_pwS = bg::distance(pwS, bg::make<point3d>(p.get<0>(), p.get<1>(), p.get<2>() + m_prop.m_layer_height));

                        auto ratio_quef = m_log_mesh[i] / dist_from_p_to_pwS;

                        auto newx = bg::get<0>(pwS) + (bg::get<0>(p) - bg::get<0>(pwS)) * ratio_quef;
                        auto newy = bg::get<1>(pwS) + (bg::get<1>(p) - bg::get<1>(pwS)) * ratio_quef;
                        auto newz = bg::get<2>(pwS) + (bg::get<2>(p) + m_prop.m_layer_height - bg::get<2>(pwS)) * ratio_quef;

                        m_nodes.push_back(bg::make<point3d>(
                            newx,
                            newy,
                            newz
                        ));
                    }
                    else
                    {
                        south_deleted[i] = true;
                        south_sphere_deleted_count[i]++;
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
                        bg::get<2>(pwN) + i * (m_prop.m_lw / (m_prop.m_cylinder_count - 1)) 
                    )
                );
        }

        m_nodes.push_back(pwS);
    }

private:
    void create_cylindrical_nodes()
    {
        auto h_angle  = 360 / m_prop.m_segments * std::atan(1.) / 45; // angle at radians 
        auto h_at_cyl = m_prop.m_lw / (m_prop.m_cylinder_count - 1);

        for(index_type i = 0; i < m_prop.m_mesh_count; i++)
        {
            for(index_type j = 0; j < m_prop.m_cylinder_count; j++)
            {
                for(index_type k = 0; k < m_prop.m_segments; k++)
                {
                    auto x_coord_of_created_point = m_log_mesh[i] * std::cos(k * h_angle); // to connect with spherical part
                    auto y_coord_of_created_point = m_log_mesh[i] * std::sin(-k * h_angle);
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
    void create_inner_mesh()
    {
        // create vecrot yo all of the entry nodes
        // if entry node lay inside circle with rad = m_prop.max_rad, we add node to mesh without changes
        // else we find projection of a point: we need to find point that lay at "cylinder" with rad = m_prop.max_rad 
        //                                                                  with unknown value of z - coordinate

        std::for_each(
            m_entry_2d_mesh.begin(),
            m_entry_2d_mesh.end(),
            [this](point3d p)
            {
                // find direction
                // alpha = atan(dz/dx)
                auto dx = bg::get<0>(p) - bg::get<0>(pwN);
                auto dy = bg::get<1>(p) - bg::get<1>(pwN);
                auto dz = bg::get<2>(pwN) - bg::get<2>(p);

                auto r_at_xy = std::sqrt(dx * dx + dy * dy);

                auto alpha_angle = std::atan(dy/dx);
                auto betta_angle = std::atan(r_at_xy/dz);

                if(r_at_xy <= m_prop.m_max_radius) 
                {
                    m_nodes.push_back(p);
                }
                else
                {
                    // get x y coordinates at point with rad m_max_rad with angle from x axis equal to alpha_angle
                    auto compatible_radius = m_prop.m_max_radius;
                    // auto angle_r_to_z      = std::atan(compatible_radius / dz); 

                    auto x_coord = m_prop.m_max_radius * (dx / std::sqrt(dx * dx + dy * dy));
                    auto y_coord = m_prop.m_max_radius * (dy / std::sqrt(dx * dx + dy * dy));

                    value_type z_coord = bg::get<2>(pwN) - compatible_radius / std::tan(betta_angle);
                    // value_type z_coord = compatible_radius * dz / dx;

                    m_nodes.push_back(
                        bg::make<point3d>(
                            x_coord,
                            y_coord,
                            z_coord
                        )
                    );
                }
                
            }
        );

        // create downside part of mesh

        std::for_each(
            m_entry_2d_mesh.begin(),
            m_entry_2d_mesh.end(),
            [this](point3d p)
            {
                auto p_downside = bg::make<point3d>(
                    bg::get<0>(p),
                    bg::get<1>(p),
                    bg::get<2>(p) + m_prop.m_layer_height
                );
                // find direction
                // alpha = atan(dz/dx)
                auto dx = bg::get<0>(p_downside) - bg::get<0>(pwS);
                auto dy = bg::get<1>(p_downside) - bg::get<1>(pwS);
                auto dz = bg::get<2>(p_downside) - bg::get<2>(pwS);

                auto r_at_xy = std::sqrt(dx * dx + dy * dy);

                auto alpha_angle = std::atan(dy/dx);
                auto betta_angle = std::atan(r_at_xy/dz);

                if(r_at_xy <= m_prop.m_max_radius) 
                {
                    m_nodes.push_back(p_downside);
                }
                else
                {
                    // get x y coordinates at point with rad m_max_rad with angle from x axis equal to alpha_angle
                    auto compatible_radius = m_prop.m_max_radius;
                    // auto angle_r_to_z      = std::atan(compatible_radius / dz); 

                    auto x_coord = m_prop.m_max_radius * (dx / std::sqrt(dx * dx + dy * dy));
                    auto y_coord = m_prop.m_max_radius * (dy / std::sqrt(dx * dx + dy * dy));

                    value_type z_coord = bg::get<2>(pwS) + compatible_radius / std::tan(betta_angle);


                    m_nodes.push_back(
                        bg::make<point3d>(
                            x_coord, // TODO: check
                            y_coord,
                            z_coord
                        )
                    );
                }
            }
        );
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

            file << "CELLS " << points_count << "\t" << 2 * points_count << std::endl;

            for(size_t i = 0; i < points_count; i++)
            {
                file << 1 << "\t" << i << std::endl;
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