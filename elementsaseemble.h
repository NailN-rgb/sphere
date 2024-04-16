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


std::vector<std::vector<double>> connect_elems(
    std::vector<point3d> mesh,
    int lgr_count,
    int n_cyl,
    int n_points,
    int n_segments
)
{
    std::vector<std::vector<double>> elems; // we need special data type?

    // loop by lgr meshs
    for(size_t i = 0; i < lgr_count; i++)
    {
        // if i - layer not last layer


        // if i last layer -> make_connection with well
        // cut unused mwsh points

        elems = last_lgr_connection_with_well(mesh, n_cyl, n_points, n_segments);
    }

    return elems;
}