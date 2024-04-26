#pragma once

#include "../../precompile.h"

class Element
{
private:
    index_type m_element_size;

private:
    vector_of_indexes m_points;

public:
    Element(
        index_type element_size
    ) :
    m_element_size(element_size)
    {}

public:
    void add_to_element(index_type index)
    {
        if(m_points.size() >= m_element_size)
        {
            throw std::range_error("Element is full");
        }
        else
        {
            m_points.push_back(index);
        }
    };

public:
    vector_of_indexes get_element()
    {
        if(m_points.size() > 0)
        {
            return m_points;
        }
        else
        {
            throw std::range_error("Element is empty");
        }
    }

public:
    index_type get_element_size()
    {
        return m_element_size;
    }
};