#include "../precompile.h"
#include "../2DMesh/2DMesh.h"
#include "../MeshProperties/MeshProperties.h"

class NodeMesh
{
    using point = bg::model::point<value_type, 2, bg::cs::cartesian>;
    using point3d = bg::model::point<value_type, 3, bg::cs::cartesian>;
    
    using value_type = double;
    using index_type = int;
    using size_type = size_t;

    using vector_of_values = std::vector<value_type>;
    using vector_of_points = std::vector<point>;
    using vector_3d_points = std::vector<point3d>;

private:
    // entry data
    vector_3d_points m_resulted_mesh;
    vector_of_values m_atrributes_list;
    fs::path m_path;
    TwoDMesh m_flat_mesh;
    MeshProperties m_prop;

private:
    point3d pwN; // well north pheel
    point3d pwS; // well south pheel
    point3d pw0; // well projection at 2d

    vector_of_values m_log_mesh;

private:
    void create_boundary_edges()
    {

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

        // find direction

        // create points

        
    }



};