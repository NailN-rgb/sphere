#pragma once


point get_intersection_of_line_segments_flat(segment l1, segment l2)
{
    if(bg::intersects(l1, l2))
    {
        vector_of_points intersection_points;

        bg::intersection(l1, l2, intersection_points);

        if(intersection_points.size() > 1)
        {
            throw std::runtime_error("More than 1 intrersection point at segment");
        }

        return intersection_points.front();
    }
}


vector_of_points FlatMesh::get_inner_points(
        value_type max_rad,
        bool is_upperside_points
)
{
    vector_of_points inner_mesh;

    auto pw = point(0, 0);

    inner_mesh.push_back(bg::make<point>(0.0, 0.0));
    inner_mesh.push_back(bg::make<point>(0.0, m_layer_height));

    segment cylinder_side = get_line_seg(
                                bg::make<point>(max_rad, 0.0),
                                bg::make<point>(max_rad, m_layer_height));

    std::for_each(
        m_lgr_points.begin(),
        m_lgr_points.end(),
        [this, &max_rad, &is_upperside_points, &inner_mesh, &pw, &cylinder_side](point p)
        {
            // if point lay at cylinder 
            if(bg::get<0>(p) <  max_rad)
            {
                if(is_upperside_points)
                {
                    inner_mesh.push_back(p);
                }
                else
                {
                    inner_mesh.push_back(bg::make<point>(
                        bg::get<0>(p), bg::get<1>(p) + m_layer_height));
                }
            }
            // if point do not lay at cylinder
            else
            {
                if(is_upperside_points)
                {
                    pw = bg::make<point>(0.0, m_zw - m_lw/2);
                }
                else
                {
                    p = bg::make<point>(bg::get<0>(p), bg::get<1>(p) + m_layer_height);
                    pw = bg::make<point>(0.0, m_zw + m_lw/2);
                }

                inner_mesh.push_back(get_intersection_of_line_segments_flat(cylinder_side, get_line_seg(pw, p)));
            }
        }
    );
    
    return inner_mesh;
}


vector_of_points FlatMesh::get_sphere_approx(
    point pw,
    bool is_up
)
{
    vector_of_points result;

    result.push_back(
        bg::make<point>(
            bg::get<0>(pw),
            bg::get<1>(pw) - m_circle_radius
        )
    );
    
    polygon circle_trajectory = get_semi_circle(pw, m_circle_radius);

    // get intersection between given circle segment and lines to lgr points
    for(auto pt : m_lgr_points)
    {
        segment line = get_line_seg(pw, pt);

        result.push_back(get_intersect(circle_trajectory, line));
    }
    
    // special case for downside sphere
    if(!is_up)
    {
        std::for_each(
            result.begin(),
            result.end(),
            [this, &pw](point &p)
            {
                bg::set<1>(p, -1 * p.get<1>() + 2 * pw.get<1>() + m_lw);
            }
        );
    }
    

    return result;
}


vector_of_points FlatMesh::get_cylinder_mesh(
    bool is_not_well_trajectory
)
{
    // regular mesh
    value_type h = m_lw / (m_cylinder_count - 1);

    vector_of_points cylinder_mesh;

    for(int i = 0; i < m_cylinder_count; i++)
    {
        if(is_not_well_trajectory)
        {
            cylinder_mesh.push_back(
                bg::make<point>(
                    value_type(m_circle_radius), 
                    m_zw - m_lw/2 + i * h
                )
            );
        }
        else
        {
            cylinder_mesh.push_back(
                bg::make<point>(
                    0.0, 
                    m_zw - m_lw/2 + i * h
                )
            );
        }
        
    }

    return cylinder_mesh;
}