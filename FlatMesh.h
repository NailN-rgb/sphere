#pragma once

#include "precompile.h"

#include "MeshProperties/MeshProperties.h"

class FlatMesh
{
    typedef bg::model::point<double, 2, bg::cs::cartesian> point;
    typedef bg::model::polygon<point> polygon;
    typedef bg::model::segment<point> segment;
    
    typedef double value_type;
    typedef int index_type;
    typedef size_t size_type;

    typedef std::vector<point> vector_of_points;

public:
    value_type m_zw               = 5;
    value_type m_lw               = 3;
    value_type m_circle_radius    = 1;
    value_type m_layer_height     = 10;
    vector_of_points m_lgr_points {point(1,0), point(2,0), point(3,0), point(4,0), point(5,0), point(6,0), point(7,0), point(8,0), point(20,0), point(50,0)};   // { point(1,0), point(2,0), point(3,0), point(4,0)};
    index_type m_segments         = 12; // > = 3  !!
    index_type m_cylinder_count   = 10;
    index_type m_mesh_count       = 4;

private:
    bool m_bcreate_outer_mesh = true;

public:
    vector_of_points mesh_points;
    vector_of_points well_points;


public:
    FlatMesh(
		value_type zw,
        value_type lw,
        value_type circle_radius,
        value_type layer_height,
        const vector_of_points &lgr_points,
        index_type segments, 
        index_type cylinder_count,
        index_type mesh_count,
        bool bcreate_outer_mesh
	)
	: m_zw(zw),
	m_lw(lw),
	m_circle_radius(circle_radius),
	m_layer_height(layer_height),
	m_lgr_points(lgr_points),
	m_segments(segments),
    m_cylinder_count(cylinder_count),
    m_mesh_count(mesh_count),
    m_bcreate_outer_mesh(bcreate_outer_mesh)
    {} 

public:
    FlatMesh(
        MeshProperties prop,
        bool is_inner
    ) :
    m_zw(prop.m_zw),
	m_lw(prop.m_lw),
	m_circle_radius(prop.m_circle_radius),
	m_layer_height(prop.m_layer_height),
	m_lgr_points(prop.m_lgr_points),
	m_segments(prop.m_segments),
    m_cylinder_count(prop.m_cylinder_count),
    m_mesh_count(prop.m_mesh_count),
    m_bcreate_outer_mesh(is_inner)
    {}


public:
    FlatMesh() = default;

    ~FlatMesh() = default;

public:
    // get inner or outer meshes
    void get_mesh();

private:
    void get_outer_mesh();

private:
    void get_inner_mesh();

private:
    vector_of_points get_sphere_approx(
        point pw,
        bool is_up
    );

private:
    polygon get_semi_circle(
        point pw, 
        value_type radius, 
        index_type point_count = 10
    );

private:
    point get_intersect(polygon pgn, segment line);

private:
    segment get_line_seg(point pw, point point_out);

private:
    vector_of_points get_inner_points(
        value_type max_rad,
        bool is_upperside_points
    );

private:
    vector_of_points get_cylinder_mesh(
        bool is_not_well_trajectory = true
    );
};

#include "detail/FlatMesh.inl"
#include "detail/FlatMeshAlgorithms.inl"