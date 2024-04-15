#pragma once

#include "sphericmesh.h"

#include <cmath>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/algorithms/for_each.hpp>
#include <boost/geometry/algorithms/intersection.hpp>

#include <iostream>
#include <deque>

namespace bg = boost::geometry;
typedef bg::model::point<double, 2, bg::cs::cartesian> point;
typedef bg::model::point<double, 3, bg::cs::cartesian> point3d;
typedef bg::model::polygon<point> polygon;
typedef bg::model::segment<point> segment;


std::vector<point3d> rotate_mesh(const std::vector<point>& mesh, int rotations)
{
    float theta = degrees_to_radians(360 / rotations);

    std::vector<point3d> semi_sphere;

    semi_sphere.push_back(
        bg::make<point3d>(
            bg::get<0>(mesh.back()),
            bg::get<0>(mesh.back()),
            bg::get<1>(mesh.back())
        )
    );

    semi_sphere.push_back(
        bg::make<point3d>(
            bg::get<0>(mesh.front()),
            bg::get<0>(mesh.front()),
            bg::get<1>(mesh.front())
        )
    );

    for(size_t i = 0; i < rotations; i++)
    {
        for(size_t j = 1; j < mesh.size() - 1; j++)
        {
            semi_sphere.push_back(
                bg::make<point3d>(
                    bg::get<0>(mesh[j]) * cosf(theta * i),  // y -> x cos(theta)
                    bg::get<0>(mesh[j]) * sinf(theta * i),  // x -> x sin(theta)
                    bg::get<1>(mesh[j])                    // z -> y
                )
            );
        }
    }

    return semi_sphere;
}
