#pragma once

#include <cmath>
#include <fstream>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/algorithms/for_each.hpp>
#include <boost/geometry/algorithms/intersection.hpp>
#include <experimental/filesystem>

#include <iostream>
#include <deque>
#include <string>

namespace bg = boost::geometry;
namespace fs = std::experimental::filesystem;


using value_type = double;
using index_type = int;
using size_type = size_t;

using point = bg::model::point<value_type, 2, bg::cs::cartesian>;
using point3d = bg::model::point<value_type, 3, bg::cs::cartesian>;
using polygon = bg::model::polygon<point>;
using segment = bg::model::segment<point>;


using mesh_points_vector = std::vector<point3d>;
using vector_of_points = std::vector<point>;
using elements_vector= std::vector<std::vector<value_type>>;