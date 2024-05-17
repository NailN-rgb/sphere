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


index_type EdgesMesh::total_deleted_points_to_layer(index_type mesh_layer)
{
    auto deleted_count = 0;
    
    for(index_type i = 0; i < mesh_layer + 1; i++)
    {
        deleted_count += north_deleted_points[i] + south_deleted_points[i];
    }

    return deleted_count;
}

// Method to get number of deleted points to this mesh layer


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