#pragma once 

#include "../../precompile.h"


class Edge
{
private:
    index_type m_first;
    index_type m_second;

public:
    Edge(
        index_type first, 
        index_type second
    ): m_first(first), m_second(second)
    {}

    Edge()
    {
        
    };

public:
    void set_first(index_type first);

public:
    void set_second(index_type second);

public:
    index_type get_first() const;

public:
    index_type get_second() const;

public:
    vector_of_indexes get_edge() const;
};



void Edge::set_first(index_type first)
{
    m_first = first;
}



void Edge::set_second(index_type second)
{
    m_second = second;
}


index_type Edge::get_first() const
{
    return m_first;
}


index_type Edge::get_second() const
{
    return m_second;
}


vector_of_indexes Edge::get_edge() const
{
    return vector_of_indexes({m_first, m_second});
};
