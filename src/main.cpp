#include <Eigen/Dense>
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>
#include <raisim/World.hpp>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

namespace Eigen {
    typedef Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic> MatrixXb;
}

namespace grid_map_raycasting {
    Eigen::MatrixXb rayCastGridMap(Eigen::Vector3d vantage_point, Eigen::MatrixXd grid_map, Eigen::Vector2d grid_resolution){
        const std::vector<double> height_vec(grid_map.data(), grid_map.data() + grid_map.rows() * grid_map.cols());

        double xSize = grid_map.rows() * grid_resolution[0];
        double ySize = grid_map.cols() * grid_resolution[1];
        double centerX = 0;
        double centerY = 0;

        raisim::World world;
        world.addHeightMap(grid_map.rows(), grid_map.cols(), xSize, ySize, centerX, centerY, height_vec);

        Eigen::MatrixXb occlusion_mask(grid_map.rows(), grid_map.cols());
        occlusion_mask.setConstant(false);

        for (int i = 0; i < grid_map.rows(); i++) {
            for (int j = 0; j < grid_map.cols(); j++) {
                double grid_cell_x = (-grid_map.rows()/2 + i) * grid_resolution(0);
                double grid_cell_y = (-grid_map.cols()/2 + j) * grid_resolution(1);
                double grid_cell_z = grid_map(i, j);
                Eigen::Vector3d grid_cell_position = {grid_cell_x, grid_cell_y, grid_cell_z};

                Eigen::Vector3d direction = grid_cell_position - vantage_point;
                double ray_length = direction.norm();
                direction /= ray_length;

                auto& col = world.rayTest(vantage_point, direction, 0.99*ray_length, true);
                if(col.size() > 0) {
                    occlusion_mask(i, j) = true;
                }
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

    m.def("setRaisimLicenseFile", &raisim::World::setActivationKey, R"pbdoc(
        Set the raisim license file path.

        Args:
            path: Path to the license file.
    )pbdoc",  py::arg("licenseFile"));

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
