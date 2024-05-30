#pragma once

#include "../EdgesMesh.h"


index_type EdgesMesh::total_deleted_points_north(index_type mesh_layer)
{
        return std::accumulate(
            north_deleted_points.begin(),
            north_deleted_points.begin() + mesh_layer,
            0
        );
}


index_type EdgesMesh::total_deleted_points_south(index_type mesh_layer)
{
    return std::accumulate(
        south_deleted_points.begin(),
        south_deleted_points.begin() + mesh_layer,
        0
    );
}


index_type EdgesMesh::total_deleted_points_to_layer_north(index_type mesh_layer)
{
    return std::accumulate(
        north_deleted_points.begin(),
        north_deleted_points.begin() + mesh_layer,
        0);
}

// Method to get number of deleted points to this mesh layer


index_type EdgesMesh::get_number_of_spherical_points()
{
    return 2 * m_spherical_offset * m_mesh_count - total_deleted_points_north() - total_deleted_points_south(m_mesh_count); 
}

void EdgesMesh::get_spherical_radial_count()
{
    for(index_type i = 0; i < m_mesh_count; i++)
    {

    }
}