#pragma once

#include "../EdgesMesh.h"


index_type EdgesMesh::total_deleted_points_north()
{
    return std::accumulate(
        north_deleted_points.begin(),
        north_deleted_points.end(),
        0
    );
}


index_type EdgesMesh::total_deleted_points_south()
{
    return std::accumulate(
        south_deleted_points.begin(),
        south_deleted_points.end(),
        0
    );
}


index_type EdgesMesh::get_number_of_spherical_points()
{
    return 2 * m_spherical_offset * m_mesh_count - total_deleted_points_north() - total_deleted_points_south(); 
}