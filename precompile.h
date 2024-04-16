#pragma once

#include <cmath>
#include <fstream>
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


typedef double value_type;
typedef int index_type;
typedef size_t size_type;

typedef std::vector<point3d> mesh_points_vector;
typedef std::vector<point> vector_of_points;
typedef std::vector<std::vector<double>> elements_vector;