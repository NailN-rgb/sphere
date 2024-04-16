#pragma once


points_vector get_cylinder_mesh(double zw, double lw, int points_count, double radius)
{
    // regular mesh

    double h = lw / (points_count - 1);

    points_vector cylinder_mesh;

    for(size_t i = 0; i < points_count; i++)
    {
        cylinder_mesh.push_back(
            bg::make<point>(
                radius, 
                zw - lw/2 + i * h
            )
        );
    }

    return cylinder_mesh;
}


std::vector<point3d> make_composite_mesh(
    double zw,
    double lw,
    double circle_radius,
    double layer_height,
    vector_of_points lgr_points,
    int segments,
    int cylinder_count,
    int mesh_count
)
{
    // first create semi circle by params

    // calculate pw

    point top_circle_center    = point(0, zw - lw/2);
    point bottom_circle_center = point(0, layer_height - zw - lw/2);

    vector_of_points top_circle   = get_sphere_approx(lgr_points, top_circle_center, lw, circle_radius, true);
    vector_of_points bottom_circle = get_sphere_approx(lgr_points, bottom_circle_center, lw, circle_radius, false);

    vector_of_points cylinder_mesh = get_cylinder_mesh(zw, lw, cylinder_count, circle_radius);

    vector_of_points capsule_mesh;
    capsule_mesh.reserve(top_circle.size() + bottom_circle.size() + cylinder_mesh.size());
    capsule_mesh.insert(capsule_mesh.end(), top_circle.begin(), top_circle.end());
    capsule_mesh.insert(capsule_mesh.end(), cylinder_mesh.begin(), cylinder_mesh.end());

    std::reverse(bottom_circle.begin(), bottom_circle.end());

    capsule_mesh.insert(capsule_mesh.end(), bottom_circle.begin(), bottom_circle.end());

    return rotate_mesh(capsule_mesh, segments);
}