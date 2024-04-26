#include "../DualFVMesh"


point3d DualFV::get_mass_center(vector_of_indexes element)
{
    vector_of_points element_points;
    size_t elem_size = element.size;

    std::for_each(
        element.begin(),
        element.end(),
        [&element_points, this](index_type idx)
        {
            element_points.push_back(
                m_mesh[idx]
            );
        }
    )

    point3d mass_center;

    mass_center = std::accumulate(
        element_points.begin(),
        element_points.end(),
        0
    );

    return point3d(bg::get<0>(mass_center) / elem_size,
                   bg::get<1>(mass_center) / elem_size,
                   bg::get<2>(mass_center) / elem_size);
}


void DualFV::get_dual_points()
{
    std::for_each(
        m_elems.begin(),
        m_elems.end(),
        [](auto element)
        {
            m_dual_points.push_back(
                get_mass_center(element);
            );
        }
    )
}
