#include "../precompile.h"

class TwoDMesh
{
    using point = bg::model::point<value_type, 2, bg::cs::cartesian>;
    using point3d = bg::model::point<value_type, 3, bg::cs::cartesian>;

    
    using value_type = double;
    using index_type = int;
    using size_type = size_t;

    using vector_of_values = std::vector<value_type>;
    using vector_of_points = std::vector<point>;
    using vector_of_points3d = std::vector<point3d>;


private:
    vector_of_points m_points_list;
    vector_of_points m_boundary_points;
    fs::path m_path;

    vector_of_points3d m_points_3d;

public:
    explicit TwoDMesh(
        const fs::path& path
    ) :
    m_path(path) 
    {
        parse();
    }

private:
    void parse() const
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
    }

public:
    vector_of_points get_points_list()
    {
        return m_points_list;
    }

public:
    vector_of_points get_boundary_points()
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