#pragma once


point get_intersection_of_line_segments(segment l1, segment l2)
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

/**
    * Generation of a cylinder mesh
    * @param zw well centers depth
    * @param lw well length
    * @param points_count the number of points at cylinder
    * @param radius the radius of cylinder
    * 
    * @return vector of points
    * 
    * @throws None
*/
vector_of_points get_cylinder_mesh(
    value_type zw,
    value_type lw,
    index_type points_count,
    value_type radius
)
{
    // regular mesh
    value_type h = lw / (points_count - 1);

    vector_of_points cylinder_mesh;

    for(size_t i = 0; i < points_count; i++)
    {
        cylinder_mesh.push_back(
            bg::make<point>(
                radius, 
                zw - lw/2 + i * h
            )
        );
    }

    return cylinder_mesh;
}



mesh_points_vector make_composite_mesh(
    value_type zw,
    value_type lw,
    value_type circle_radius,
    value_type layer_height,
    const vector_of_points &lgr_points,
    index_type segments,
    index_type cylinder_count,
    index_type mesh_count
)
{
    // first create semi circle by params

    // calculate pw

    point top_circle_center    = point(0, zw - lw/2);
    point bottom_circle_center = point(0, layer_height - zw - lw/2);

    vector_of_points top_circle   = get_sphere_approx(lgr_points, top_circle_center, lw, circle_radius, true);
    vector_of_points bottom_circle = get_sphere_approx(lgr_points, bottom_circle_center, lw, circle_radius, false);

    vector_of_points cylinder_mesh = get_cylinder_mesh(zw, lw, cylinder_count, circle_radius);

    vector_of_points capsule_mesh;
    capsule_mesh.reserve(top_circle.size() + bottom_circle.size() + cylinder_mesh.size());
    capsule_mesh.insert(capsule_mesh.end(), top_circle.begin(), top_circle.end());
    capsule_mesh.insert(capsule_mesh.end(), cylinder_mesh.begin(), cylinder_mesh.end());

    std::reverse(bottom_circle.begin(), bottom_circle.end());

    capsule_mesh.insert(capsule_mesh.end(), bottom_circle.begin(), bottom_circle.end());

    return rotate_mesh(capsule_mesh, segments, true);
}


vector_of_points get_inner_points(
    value_type zw,
    value_type lw,
    value_type h,
    const vector_of_points &lgr_points,
    value_type max_rad,
    bool is_upperside_points
)
{
    vector_of_points inner_mesh;

    inner_mesh.push_back(bg::make<point>(0.0, 0.0));
    inner_mesh.push_back(bg::make<point>(0.0, h));

    segment cylinder_side = get_line_seg(
                                bg::make<point>(max_rad, 0.0),
                                bg::make<point>(max_rad, h));

    std::for_each(
        lgr_points.begin(),
        lgr_points.end(),
        [&zw, &lw, &h, &max_rad, &inner_mesh, &cylinder_side, &is_upperside_points](point p)
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
                        bg::get<0>(p), bg::get<1>(p) + h));
                }
            }
            // if point do not lay at cylinder
            else
            {
                point pw = point(0, 0);

                if(is_upperside_points)
                {
                    pw = bg::make<point>(0.0, zw - lw/2);
                }
                else
                {
                    p = bg::make<point>(bg::get<0>(p), bg::get<1>(p) + h);
                    pw = bg::make<point>(0.0, zw + lw/2);
                }

                segment point_to_pw_line = get_line_seg(pw, p);

                inner_mesh.push_back(get_intersection_of_line_segments(cylinder_side, point_to_pw_line));
            }
        }
    );
    
    return inner_mesh;
}



