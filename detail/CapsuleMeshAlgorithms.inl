#pragma once

point get_intersection_of_line_segments(segment l1, segment l2)
{
    if(bg::intersects(l1, l2))
    {
        vector_of_points intersection_points;

        bg::intersection(l1, l2, intersection_points);

        if(intersection_points.size() > 1)
        {
            throw std::runtime_error("More than 1 intrersection point at segment");
        }

        return intersection_points.front();
    }
}


value_type degrees_to_radians(value_type deg)
{
    return deg * 4. * std::atan(1.) / 180;
}


void CapsuleMesh::add_well_trajectory_to_mesh()
{
    mesh_points_vector well_points;

    std::for_each(
        m_well_mesh.begin(),
        m_well_mesh.end(),
        [this, &well_points](point p)
        {
            well_points.push_back(
                bg::make<point3d>(
                    0.0,
                    0.0, 
                    bg::get<1>(p)
                )
            );
        }
    );

    capsule_mesh.insert(
        capsule_mesh.end(),
        well_points.begin(),
        well_points.end()
    );
}


mesh_points_vector CapsuleMesh::get_capsular_mesh_at_layer(
    value_type circle_radius
)
{
    FlatMesh mesh = m_flat_mesh;

    mesh.get_mesh();

    return rotate_mesh(mesh.mesh_points, m_segments_count, true);
}


mesh_points_vector CapsuleMesh::rotate_mesh(
    const vector_of_points& mesh,
    int rotations,
    bool is_mesh_with_polar
)
{
    value_type theta = degrees_to_radians(360 / rotations);

    mesh_points_vector capsule;
    
    if(is_mesh_with_polar)
    {
        capsule.push_back(
            bg::make<point3d>(
                bg::get<0>(mesh.front()),
                bg::get<0>(mesh.front()),
                bg::get<1>(mesh.front())
            )
        );

        capsule.push_back(
            bg::make<point3d>(
                bg::get<0>(mesh.back()),
                bg::get<0>(mesh.back()),
                bg::get<1>(mesh.back())
            )
        );
    }

    for(size_t i = 0; i < rotations; i++)
    {
        for(size_t j = 1; j < mesh.size() - 1; j++)
        {
            capsule.push_back(
                bg::make<point3d>(
                    bg::get<0>(mesh[j]) * cosf(theta * i),  // y -> x cos(theta)
                    bg::get<0>(mesh[j]) * sinf(theta * i),  // x -> x sin(theta)
                    bg::get<1>(mesh[j])                     // z -> y
                )
            );
        }
    }

    return capsule;
}



elements_vector CapsuleMesh::create_prizmoidal_mesh_around_well(
    index_type n_side
)
{
    elements_vector prizmoidal_elems;

    // loop by capsule side
    for(index_type k = 0; k < m_segments_count; k++)
    {
        if(k != m_segments_count - 1)
        {
            // loop by each element at cylindrical side
            for(index_type idx = 0; idx < m_cylinder_size - 1; idx++)
            {
                // there i - relative number of first cylindral point

                vector_of_values elem;
                
                // point at selected capsile side
                elem.push_back(2 + m_cylinder_size + m_points_size + idx + k * n_side);

                // point at next segment side
                elem.push_back(2 + m_cylinder_size + m_points_size + n_side + idx + k * n_side );

                // well segment
                elem.push_back(idx);

                // downer point at selected capsile side
                elem.push_back(2 + m_cylinder_size + m_points_size + 1 + idx + k * n_side);

                // downer point at next segment side
                elem.push_back(2 + m_cylinder_size + m_points_size + n_side + 1 + idx + k * n_side);

                // downer well segment
                elem.push_back(idx + 1);

                prizmoidal_elems.push_back(elem);
            }
        }
        else
        {
            for(index_type idx = 0; idx < m_cylinder_size - 1; idx++)
            {
                // there i - relative number of first cylindral point

                vector_of_values elem;
                
                // point at selected capsile side
                elem.push_back(2 + m_cylinder_size + m_points_size + idx + k * n_side);

                // point at next segment side
                elem.push_back(2 + m_cylinder_size + m_points_size + idx);

                // well segment
                elem.push_back(idx);

                // downer point at selected capsile side
                elem.push_back(2 + m_cylinder_size + m_points_size + 1 + idx + k * n_side);

                // downer point at next segment side
                elem.push_back(2 + m_cylinder_size + m_points_size + 1 + idx);

                // downer well segment
                elem.push_back(idx + 1);

                prizmoidal_elems.push_back(elem);
            }
        }
    }

    return prizmoidal_elems;
}


elements_vector CapsuleMesh::create_polar_mesh_around_well_segment(
    index_type n_side
)
{
    elements_vector polar_mesh;

    /*
    ******

    Mesh at north

    ******
    
    */

    // at polar
    for(int k = 0; k < m_segments_count; k++)
    {
        vector_of_values elem;

        if(k < m_segments_count - 1)
        {
            elem.push_back(m_cylinder_size);

            elem.push_back(m_cylinder_size + 2 + k * n_side);

            elem.push_back(m_cylinder_size + 2 + (k + 1) * n_side);

            elem.push_back(0); 
        }
        else
        {
            elem.push_back(m_cylinder_size);

            elem.push_back(m_cylinder_size + 2 + k * n_side);

            elem.push_back(m_cylinder_size + 2);

            elem.push_back(0); 
        }

        polar_mesh.push_back(elem);
    }

    // to cylynder
    for(int k = 0; k < m_segments_count; k++)
    {
        if(k < m_segments_count - 1)
        {
            for(int idx = 0; idx < m_points_size; idx++)
            {
                vector_of_values elem;
                // right up point
                elem.push_back(m_cylinder_size + 2 + idx + k * n_side);
                // right down point
                elem.push_back(m_cylinder_size + 3 + idx + k * n_side);
                // left down point
                elem.push_back(m_cylinder_size + 3 + idx + (k + 1) * n_side);
                // left up point
                elem.push_back(m_cylinder_size + 2 + idx + (k + 1) * n_side);
                // well head
                elem.push_back(0);

                polar_mesh.push_back(elem);
            }
        }
        else
        {
            for (int idx = 0; idx < m_points_size; idx++)
            {
                vector_of_values elem;
                // right up point
                elem.push_back(m_cylinder_size + 2 + idx + k * n_side);
                // right down point
                elem.push_back(m_cylinder_size + 3 + idx + k * n_side);
                // left down point
                elem.push_back(m_cylinder_size + 3 + idx);
                // left up point
                elem.push_back(m_cylinder_size + 2 + idx);
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
    for(int k = 0; k < m_segments_count; k++)
    {
        vector_of_values elem;

        if(k < m_segments_count - 1)
        {
            // polar point
            elem.push_back(m_cylinder_size + 1);

            elem.push_back(2 * m_cylinder_size + 1 + 2 * m_points_size + k * n_side);

            elem.push_back(2 * m_cylinder_size + 1 + 2 * m_points_size + (k + 1) * n_side);
            // well point
            elem.push_back(m_cylinder_size - 1); 
        }
        else
        {
            // polar point
            elem.push_back(m_cylinder_size + 1);

            elem.push_back(2 * m_cylinder_size + 1 + 2 * m_points_size + k * n_side);

            elem.push_back(2 * m_cylinder_size + 1 + 2 * m_points_size);
            // well point
            elem.push_back(m_cylinder_size - 1); 
        }

        polar_mesh.push_back(elem);
    }

    // to cylynder
    for(int k = 0; k < m_segments_count; k++)
    {
        if(k < m_segments_count - 1)
        {
            for(int idx = 0; idx < m_points_size; idx++)
            {
                vector_of_values elem;
                // right up point
                elem.push_back(2 * m_cylinder_size + 2 + m_points_size + idx + k * n_side);
                // right down point
                elem.push_back(2 * m_cylinder_size + 1 + m_points_size + idx + k * n_side);
                // left down point
                elem.push_back(2 * m_cylinder_size + 1 + m_points_size + idx + (k + 1) * n_side);
                // left up point
                elem.push_back(2 * m_cylinder_size + 2 + m_points_size + idx + (k + 1) * n_side);
                // well head
                elem.push_back(m_cylinder_size - 1);

                polar_mesh.push_back(elem);
            }
        }
        else
        {
            for (int idx = 0; idx < m_points_size; idx++)
            {
                vector_of_values elem;
                // right up point
                // right up point
                elem.push_back(2 * m_cylinder_size + 2 + m_points_size + idx + k * n_side);
                // right down point
                elem.push_back(2 * m_cylinder_size + 1 + m_points_size + idx + k * n_side);
                // left down point
                elem.push_back(2 * m_cylinder_size + 1 + m_points_size + idx);
                // left up point
                elem.push_back(2 * m_cylinder_size + 2 + m_points_size + idx);
                // well head
                elem.push_back(m_cylinder_size - 1);

                polar_mesh.push_back(elem);
            }
            
        }

    }


    return polar_mesh;
}


elements_vector CapsuleMesh::last_lgr_connection_with_well()
{
    elements_vector resulted_elems;

    auto n_side = m_cylinder_size + 2 * m_points_size; // number of non polar nodes at mesh side

    elements_vector polar_elems = create_polar_mesh_around_well_segment(n_side);

    resulted_elems = create_prizmoidal_mesh_around_well(n_side);

    resulted_elems.insert(resulted_elems.end(), polar_elems.begin(), polar_elems.end());

    // first connect cylinder part
    return resulted_elems;

}


elements_vector CapsuleMesh::create_between_layer_polar_connection(
    index_type n_side,
    index_type n_layer,
    index_type current_lgr_idx
)
{
    elements_vector polar_elems;

    /*
        NORTH POLUS
    */
    for(int k = 0; k < m_segments_count; k++)
    {
        if(k < m_segments_count - 1)
        {
            vector_of_values elem;

            auto segment_offset = k * n_side;

            // elem part at previous layer
            elem.push_back(m_cylinder_size + current_lgr_idx * n_layer);
            elem.push_back(m_cylinder_size + 2  + current_lgr_idx * n_layer + segment_offset);
            elem.push_back(m_cylinder_size + 2 + n_side  + current_lgr_idx * n_layer + segment_offset);

            // elem part at next layer
            elem.push_back(m_cylinder_size + (current_lgr_idx + 1) * n_layer);
            elem.push_back(m_cylinder_size + 2  + (current_lgr_idx + 1) * n_layer + segment_offset);
            elem.push_back(m_cylinder_size + 2 + n_side  + (current_lgr_idx + 1) * n_layer + segment_offset);

            polar_elems.push_back(elem);
        }
        else
        {
            vector_of_values elem;

            auto segment_offset = k * n_side;
            
            // elem part at previous layer
            elem.push_back(m_cylinder_size + current_lgr_idx * n_layer);
            elem.push_back(m_cylinder_size + 2  + current_lgr_idx * n_layer + segment_offset);
            elem.push_back(m_cylinder_size + 2 + n_side  + current_lgr_idx * n_layer + segment_offset - (k + 1) * n_side);

            // elem part at next layer
            elem.push_back(m_cylinder_size + (current_lgr_idx + 1) * n_layer);
            elem.push_back(m_cylinder_size + 2  + (current_lgr_idx + 1) * n_layer + segment_offset);
            elem.push_back(m_cylinder_size + 2 + n_side  + (current_lgr_idx + 1) * n_layer + segment_offset - (k + 1) * n_side);

            polar_elems.push_back(elem);
        }
    }

    /*
        SOUTH POLUS
    */

   for(int k = 0; k < m_segments_count; k++)
    {
        if(k < m_segments_count - 1)
        {
            vector_of_values elem;

            auto segment_offset = k * n_side;
            auto polar_offset   = n_side - 1;

            // elem part at previous layer
            elem.push_back(m_cylinder_size + 1 + current_lgr_idx * n_layer);
            elem.push_back(m_cylinder_size + 2  + current_lgr_idx * n_layer + segment_offset + polar_offset);
            elem.push_back(m_cylinder_size + 2 + n_side  + current_lgr_idx * n_layer + segment_offset + polar_offset);

            // elem part at next layer
            elem.push_back(m_cylinder_size + 1 + (current_lgr_idx + 1) * n_layer);
            elem.push_back(m_cylinder_size + 2  + (current_lgr_idx + 1) * n_layer + segment_offset + polar_offset);
            elem.push_back(m_cylinder_size + 2 + n_side  + (current_lgr_idx + 1) * n_layer + segment_offset + polar_offset);

            polar_elems.push_back(elem);
        }
        else
        {
            vector_of_values elem;

            auto segment_offset = k * n_side;
            auto polar_offset   = n_side - 1;

            // elem part at previous layer
            elem.push_back(m_cylinder_size + 1 + current_lgr_idx * n_layer);
            elem.push_back(m_cylinder_size + 2  + current_lgr_idx * n_layer + segment_offset + polar_offset);
            elem.push_back(m_cylinder_size + 2 + n_side  + current_lgr_idx * n_layer + segment_offset + polar_offset - (k + 1) * n_side );

            // elem part at next layer
            elem.push_back(m_cylinder_size + 1 + (current_lgr_idx + 1) * n_layer);
            elem.push_back(m_cylinder_size + 2  + (current_lgr_idx + 1) * n_layer + segment_offset + polar_offset);
            elem.push_back(m_cylinder_size + 2 + n_side  + (current_lgr_idx + 1) * n_layer + segment_offset + polar_offset - (k + 1) * n_side);

            polar_elems.push_back(elem);
        }
    }

    return polar_elems;
}


elements_vector CapsuleMesh::create_between_layer_side_connection(
    index_type n_side,
    index_type n_layer,
    index_type current_lgr_idx
)
{
    elements_vector side_elements;

    for(int k = 0; k < m_segments_count; k++)
    {
        if(k < m_segments_count - 1)
        {
            for(int i = 0; i < 2 * m_points_size + m_cylinder_size - 1; i++)
            {
                vector_of_values elem;

                index_type start_offset = 2 + m_cylinder_size;                // for first non-aroundwell capsule
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
            for(int i = 0; i < 2 * m_points_size + m_cylinder_size - 1; i++)
            {
                vector_of_values elem;

                index_type start_offset = 2 + m_cylinder_size;                // for first non-aroundwell capsule
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


elements_vector CapsuleMesh::connect_with_pervious_lgr(
    index_type current_lgr_idx
)
{
    // number of non polar nodes at mesh side
    // used at adressing indexes between layer's segments
    auto n_side = m_cylinder_size + 2 * m_points_size; 
    
    // number of nodes at mesh layer
    // used at adressing indexes to pervious layer nodes
    auto n_layer = 2 + m_segments_count * (2 * m_points_size + m_cylinder_size); 

    elements_vector layer_mesh;

    elements_vector polar_elems = create_between_layer_polar_connection(n_side, n_layer, current_lgr_idx);

    elements_vector side_elems = create_between_layer_side_connection(n_side, n_layer, current_lgr_idx);

    layer_mesh.insert(layer_mesh.end(), polar_elems.begin(), polar_elems.end());
    layer_mesh.insert(layer_mesh.end(), side_elems.begin(), side_elems.end());

    return layer_mesh;
}


elements_vector CapsuleMesh::connect_elems()
{
    // loop by lgr meshs
    for(index_type i = 0; i < m_capsule_count; i++)
    {
        elements_vector elems;
        // if i - layer not last layer
        if(i == 0)
        {
            // if i last layer -> make_connection with well
            elems = last_lgr_connection_with_well();
        }
        else if(i > 1) // TODO: rewrite formulas to i indx
        {
            elems = connect_with_pervious_lgr(i - 1);
        }
        else //if(i == 1)
        {
            elems = connect_with_pervious_lgr(i - 1);
        }
        

        mesh_elements.insert(mesh_elements.end(), elems.begin(), elems.end());
    }

    return mesh_elements;
}


mesh_points_vector CapsuleMesh::get_inner_nodes()
{
    vector_of_points inner_mesh_up = get_inner_points(true);
    vector_of_points inner_mesh_down = get_inner_points(false);

    inner_mesh_up.insert(
        inner_mesh_up.end(),
        inner_mesh_down.begin(),
        inner_mesh_down.end()
    );

    return rotate_mesh(inner_mesh_up, false);
}


vector_of_points CapsuleMesh::get_inner_points(
    bool is_upperside_points
)
{
    vector_of_points inner_mesh;

    inner_mesh.push_back(bg::make<point>(0.0, 0.0));
    inner_mesh.push_back(bg::make<point>(0.0, m_layer_height));

    auto cylinder_side = segment(
                                bg::make<point>(m_max_radius, 0.0),
                                bg::make<point>(m_max_radius, m_layer_height));

    std::for_each(
        m_lgr_points.begin(),
        m_lgr_points.end(),
        [this, &inner_mesh, &cylinder_side, &is_upperside_points](point p)
        {
            // if point lay at cylinder 
            if(bg::get<0>(p) <  m_max_radius)
            {
                if(is_upperside_points)
                {
                    inner_mesh.push_back(p);
                }
                else
                {
                    inner_mesh.push_back(bg::make<point>(
                        bg::get<0>(p), bg::get<1>(p) + m_layer_height));
                }
            }
            // if point do not lay at cylinder
            else
            {
                auto pw = point(0, 0);

                if(is_upperside_points)
                {
                    pw = bg::make<point>(0.0, m_zw - m_lw/2);
                }
                else
                {
                    p = bg::make<point>(bg::get<0>(p), bg::get<1>(p) + m_layer_height);
                    pw = bg::make<point>(0.0, m_zw + m_lw/2);
                }

                auto point_to_pw_line = segment(pw, p);

                inner_mesh.push_back(get_intersection_of_line_segments(cylinder_side, point_to_pw_line));
            }
        }
    );
    
    return inner_mesh;
}