#pragma once


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


int get_elems_size(std::vector<std::vector<double>> elems)
{
    int size = 0;

    for(auto elem : elems)
    {
        size += elem.size();
    }

    return size;
}


void write_mesh_data(std::ofstream stream, std::vector<point3d> mesh)
{
    
}


void form_vtk_file(
    std::vector<point3d> pts, 
    std::vector<std::vector<double>> elems
)
{
    std::ofstream file;

    file.open("mesh.vtk");

    if(file.is_open())
    {
        size_t points_count = pts.size();
        size_t elems_count  = elems.size();

        int vtk_mesh_type = 2;

        file << "# vtk DataFile Version 2.0" << std::endl;
        file << "Semi sphere mesh" << std::endl;
        file << "ASCII" << std::endl;
        file << "DATASET UNSTRUCTURED_GRID" << std::endl;
        file << "POINTS " << points_count << " float" << std::endl; 

        for (size_t i = 0; i < points_count; i++)
        {
            file << float(bg::get<0>(pts[i])) << "\t" << float(bg::get<1>(pts[i])) << "\t" << float(bg::get<2>(pts[i])) << std::endl;
        }

        file << "CELLS " << points_count + elems_count << "\t" << 2 * points_count + elems_count + get_elems_size(elems) << std::endl;

        for(size_t i = 0; i < points_count; i++)
        {
            file << 1 << "\t" << i << std::endl;
        }

        for(size_t i = 0; i < elems_count; i++)
        {
            file << elems[i].size() << "\t";
            
            for(auto idx : elems[i])
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
            if(elems[i].size() == 6)
            {
                file << 13 << std::endl;
            }
            else if(elems[i].size() == 4)
            {
                file << 10 << std::endl;
            }
            else if(elems[i].size() == 5)
            {
                file << 14 << std::endl;
            }
        }

        file.close();
    }
    else
    {
        throw std::runtime_error("File can not to be opened");
    }
}