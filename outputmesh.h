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


void print3d_mesh(std::vector<point3d> pts)
{
    for(size_t i = 0; i < pts.size(); i++)
    {
        std::cout << "\n Point number " << i << std::endl;
        std::cout << "x : " << bg::get<0>(pts[i]) << std::endl;
        std::cout << "y : " << bg::get<1>(pts[i]) << std::endl;
        std::cout << "z : " << bg::get<2>(pts[i]) << std::endl;
    }
}


void write_mesh_data(std::ofstream stream, std::vector<point3d> mesh)
{
    
}


void form_vtk_file(std::vector<point3d> pts)
{
    std::ofstream file;

    file.open("mesh.vtk");

    if(file.is_open())
    {
        size_t points_count = pts.size();

        int vtk_mesh_type = 2;

        file << "# vtk DataFile Version 2.0" << std::endl;
        file << "Semi sphere mesh" << std::endl;
        file << "ASCII" << std::endl;
        file << "DATASET UNSTRUCTURED_GRID" << std::endl;
        file << "POINTS " << points_count << " float" << std::endl; 

        for (size_t i = 0; i < points_count; i++)
        {
            file << float(bg::get<0>(pts[i])) << "\t" << float(bg::get<1>(pts[i])) << "\t" << float(bg::get<2>(pts[i])) << std::endl;
        }

        file << "CELLS " << points_count << "\t" << points_count * 2 << std::endl;

        for(size_t i = 0; i < points_count; i++)
        {
            file << 1 << "\t" << i << std::endl;
        }

        file << "CELL_TYPES " << points_count << std::endl;

        for(size_t i = 0; i < points_count; i++)
        {
            file << 2 << std::endl;
        }

        file.close();
    }
    else
    {
        throw std::runtime_error("File can not to be opened");
    }
}