#pragma once

#include "../../precompile.h"

#include "../../DataTypes/Edge/Edge.h"
#include "../../DataTypes/Element/Element.h"


class ElemsByEdges
{    
    typedef double value_type;
    typedef int index_type;
    
    typedef std::vector<value_type> vector_of_values;
    typedef std::vector<index_type> vector_of_indexes;

    typedef std::vector<point3d> mesh_points_vector;

    typedef std::vector<Edge> edges_list;

    typedef std::vector<Element> elements_vector;

private:
    edges_list m_edges_list;
    
    index_type m_n_side;
    index_type m_n_points;
    index_type m_n_cylinder;
    index_type m_n_segments;
    
public:
    elements_vector elems_by_edges;

public:
    ElemsByEdges(
        edges_list edgelist
    ) :
    m_edges_list(edgelist)
    {}

public:
    void get_elems_by_edges();

private:
    void get_elems_at_first_layer();

private:
    void get_elems_at_layer();

private:
    void get_elems_around_north_polus();
    
private:
    void get_elems_around_south_polus();

private:
    void get_elems_at_north_semisphere();

private:
    void get_elems_at_cylinder();

private:
    void get_elems_at_south_semisphere();
};