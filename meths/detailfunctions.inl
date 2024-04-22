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
