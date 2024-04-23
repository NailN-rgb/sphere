#pragma once

/**
     * Generates a limited entry mesh of points.
     *
     * @param zw the value well center depth
     * @param lw the value well length
     * @param circle_radius the radius of formed mesh
     * @param layer_height the height of the layer
     * @param lgr_points the vector of points from 2D mesh. descending list
     * @param segments the number of mesh rotations
     * @param cylinder_count the number of points at cylinder
     * @param mesh_count the number of lgr mesh capsules
     * @param min_rad the minimum capsule radius (default: 1)
     * @param max_rad the maximum capsule radius (default: 0.3)
     *
     * @return the mesh points vector
     *
     * @throws None
 */
mesh_points_vector get_mesh_nodes(
    value_type zw,
    value_type lw,
    value_type circle_radius,
    value_type layer_height,
    const vector_of_points &lgr_points,
    index_type segments,
    index_type cylinder_count,
    index_type mesh_count,
    value_type min_rad = 0.3,
    value_type max_rad = 3
)
{
    mesh_points_vector points_mesh;

    // get well traj
    vector_of_points well_points = get_cylinder_mesh(zw, lw, cylinder_count, 0);

    std::for_each(
        well_points.begin(),
        well_points.begin() + cylinder_count,
        [&points_mesh](const point &p)
        {
            points_mesh.push_back(
                bg::make<point3d>(
                    0.0,
                    0.0,
                    bg::get<1>(p)
                )
            );
        }
    );

    value_type h_rad = (max_rad-min_rad) / mesh_count;

    // loop by lgr's
    for(size_t i = 0; i <= mesh_count; i++)
    {
        // create each lgr
        std::vector<point3d> lgr_pts = make_composite_mesh(
            zw,
            lw,
            std::exp(i * (std::log(max_rad) / mesh_count)),
            layer_height,
            lgr_points,
            segments,
            cylinder_count,
            mesh_count
        );

        points_mesh.insert(
            points_mesh.end(),
            lgr_pts.begin(),
            lgr_pts.end()
        );
    }

    return points_mesh;
}


mesh_points_vector get_inner_nodes(
    value_type zw,
    value_type lw,
    value_type circle_radius,
    value_type layer_height,
    const vector_of_points &lgr_points,
    index_type segments,
    value_type max_rad = 3
)
{
    vector_of_points inner_mesh_up = get_inner_points(zw, lw, layer_height, lgr_points, max_rad, true);
    vector_of_points inner_mesh_down = get_inner_points(zw, lw, layer_height, lgr_points, max_rad, false);

    inner_mesh_up.insert(
        inner_mesh_up.end(),
        inner_mesh_down.begin(),
        inner_mesh_down.end()
    );

    return rotate_mesh(inner_mesh_up, segments, false);
}
