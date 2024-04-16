#pragma once

double degrees_to_radians(double deg)
{
    return deg * 4. * std::atan(1.) / 180;
}


polygon get_semi_circle(point pw, double radius, int point_count = 10)
{
    polygon semi_circle;

    bg::append(semi_circle, point(bg::get<0>(pw), bg::get<1>(pw) - radius));

    // create semi-circle points 
    for(int i = 1; i <= point_count; ++i)
    {
        double angle = degrees_to_radians(90 - 90 / point_count * i);
        bg::append(semi_circle, point(bg::get<0>(pw) + radius * cos(angle), bg::get<1>(pw) - radius * sin(angle)));
    }

    bg::append(semi_circle, point(bg::get<0>(pw), bg::get<1>(pw)));

    // append last point to polygon
    bg::append(semi_circle, point(bg::get<0>(pw), bg::get<1>(pw) + radius));

    return semi_circle;
}


segment get_line_seg(point pw, point point_out)
{
    segment line(pw, point_out);
    return line;
}

point get_intersect(polygon pgn, segment line)
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

// точки в points должны идти в пордке убывания координаты х 
vector_of_points get_sphere_approx(vector_of_points &points, point pw, double lw, double radius, bool is_up)
{
    vector_of_points result;

    result.push_back(
        bg::make<point>(
            bg::get<0>(pw),
            bg::get<1>(pw) - radius
        )
    );
    
   polygon circle = get_semi_circle(pw, radius);


    for(auto pt : points)
    {
        segment line = get_line_seg(pw, pt);

        result.push_back(get_intersect(circle, line));
    }

    // result.push_back(
    //     bg::make<point>(
    //         bg::get<0>(pw) + radius,
    //         bg::get<1>(pw)
    //     )
    // );
    
    if(!is_up)
    {
        std::for_each(
            result.begin(),
            result.end(),
            [&pw, lw](point &p)
            {
                bg::set<1>(p, -1 * p.get<1>() + 2 * pw.get<1>() + lw);
            }
        );
    }
    

    return result;
}