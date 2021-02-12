#include <Eigen/Dense>
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

namespace Eigen {
    typedef Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic> MatrixXb;
}

namespace grid_map_raycasting {
    Eigen::MatrixXb rayCastGridMap(Eigen::Vector3d vantage_point, Eigen::MatrixXd grid_map, Eigen::Vector2d grid_resolution){
        Eigen::MatrixXb occlusion_mask(grid_map.rows(), grid_map.cols());
        occlusion_mask.setConstant(false);

        for (int i = 0; i < grid_map.rows(); i++) {
            for (int j = 0; j < grid_map.cols(); j++) {
                double grid_cell_x = (-grid_map.rows()/2 + i + 0.4) * grid_resolution(0);
                double grid_cell_y = (-grid_map.cols()/2 + j + 0.4) * grid_resolution(1);
                double grid_cell_z = grid_map(i, j);

                if (std::isnan(grid_cell_z)){
                    // we skip already occluded cells to improve computational efficiency
                    occlusion_mask(i, j) = true;
                    continue;
                }

                Eigen::Vector3d grid_cell_pos = {grid_cell_x, grid_cell_y, grid_cell_z};

                Eigen::Vector3d direction = grid_cell_pos - vantage_point;
                double ray_length = direction.norm();
                direction /= ray_length;

                Eigen::Vector3d raycast_pos = vantage_point;
                bool grid_cell_occluded = false;
                while (grid_cell_occluded == false) {
                    raycast_pos += direction * std::min(grid_resolution(0), grid_resolution(1));

                    int raycast_u = (int)std::round(grid_map.rows() / 2 + raycast_pos(0) / grid_resolution(0));
                    int raycast_v = (int)std::round(grid_map.cols() / 2 + raycast_pos(1) / grid_resolution(1));
                    // int raycast_u = (int)(grid_map.rows() / 2 + raycast_pos(0) / grid_resolution(0));
                    // int raycast_v = (int)(grid_map.cols() / 2 + raycast_pos(1) / grid_resolution(1));

                    // the grid_cell cannot occlude itself, thats why we do not consider the final cell
                    if ((i == raycast_u && j == raycast_v)) {
                        // std::cout << "break because we reached grid_cell for i=" << i << ", j=" << j << std::endl;
                        break;
                    }

                    if ((raycast_pos - vantage_point).norm() > ray_length) {
                        // std::cout << "break because we are past max_distance for i=" << i << ", j=" << j << std::endl;
                        break;
                    }

                    double ground_elevation = grid_map(raycast_u, raycast_v);
                    // we do not consider cells with missing elevation information to cause occlusion
                    if (!std::isnan(ground_elevation)){
                        // we consider a cell to be occluded, if the ray hits a higher elevation on its trajectory to the cell
                        if (ground_elevation > raycast_pos(2)){
                            grid_cell_occluded = true;
                        }
                    }
                }

                occlusion_mask(i, j) = grid_cell_occluded;
            }
        }

        return occlusion_mask;
    }
}

PYBIND11_MODULE(grid_map_raycasting, m) {
    m.doc() = R"pbdoc(
        C++ component including Python bindings to raycast a gridmap from a viewpoint to check for occlusions
        -----------------------

        .. currentmodule:: grid_map_raycasting

        .. autosummary::
           :toctree: _generate
    )pbdoc";

    m.def("rayCastGridMap", &grid_map_raycasting::rayCastGridMap, R"pbdoc(
        Raycast every cell on the grid from a constant origin of the ray.

        It returns a grid map of booleans which signify weather the grid cell is visible from the vantage point of the robot or if its hidden by the terrain.
        Formulated alternatively, it creates an occlusion mask for a given Digital Elevation Map (DEM) which stores true for occluded and false for visible.
    )pbdoc", py::arg("vantage_point"), py::arg("grid_map"), py::arg("grid_resolution"));


#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
