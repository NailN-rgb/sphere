#include "../precompile.h"
#include "../DataTypes/Element/Element.h"


class DualMesh
{
    typedef double value_type;
    typedef int index_type;

    typedef bg::model::point<double, 2, bg::cs::cartesian> point;
    typedef bg::model::point<double, 3, bg::cs::cartesian> point3d;

    typedef std::vector<value_type> vector_of_values;
    typedef std::vector<index_type> vector_of_indexes;

    typedef std::vector<point3d> mesh_points_vector;
    typedef std::vector<point> vector_of_points;
    typedef std::vector<Element> elements_vector;

    typedef std::vector<vector_of_indexes> FV;

private:
    elements_vector m_elems;
    mesh_points_vector m_mesh;

private:
    mesh_points_vector m_dual_points;

public:
    DualMesh(
        elements_vector elems,
        mesh_points_vector mesh
    ):
    m_elems(elems),
    m_mesh(mesh)
    {}

public:
    void get_dual_points();

private:
    point3d get_mass_center(vector_of_values element);

};