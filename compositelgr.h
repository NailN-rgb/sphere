#pragma once

#include "outputmesh.h"
#include "rotatemesh.h"
#include "sphericmesh.h"
#include "compositemesh.h"


std::vector<point3d> well_trajectory(double zw, double lw, int points_count, double radius)
{
    std::vector<point> well_seg = get_cylinder_mesh(zw, lw, points_count, radius);

    std::vector<point3d> result;

    std::for_each(
        well_seg.begin(),
        well_seg.end(),
        [&radius](point p)
        {
            result.push_back(
                bg::make<point3d>(
                    0,
                    0,
                    bg::get<1>(p)
                )
            );
        }
    );

    return result;
}


std::vector<std::vector<point3d>> get_elements(std::vector<point3d> &mesh)
{
    


}


void composite_lgr()
{
    std::vector<point3d> 
}