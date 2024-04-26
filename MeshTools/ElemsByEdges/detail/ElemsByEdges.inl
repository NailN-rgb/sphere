#include "../ElemsByEdges.h"


void ElemsByEdges::get_elems_by_edges()
{
    get_elems_at_first_layer();

    get_elems_at_layer();
}


void ElemsByEdges::get_elems_at_first_layer()
{
    // get elements at north pole

    // around north pole point

   
}


void ElemsByEdges::get_elems_around_north_polus()
{
    for(index_type k = 0; k < m_n_segments; k++)
    {
        auto polar_offset = m_n_cylinder + 1;
        auto side_offset  = m_n_segments * m_n_side;
        auto to_well_offset = m_n_segments * (2 * m_n_points + m_n_cylinder);

        if(k < m_n_segments - 1)
        {
            Element elem(6);

            elem.add_to_element(polar_offset - 2);
            elem.add_to_element(polar_offset + k * m_n_side);
            elem.add_to_element(polar_offset + (k + 1) * m_n_side);
            elem.add_to_element(polar_offset + side_offset + to_well_offset + k * (2 * m_n_points + m_n_cylinder));
            elem.add_to_element(polar_offset + side_offset + k * (2 * m_n_points + m_n_cylinder));
            elem.add_to_element(polar_offset + side_offset + (k + 1) * (2 * m_n_points + m_n_cylinder));

            elems_by_edges.push_back(elem);
        }
        else
        {
        
        }
    }
}


void ElemsByEdges::get_elems_at_north_semisphere()
{
    for(index_type k = 0; k < m_n_segments; k++)
    {
        auto polar_offset = m_n_cylinder + 1;
        auto side_offset  = m_n_segments * m_n_side;
        auto to_well_offset = m_n_segments * (2 * m_n_points + m_n_cylinder);

        if(k < m_n_segments - 1)
        {
            for (index_type j = 0; j < m_n_points; j++)
            {
                Element elem(8);

                elem.add_to_element(polar_offset + side_offset + to_well_offset + k * (2 * m_n_points + m_n_cylinder) + j);
                elem.add_to_element(polar_offset + side_offset + to_well_offset + k * (2 * m_n_points + m_n_cylinder) + j + 1);
                elem.add_to_element(polar_offset + j + 1 + k * m_n_side);
                elem.add_to_element(polar_offset + j + 1 + (k + 1) * m_n_side);
                
                elem.add_to_element(polar_offset + side_offset + k * (2 * m_n_points + m_n_cylinder) + j);
                elem.add_to_element(polar_offset + side_offset + k * (2 * m_n_points + m_n_cylinder) + j + 1);
                elem.add_to_element(polar_offset + side_offset + (k + 1) * (2 * m_n_points + m_n_cylinder) + j + 1);
                elem.add_to_element(polar_offset + side_offset + (k + 1) * (2 * m_n_points + m_n_cylinder) + j);

                elems_by_edges.push_back(elem);
            }
            
        }
        else
        {
        
        }
    }
}


void ElemsByEdges::get_elems_at_cylinder()
{
    for(index_type k = 0; k < m_n_segments; k++)
    {
        auto polar_offset = m_n_cylinder + 1;
        auto side_offset  = m_n_segments * m_n_side;
        auto to_well_offset = m_n_segments * (2 * m_n_points + m_n_cylinder);

        if(k < m_n_segments - 1)
        {
            for (index_type j = 0; j < m_n_cylinder; j++)
            {
                Element elem(6);

                elem.add_to_element(polar_offset + side_offset + m_n_points + j + k * (2 * m_n_points + m_n_cylinder));
                elem.add_to_element(polar_offset + side_offset + + to_well_offset + m_n_points + j + k * (2 * m_n_points + m_n_cylinder));
                elem.add_to_element(polar_offset + side_offset + m_n_points + j + (k + 1) * (2 * m_n_points + m_n_cylinder));

                elem.add_to_element(polar_offset + side_offset + m_n_points + (j + 1) + k * (2 * m_n_points + m_n_cylinder));
                elem.add_to_element(polar_offset + side_offset + + to_well_offset + m_n_points + (j + 1) + k * (2 * m_n_points + m_n_cylinder));
                elem.add_to_element(polar_offset + side_offset + m_n_points + (j + 1) + (k + 1) * (2 * m_n_points + m_n_cylinder));

                elems_by_edges.push_back(elem);
            }
            
        }
        else
        {
        
        }
    }
}


void ElemsByEdges::get_elems_at_south_semisphere()
{
    for(index_type k = 0; k < m_n_segments; k++)
    {
        auto polar_offset = m_n_cylinder + 1;
        auto side_offset  = m_n_segments * m_n_side;
        auto to_well_offset = m_n_segments * (2 * m_n_points + m_n_cylinder);


        if(k < m_n_segments - 1)
        {
            for (index_type j = 0; j < m_n_points; j++)
            {
                Element elem(8);

                elem.add_to_element(polar_offset + side_offset + to_well_offset + m_n_cylinder + k * (2 * m_n_points + m_n_cylinder) + j);
                elem.add_to_element(polar_offset + side_offset + to_well_offset + m_n_cylinder + k * (2 * m_n_points + m_n_cylinder) + j + 1);
                elem.add_to_element(polar_offset + m_n_cylinder + j + 1 + k * m_n_side);
                elem.add_to_element(polar_offset + m_n_cylinder + j + 1 + (k + 1) * m_n_side);
                
                elem.add_to_element(polar_offset + side_offset + m_n_cylinder + k * (2 * m_n_points + m_n_cylinder) + j);
                elem.add_to_element(polar_offset + side_offset + m_n_cylinder + k * (2 * m_n_points + m_n_cylinder) + j + 1);
                elem.add_to_element(polar_offset + side_offset + m_n_cylinder + (k + 1) * (2 * m_n_points + m_n_cylinder) + j + 1);
                elem.add_to_element(polar_offset + side_offset + m_n_cylinder + (k + 1) * (2 * m_n_points + m_n_cylinder) + j);

                elems_by_edges.push_back(elem);
            }
            
        }
        else
        {
        
        }
    }
}


void ElemsByEdges::get_elems_around_south_polus()
{
    for(index_type k = 0; k < m_n_segments; k++)
    {
        auto polar_offset = m_n_cylinder + 1;
        auto side_offset  = m_n_segments * m_n_side;
        auto to_well_offset = m_n_segments * (2 * m_n_points + m_n_cylinder);

        if(k < m_n_segments - 1)
        {
            Element elem(6);

            elem.add_to_element(polar_offset - 1);
            elem.add_to_element(polar_offset + (k + 1) * m_n_side - 1);
            elem.add_to_element(polar_offset + (k + 2) * m_n_side - 1);
            elem.add_to_element(polar_offset + side_offset + to_well_offset + (k + 1) * (2 * m_n_points + m_n_cylinder) - 1);
            elem.add_to_element(polar_offset + side_offset + (k + 1) * (2 * m_n_points + m_n_cylinder) - 1);
            elem.add_to_element(polar_offset + side_offset + (k + 2) * (2 * m_n_points + m_n_cylinder) - 1);

            elems_by_edges.push_back(elem);
        }
        else
        {
        
        }
    }
}





void ElemsByEdges::get_elems_at_layer()
{

}