#pragma once
#include "FlatMesh.inl"


value_type degrees_to_radians_flat(value_type deg)
{
    return deg * 4. * std::atan(1.) / 180;
}


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

    segment cylinder_side = segment(
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

                inner_mesh.push_back(get_intersection_of_line_segments_flat(cylinder_side, segment(pw, p)));
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
        segment line = segment(pw, pt);

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


/**
 * That code creates a seci-circle to use in circle points mesh builder
 * @param pw center of a circle
 * @param radius radius of a circle
 * @param point_count number of a points at circle, may be used to increase a interpolation quality
*/
polygon FlatMesh::get_semi_circle(
    point pw, 
    value_type radius, 
    index_type point_count
)
{
    // create polygon that after we fill
    polygon semi_circle;

    // append the polar point to polygon
    bg::append(semi_circle, point(bg::get<0>(pw), bg::get<1>(pw) - radius));

    // create semi-circle points 
    for(int i = 1; i <= point_count; ++i)
    {
        value_type angle = degrees_to_radians_flat(90 - 90 / point_count * i);
        bg::append(semi_circle, point(bg::get<0>(pw) + radius * cos(angle), bg::get<1>(pw) - radius * sin(angle)));
    }

    // append circle center to polygon
    bg::append(semi_circle, point(bg::get<0>(pw), bg::get<1>(pw)));

    // append last point to polygon
    bg::append(semi_circle, point(bg::get<0>(pw), bg::get<1>(pw) + radius));

    return semi_circle;
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


point FlatMesh::get_intersect(polygon pgn, segment line)
{
    std::deque<polygon> output;

    for(size_t i = 0; i < pgn.outer().size() - 1; i++)
    {
        // get each part of polygon
        segment polygon_seg = segment(
            bg::make<point>(
                bg::get<0>(pgn.outer()[i]),
                bg::get<1>(pgn.outer()[i])
            ),
            bg::make<point>(
                bg::get<0>(pgn.outer()[i + 1]),
                bg::get<1>(pgn.outer()[i + 1])
            )
        );

        // if intersects
        if(bg::intersects(polygon_seg, line))
        {
            vector_of_points res;

            bg::intersection(polygon_seg, line, res);

            if(res.size() > 1)
            {
                throw std::runtime_error("More than 1 intrersection point at segment");
            }

            if(bg::get<0>(res[0]) !=0 && bg::get<1>(res[0]) != 0)
            {
                return res[0];
            }
        }
    }
}