#pragma once


std::vector<std::vector<double>> create_prizmoidal_mesh_around_well(
    std::vector<point3d> mesh,
    int n_cyl,
    int n_points,
    int n_segments,
    int n_side
)
{
    std::vector<std::vector<double>> prizmoidal_elems;

    // loop by capsule side
    for(size_t k = 0; k < n_segments; k++)
    {
        if(k != n_segments - 1)
        {
            // loop by each element at cylindrical side
            for(size_t idx = 0; idx < n_cyl - 1; idx++)
            {
                // there i - relative number of first cylindral point

                std::vector<double> elem;
                
                // point at selected capsile side
                elem.push_back(2 + n_cyl + n_points + idx + k * n_side);

                // point at next segment side
                elem.push_back(2 + n_cyl + n_points + n_side + idx + k * n_side );

                // well segment
                elem.push_back(idx);

                // downer point at selected capsile side
                elem.push_back(2 + n_cyl + n_points + 1 + idx + k * n_side);

                // downer point at next segment side
                elem.push_back(2 + n_cyl + n_points + n_side + 1 + idx + k * n_side);

                // downer well segment
                elem.push_back(idx + 1);

                prizmoidal_elems.push_back(elem);
            }
        }
        else
        {
            for(size_t idx = 0; idx < n_cyl - 1; idx++)
            {
                // there i - relative number of first cylindral point

                std::vector<double> elem;
                
                // point at selected capsile side
                elem.push_back(2 + n_cyl + n_points + idx + k * n_side);

                // point at next segment side
                elem.push_back(2 + n_cyl + n_points + idx);

                // well segment
                elem.push_back(idx);

                // downer point at selected capsile side
                elem.push_back(2 + n_cyl + n_points + 1 + idx + k * n_side);

                // downer point at next segment side
                elem.push_back(2 + n_cyl + n_points + 1 + idx);

                // downer well segment
                elem.push_back(idx + 1);

                prizmoidal_elems.push_back(elem);
            }
        }
    }

    return prizmoidal_elems;
}


std::vector<std::vector<double>> create_polar_mesh_around_well_segment(
    std::vector<point3d> mesh,
    int n_cyl,
    int n_points,
    int n_segments,
    int n_side
)
{
    std::vector<std::vector<double>> polar_mesh;

    /*
    ******

    Mesh at north

    ******
    
    */

    // at polar
    for(int k = 0; k < n_segments; k++)
    {
        std::vector<double> elem;

        if(k < n_segments - 1)
        {
            elem.push_back(n_cyl);

            elem.push_back(n_cyl + 2 + k * n_side);

            elem.push_back(n_cyl + 2 + (k + 1) * n_side);

            elem.push_back(0); 
        }
        else
        {
            elem.push_back(n_cyl);

            elem.push_back(n_cyl + 2 + k * n_side);

            elem.push_back(n_cyl + 2);

            elem.push_back(0); 
        }

        polar_mesh.push_back(elem);
    }

    // to cylynder
    for(int k = 0; k < n_segments; k++)
    {
        if(k < n_segments - 1)
        {
            for(int idx = 0; idx < n_points; idx++)
            {
                std::vector<double> elem;
                // right up point
                elem.push_back(n_cyl + 2 + idx + k * n_side);
                // right down point
                elem.push_back(n_cyl + 3 + idx + k * n_side);
                // left down point
                elem.push_back(n_cyl + 3 + idx + (k + 1) * n_side);
                // left up point
                elem.push_back(n_cyl + 2 + idx + (k + 1) * n_side);
                // well head
                elem.push_back(0);

                polar_mesh.push_back(elem);
            }
        }
        else
        {
            for (int idx = 0; idx < n_points; idx++)
            {
                std::vector<double> elem;
                // right up point
                elem.push_back(n_cyl + 2 + idx + k * n_side);
                // right down point
                elem.push_back(n_cyl + 3 + idx + k * n_side);
                // left down point
                elem.push_back(n_cyl + 3 + idx);
                // left up point
                elem.push_back(n_cyl + 2 + idx);
                // well head
                elem.push_back(0);

                polar_mesh.push_back(elem);
            }
            
        }

    }


    /*
    ******

    Mesh at south

    ******
    
    */

    // at polar
    for(int k = 0; k < n_segments; k++)
    {
        std::vector<double> elem;

        if(k < n_segments - 1)
        {
            // polar point
            elem.push_back(n_cyl + 1);

            elem.push_back(2 * n_cyl + 1 + 2 * n_points + k * n_side);

            elem.push_back(2 * n_cyl + 1 + 2 * n_points + (k + 1) * n_side);
            // well point
            elem.push_back(n_cyl - 1); 
        }
        else
        {
            // polar point
            elem.push_back(n_cyl + 1);

            elem.push_back(2 * n_cyl + 1 + 2 * n_points + k * n_side);

            elem.push_back(2 * n_cyl + 1 + 2 * n_points);
            // well point
            elem.push_back(n_cyl - 1); 
        }

        polar_mesh.push_back(elem);
    }

    // to cylynder
    for(int k = 0; k < n_segments; k++)
    {
        if(k < n_segments - 1)
        {
            for(int idx = 0; idx < n_points; idx++)
            {
                std::vector<double> elem;
                // right up point
                elem.push_back(2 * n_cyl + 2 + n_points + idx + k * n_side);
                // right down point
                elem.push_back(2 * n_cyl + 1 + n_points + idx + k * n_side);
                // left down point
                elem.push_back(2 * n_cyl + 1 + n_points + idx + (k + 1) * n_side);
                // left up point
                elem.push_back(2 * n_cyl + 2 + n_points + idx + (k + 1) * n_side);
                // well head
                elem.push_back(n_cyl - 1);

                polar_mesh.push_back(elem);
            }
        }
        else
        {
            for (int idx = 0; idx < n_points; idx++)
            {
                std::vector<double> elem;
                // right up point
                // right up point
                elem.push_back(2 * n_cyl + 2 + n_points + idx + k * n_side);
                // right down point
                elem.push_back(2 * n_cyl + 1 + n_points + idx + k * n_side);
                // left down point
                elem.push_back(2 * n_cyl + 1 + n_points + idx);
                // left up point
                elem.push_back(2 * n_cyl + 2 + n_points + idx);
                // well head
                elem.push_back(n_cyl - 1);

                polar_mesh.push_back(elem);
            }
            
        }

    }


    return polar_mesh;
}


std::vector<std::vector<double>> last_lgr_connection_with_well(
    std::vector<point3d> mesh,
    int n_cyl,
    int n_points,
    int n_segments
)
{
    std::vector<std::vector<double>> resulted_elems;

    auto n_side = n_cyl + 2 * n_points; // number of non polar nodes at mesh side

    std::vector<std::vector<double>> polar_elems = create_polar_mesh_around_well_segment(mesh, n_cyl, n_points, n_segments, n_side);

    resulted_elems = create_prizmoidal_mesh_around_well(mesh, n_cyl, n_points, n_segments, n_side);

    resulted_elems.insert(resulted_elems.end(), polar_elems.begin(), polar_elems.end());


    // first connect cylinder part
    return resulted_elems;

}


elements_vector create_between_layer_polar_connection(
    index_type n_cyl,
    index_type n_points,
    index_type n_segments,
    index_type n_side,
    index_type n_layer,
    index_type current_lgr_idx
)
{
    elements_vector polar_elems;

    /*
        NORTH POLUS
    */
    for(int k = 0; k < n_segments; k++)
    {
        if(k < n_segments - 1)
        {
            std::vector<double> elem;

            auto segment_offset = k * n_side;

            // elem part at previous layer
            elem.push_back(n_cyl + current_lgr_idx * n_layer);
            elem.push_back(n_cyl + 2  + current_lgr_idx * n_layer + segment_offset);
            elem.push_back(n_cyl + 2 + n_side  + current_lgr_idx * n_layer + segment_offset);

            // elem part at next layer
            elem.push_back(n_cyl + (current_lgr_idx + 1) * n_layer);
            elem.push_back(n_cyl + 2  + (current_lgr_idx + 1) * n_layer + segment_offset);
            elem.push_back(n_cyl + 2 + n_side  + (current_lgr_idx + 1) * n_layer + segment_offset);

            polar_elems.push_back(elem);
        }
        else
        {
            std::vector<double> elem;

            auto segment_offset = k * n_side;
            
            // elem part at previous layer
            elem.push_back(n_cyl + current_lgr_idx * n_layer);
            elem.push_back(n_cyl + 2  + current_lgr_idx * n_layer + segment_offset);
            elem.push_back(n_cyl + 2 + n_side  + current_lgr_idx * n_layer + segment_offset - (k + 1) * n_side);

            // elem part at next layer
            elem.push_back(n_cyl + (current_lgr_idx + 1) * n_layer);
            elem.push_back(n_cyl + 2  + (current_lgr_idx + 1) * n_layer + segment_offset);
            elem.push_back(n_cyl + 2 + n_side  + (current_lgr_idx + 1) * n_layer + segment_offset - (k + 1) * n_side);

            polar_elems.push_back(elem);
        }
    }

    /*
        SOUTH POLUS
    */

   for(int k = 0; k < n_segments; k++)
    {
        if(k < n_segments - 1)
        {
            std::vector<double> elem;

            auto segment_offset = k * n_side;
            auto polar_offset   = n_side - 1;

            // elem part at previous layer
            elem.push_back(n_cyl + 1 + current_lgr_idx * n_layer);
            elem.push_back(n_cyl + 2  + current_lgr_idx * n_layer + segment_offset + polar_offset);
            elem.push_back(n_cyl + 2 + n_side  + current_lgr_idx * n_layer + segment_offset + polar_offset);

            // elem part at next layer
            elem.push_back(n_cyl + 1 + (current_lgr_idx + 1) * n_layer);
            elem.push_back(n_cyl + 2  + (current_lgr_idx + 1) * n_layer + segment_offset + polar_offset);
            elem.push_back(n_cyl + 2 + n_side  + (current_lgr_idx + 1) * n_layer + segment_offset + polar_offset);

            polar_elems.push_back(elem);
        }
        else
        {
            std::vector<double> elem;

            auto segment_offset = k * n_side;
            auto polar_offset   = n_side - 1;

            // elem part at previous layer
            elem.push_back(n_cyl + 1 + current_lgr_idx * n_layer);
            elem.push_back(n_cyl + 2  + current_lgr_idx * n_layer + segment_offset + polar_offset);
            elem.push_back(n_cyl + 2 + n_side  + current_lgr_idx * n_layer + segment_offset + polar_offset - (k + 1) * n_side );

            // elem part at next layer
            elem.push_back(n_cyl + 1 + (current_lgr_idx + 1) * n_layer);
            elem.push_back(n_cyl + 2  + (current_lgr_idx + 1) * n_layer + segment_offset + polar_offset);
            elem.push_back(n_cyl + 2 + n_side  + (current_lgr_idx + 1) * n_layer + segment_offset + polar_offset - (k + 1) * n_side);

            polar_elems.push_back(elem);
        }
    }

    return polar_elems;
}


elements_vector create_between_layer_side_connection(
    index_type n_cyl,
    index_type n_points,
    index_type n_segments,
    index_type n_side,
    index_type n_layer,
    index_type current_lgr_idx
)
{
    elements_vector side_elements;

    for(int k = 0; k < n_segments; k++)
    {
        if(k < n_segments - 1)
        {
            for(int i = 0; i < 2 * n_points + n_cyl - 1; i++)
            {
                std::vector<double> elem;

                index_type start_offset = 2 + n_cyl;                // for first non-aroundwell capsule
                index_type segment_offset = i;                          // offset at fixed segmnts part on capsule
                index_type lgr_offset = k * n_side;                 // offset between segments
                index_type capsular_ofset = current_lgr_idx * n_layer;  // offset between two capsules

                // segment at previous lgr layer
                elem.push_back(start_offset + segment_offset + capsular_ofset + lgr_offset);
                elem.push_back(start_offset + 1 + segment_offset + capsular_ofset + lgr_offset);
                elem.push_back(start_offset + 1 + segment_offset + capsular_ofset + lgr_offset + n_side);
                elem.push_back(start_offset + segment_offset + capsular_ofset + lgr_offset + n_side);
                

                //segment at current layer
                elem.push_back(start_offset + segment_offset + capsular_ofset + lgr_offset + n_layer);
                elem.push_back(start_offset + 1 + segment_offset + capsular_ofset + lgr_offset + n_layer);
                elem.push_back(start_offset + 1 + segment_offset + capsular_ofset + lgr_offset + n_side + n_layer);
                elem.push_back(start_offset + segment_offset + capsular_ofset + lgr_offset + n_side + n_layer);

                side_elements.push_back(elem);
            }
        }
        else
        {
            for(int i = 0; i < 2 * n_points + n_cyl - 1; i++)
            {
                std::vector<double> elem;

                index_type start_offset = 2 + n_cyl;                // for first non-aroundwell capsule
                index_type segment_offset = i;                          // offset at fixed segmnts part on capsule
                index_type lgr_offset = k * n_side;                 // offset between segments
                index_type capsular_ofset = current_lgr_idx * n_layer;  // offset between two capsules

                // segment at previous lgr layer
                elem.push_back(start_offset + segment_offset + capsular_ofset + lgr_offset );
                elem.push_back(start_offset + 1 + segment_offset + capsular_ofset + lgr_offset);
                elem.push_back(start_offset + 1 + segment_offset + capsular_ofset + lgr_offset + n_side - (k + 1) * n_side);
                elem.push_back(start_offset + segment_offset + capsular_ofset + lgr_offset + n_side - (k + 1) * n_side);
                

                //segment at current layer
                elem.push_back(start_offset + segment_offset + capsular_ofset + lgr_offset + n_layer);
                elem.push_back(start_offset + 1 + segment_offset + capsular_ofset + lgr_offset + n_layer);
                elem.push_back(start_offset + 1 + segment_offset + capsular_ofset + lgr_offset + n_side + n_layer - (k + 1) * n_side);
                elem.push_back(start_offset + segment_offset + capsular_ofset + lgr_offset + n_side + n_layer - (k + 1) * n_side);

                side_elements.push_back(elem);
            }
        }
    }

    return side_elements;
}


elements_vector connect_with_pervious_lgr(
    std::vector<point3d> mesh,
    int n_cyl,
    int n_points,
    int n_segments,
    index_type current_lgr_idx
)
{
    // number of non polar nodes at mesh side
    // used at adressing indexes between layer's segments
    auto n_side = n_cyl + 2 * n_points; 
    
    // number of nodes at mesh layer
    // used at adressing indexes to pervious layer nodes
    auto n_layer = 2 + n_segments * (2 * n_points + n_cyl); 

    elements_vector layer_mesh;

    elements_vector polar_elems = create_between_layer_polar_connection(n_cyl, n_points, n_segments, n_side, n_layer, current_lgr_idx);

    elements_vector side_elems = create_between_layer_side_connection(n_cyl, n_points, n_segments, n_side, n_layer, current_lgr_idx);

    layer_mesh.insert(layer_mesh.end(), polar_elems.begin(), polar_elems.end());
    layer_mesh.insert(layer_mesh.end(), side_elems.begin(), side_elems.end());

    return layer_mesh;
}


std::vector<std::vector<double>> connect_elems(
    std::vector<point3d> mesh,
    int lgr_count,
    int n_cyl,
    int n_points,
    int n_segments
)
{
    std::vector<std::vector<double>> total_elems; // we need special data type?

    // loop by lgr meshs
    for(size_t i = 0; i <= lgr_count; i++)
    {
        std::vector<std::vector<double>> elems;
        // if i - layer not last layer
        if(i == 0)
        {
            // if i last layer -> make_connection with well
            elems = last_lgr_connection_with_well(mesh, n_cyl, n_points, n_segments);
        }
        else if(i > 1) // TODO: rewrite formulas to i indx
        {
            elems = connect_with_pervious_lgr(mesh, n_cyl, n_points, n_segments, i - 1);
        }
        else //if(i == 1)
        {
            elems = connect_with_pervious_lgr(mesh, n_cyl, n_points, n_segments, i - 1);
        }
        

        total_elems.insert(total_elems.end(), elems.begin(), elems.end());
    }

    return total_elems;
}