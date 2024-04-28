#pragma once

#include "../precompile.h"

class MeshProperties
{
public:
    value_type m_zw;
    value_type m_lw;
    value_type m_circle_radius;
    value_type m_layer_height;
    index_type m_points_size; 
    vector_of_points m_lgr_points;
    index_type m_segments;
    index_type m_cylinder_count;
    index_type m_mesh_count;

public:
    MeshProperties(
        value_type zw,
        value_type lw,
        value_type circle_radius,
        value_type layer_height,
        const vector_of_points& lgr_points,
        index_type segments, 
        index_type cylinder_count,
        index_type mesh_count
    ) :
    m_zw(zw),
    m_lw(lw),
    m_circle_radius(circle_radius),
    m_layer_height(layer_height),
    m_lgr_points(lgr_points),
    m_points_size(lgr_points.size()),
    m_segments(segments),
    m_cylinder_count(cylinder_count),
    m_mesh_count(mesh_count)
    {}

};