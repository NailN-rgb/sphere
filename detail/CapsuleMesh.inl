#pragma once


void CapsuleMesh::get_capsular_elements()
{
    // first rotate mesh nodes
    get_capsular_nodes();

    mesh_elements = connect_elems();

    // then create capsule mesh
}


void CapsuleMesh::get_capsular_nodes()
{
    add_well_trajectory_to_mesh();

    mesh_points_vector layers_mesh = get_mesh_by_layers();

    capsule_mesh.insert(
        capsule_mesh.end(),
        layers_mesh.begin(),
        layers_mesh.end()
    );
}


mesh_points_vector CapsuleMesh::get_mesh_by_layers()
{
    mesh_points_vector layers_mesh;

    for(index_type i = 0; i < m_capsule_count; i++)
    {
        auto current_radius = std::exp(i * (std::log(m_max_radius) / m_capsule_count));

        mesh_points_vector mesh_at_layer = get_capsular_mesh_at_layer(current_radius);

        layers_mesh.insert(
            layers_mesh.end(),
            mesh_at_layer.begin(),
            mesh_at_layer.end()
        );
    }

    return layers_mesh;
}


void print3d_mesh(std::vector<point3d> pts)
{
    for(size_t i = 0; i < pts.size(); i++)
    {
        std::cout << "\n Point number " << i << std::endl;
        std::cout << "x : " << bg::get<0>(pts[i]) << std::endl;
        std::cout << "y : " << bg::get<1>(pts[i]) << std::endl;
        std::cout << "z : " << bg::get<2>(pts[i]) << std::endl;
    }
}


int get_elems_size(std::vector<Element> elems)
{
    int size = 0;

    for(auto elem : elems)
    {
        size += elem.get_element_size();
    }

    return size;
}


void CapsuleMesh::form_vtk_file()
{
    std::ofstream file;

    file.open("mesh.vtk");

    if(file.is_open())
    {
        size_t points_count = capsule_mesh.size();
        size_t elems_count  = mesh_elements.size();

        int vtk_mesh_type = 2;

        file << "# vtk DataFile Version 2.0" << std::endl;
        file << "Semi sphere mesh" << std::endl;
        file << "ASCII" << std::endl;
        file << "DATASET UNSTRUCTURED_GRID" << std::endl;
        file << "POINTS " << points_count << " float" << std::endl; 

        for (size_t i = 0; i < points_count; i++)
        {
            file << float(bg::get<0>(capsule_mesh[i])) << "\t" << float(bg::get<1>(capsule_mesh[i])) << "\t" << float(bg::get<2>(capsule_mesh[i])) << std::endl;
        }

        file << "CELLS " << points_count + elems_count << "\t" << 2 * points_count + elems_count + get_elems_size(mesh_elements) << std::endl;

        for(size_t i = 0; i < points_count; i++)
        {
            file << 1 << "\t" << i << std::endl;
        }

        for(size_t i = 0; i < elems_count; i++)
        {
            file << mesh_elements[i].get_element_size() << "\t";
            
            for(auto idx : mesh_elements[i].get_element())
            {
                file << idx << '\t'; 
            }

            file << std::endl;
        }


        file << "CELL_TYPES " << points_count + elems_count << std::endl;

        for(size_t i = 0; i < points_count; i++)
        {
            file << 2 << std::endl;
        }

        for(size_t i = 0; i < elems_count; i++)
        {
            if(mesh_elements[i].get_element_size() == 6)
            {
                file << 13 << std::endl;
            }
            else if(mesh_elements[i].get_element_size() == 4)
            {
                file << 10 << std::endl;
            }
            else if(mesh_elements[i].get_element_size() == 5)
            {
                file << 14 << std::endl;
            }
            else if(mesh_elements[i].get_element_size() == 8)
            {
                file << 12 << std::endl;
            }
        }

        file.close();
    }
    else
    {
        throw std::runtime_error("File can not to be opened");
    }
}
