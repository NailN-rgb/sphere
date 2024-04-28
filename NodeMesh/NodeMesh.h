#include "../precompile.h"
#include "../2DMesh/2DMesh.h"

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
    vector_3d_points m_resulted_mesh;
    fs::path m_path;
    TwoDMesh m_flat_mesh;


};