#pragma once


std::vector<point3d> rotate_mesh(
    const vector_of_points& mesh,
    int rotations,
    bool is_mesh_with_polar = true
)
{
    float theta = degrees_to_radians(360 / rotations);

    std::vector<point3d> semi_sphere;
    
    if(is_mesh_with_polar)
    {
        semi_sphere.push_back(
            bg::make<point3d>(
                bg::get<0>(mesh.front()),
                bg::get<0>(mesh.front()),
                bg::get<1>(mesh.front())
            )
        );

        semi_sphere.push_back(
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
            semi_sphere.push_back(
                bg::make<point3d>(
                    bg::get<0>(mesh[j]) * cosf(theta * i),  // y -> x cos(theta)
                    bg::get<0>(mesh[j]) * sinf(theta * i),  // x -> x sin(theta)
                    bg::get<1>(mesh[j])                     // z -> y
                )
            );
        }
    }

    return semi_sphere;
}
