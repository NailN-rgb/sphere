#pragma once

#include "../precompile.h"

class TwoDMesh
{
private:
    vector_of_points m_points_list;
    bg::model::multi_point<point> m_boundary_points;
    fs::path m_path;

    vector_3d_points m_points_3d;

public:
    explicit TwoDMesh(
        const fs::path& path
    ) :
    m_path(path)
    {
        parse();
    }

private:
    void parse() 
    {
        if (!fs::exists(m_path)) {
            throw std::runtime_error("File not found");
        }
        else
        {
            std::ifstream fileStream(m_path);

            std::string line;
            while (std::getline(fileStream, line)) {
                std::cout << line << std::endl;
                std::cout << "Method is not writed now" << std::endl;
            }

            fileStream.close();
        }

        add_third_dimension();
    }

private:
    void create_boundary_points()
    {
        // TODO: May be we don't need use vector_of_points, using multi_point instead it?
        // entry data
        bg::model::multi_point<point> entry_points_mesh;

        std::for_each(
            m_points_list.begin(),
            m_points_list.end(),
            [entry_points_mesh](point p)
            {
                //bg::append(entry_points_mesh, p);
            }
        );

        // outer data 
        polygon hull;

        bg::convex_hull(entry_points_mesh, hull);

        //bg::assign(m_boundary_points, hull.outer());
    }

public:
    vector_3d_points get_points_list() const
    {
        return m_points_3d;
    }

public:
    vector_of_points get_boundary_points() const
    {
        return m_boundary_points;
    }

private:
    void add_third_dimension()
    {
        std::for_each(
            m_points_list.begin(),
            m_points_list.end(),
            [this](point p)
            {
                m_points_3d.push_back(
                    bg::make<point3d>(
                        bg::get<0>(p),
                        bg::get<1>(p),
                        0.
                    )
                );
            }
        );
    }



};