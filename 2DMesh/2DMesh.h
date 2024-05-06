#include "../precompile.h"

class TwoDMesh
{
    using point = bg::model::point<value_type, 2, bg::cs::cartesian>;
    
    using value_type = double;
    using index_type = int;
    using size_type = size_t;

    using vector_of_values = std::vector<value_type>;
    using vector_of_points = std::vector<point>;

private:
    vector_of_points points_list;
    fs::path m_path;

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

};