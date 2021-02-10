#include <Eigen/Dense>
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>
#include <raisim/World.hpp>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

namespace grid_map_raycasting {
    Eigen::MatrixXd raycast_grid_map(Eigen::Vector3d vantage_point, Eigen::MatrixXd dem){
        const std::vector<double> height_vec(dem.data(), dem.data() + dem.rows() * dem.cols());

        raisim::World world;
        world.addHeightMap(dem.rows(), dem.cols(), 10., 10., 0, 0, height_vec);

        for (int i = 0; i < dem.rows(); i++) {
            for (int j = 0; j < dem.cols(); j++) {
                std::cout << "i=" << i << " j=" << j << std::endl;
            }
        }

        // Eigen::MatrixXf occlusion_mask = dem;
        return dem;
    }
}

/*self.dem = dem
self.res_grid = res_grid
self.max_ray_cast_dist = (np.max(res_grid) * np.max(dem.shape)).item()

center_x, center_y = 0., 0.
height_vec = dem.copy().reshape((-1, ), order="F")

self.world = self.raisim.World()
self.world.addHeightMap(dem.shape[0], dem.shape[1], dem.shape[0]*res_grid[0], dem.shape[1]*res_grid[1],
                        center_x, center_y, height_vec)*/

PYBIND11_MODULE(grid_map_raycasting, m) {
    m.doc() = R"pbdoc(
        C++ component including Python bindings to raycast a gridmap from a viewpoint to check for occlusions
        -----------------------

        .. currentmodule:: grid_map_raycasting

        .. autosummary::
           :toctree: _generate
    )pbdoc";

    m.def("raycast_grid_map", &grid_map_raycasting::raycast_grid_map, R"pbdoc(
        Raycast every cell on the grid from a constant origin of the ray.

        It returns a grid map of booleans which signify weather the grid cell is visible from the vantage point of the robot or if its hidden by the terrain.
        Formulated alternatively, it creates an occlusion mask for a given Digital Elevation Map (DEM) which stores true for occluded and false for visible.
    )pbdoc", py::arg("vantage_point"), py::arg("dem"));


#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
