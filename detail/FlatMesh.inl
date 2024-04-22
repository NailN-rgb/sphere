#pragma once


void FlatMesh::get_mesh()
{
    if(m_bcreate_outer_mesh)
    {
        get_outer_mesh();
        well_points = get_cylinder_mesh(false);
    }   
    else
    {
        get_inner_mesh();
    }
}


void FlatMesh::get_inner_mesh()
{
    // TODO: rewrite max_rad_param declaration
    vector_of_points inner_mesh_up = get_inner_points(3, true);
    vector_of_points inner_mesh_down = get_inner_points(3, false);

    inner_mesh_up.insert(
        inner_mesh_up.end(),
        inner_mesh_down.begin(),
        inner_mesh_down.end()
    );

    mesh_points.insert(
        mesh_points.end(),
        inner_mesh_up.begin(),
        inner_mesh_up.end()
    );
}


void FlatMesh::get_outer_mesh()
{
    // first create semi circle by params

    // calculate pw point
    auto top_circle_center    = point(0, m_zw - m_lw/2);
    auto bottom_circle_center = point(0, m_layer_height - m_zw - m_lw/2);

    // get points at upper sphere
    vector_of_points top_circle   = get_sphere_approx(top_circle_center, true);
    // get points at down sphere
    vector_of_points bottom_circle = get_sphere_approx(bottom_circle_center, false);

    // reverce the bottomside sphere container
    std::reverse(bottom_circle.begin(), bottom_circle.end());

    // get points at cylinder
    vector_of_points cylinder_mesh = get_cylinder_mesh(true);

    // connect meshes
    mesh_points.insert(mesh_points.end(), top_circle.begin(), top_circle.end());
    mesh_points.insert(mesh_points.end(), cylinder_mesh.begin(), cylinder_mesh.end());
    mesh_points.insert(mesh_points.end(), bottom_circle.begin(), bottom_circle.end());
}

