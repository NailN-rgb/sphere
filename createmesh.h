#pragma once

mesh_points_vector get_mesh_nodes(
    double zw,
    double lw,
    double circle_radius,
    double layer_height,
    vector_of_points lgr_points,
    int segments,
    int cylinder_count,
    int mesh_count,
    double min_rad = 1,
    double max_rad = 0.3
)
{
    mesh_points_vector points_mesh;

    // get well traj

    vector_of_points well_points = get_cylinder_mesh(zw, lw, cylinder_count, 0);

    for(size_t i = 0; i < cylinder_count; i++)
    {
        points_mesh.push_back(
            bg::make<point3d>(
                0.0,
                0.0,
                bg::get<1>(well_points[i])
            )
        );
    }

    double h_rad = (max_rad-min_rad) / mesh_count;

    // loop by lgr's
    for(size_t i = 0; i < mesh_count; i++)
    {
        // create each lgr
        std::vector<point3d> lgr_pts = make_composite_mesh(
            zw,
            lw,
            max_rad - i * h_rad,
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
