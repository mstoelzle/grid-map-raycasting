# grid-map-raycasting

C++ component including Python bindings to raycast a gridmap from a viewpoint to check for occlusions

## Prerequisites

This package relies on [RaiSim](https://raisim.com) to perform a [rayTest](https://raisim.com/sections/RayTest.html).
Please acquire the suitable license.

**On Unix (Linux, OS X)**

* A compiler with C++11 support
* CMake >= 3.12
* Ninja or Pip 10+

**On Windows**

* Visual Studio 2015 or newer (required for all Python versions, see notes below)
* CMake >= 3.12 (3.8 was the first version to support VS 2015) or Pip 10+

## Installation
Please install the following standard packages to use this repo:
On macOS this can be done with:
```bash
brew install cmake pybind11 eigen
```
or on Ubuntu with:
```bash
sudo apt install cmake python-pybind11 libeigen3-dev
```

Next, please install raisimLib: https://raisim.com/sections/Installation.html
After following the installation instructions, the build directory needs to be added to the CMAKE_PREFIX_PATH:
```bash
export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$LOCAL_INSTALL
```

Subsequently, we can install the Python module using pip:
```bash
pip install --user .
```

With the `setup.py` file included in this example, the `pip install` command will
invoke CMake and build the pybind11 module as specified in `CMakeLists.txt`.

## Usage
Please refer to the following sample code:
```python
from grid_map_raycasting import setRaisimLicenseKey, rayCastGridMap
import numpy as np

# Vantage point (e.g. robot position) is at x=0, y=0, z=1
vantage_point = np.array([0., 0., 1.], dtype=np.double)
# We define a flat Digital Elevation Map (DEM) at the ground-level
grid_map = np.zeros((64, 64), dtype=np.double)
# We define the resolution of the grid of the DEM in x and y direction
grid_resolution = np.array([0.1, 0.1], dtype=np.double)

# We raycast the DEM from the vantage point and check for every grid cell 
# weather it is visible from the vantage point
occlusion_mask = rayCastGridMap(vantage_point, grid_map, grid_resolution)
```

## Special notes for Windows

**Compiler requirements**

Pybind11 requires a C++11 compliant compiler, i.e Visual Studio 2015 on Windows.
This applies to all Python versions, including 2.7. Unlike regular C extension
modules, it's perfectly fine to compile a pybind11 module with a VS version newer
than the target Python's VS version. See the [FAQ] for more details.

**Runtime requirements**

The Visual C++ 2015 redistributable packages are a runtime requirement for this
project. It can be found [here][vs2015_runtime]. If you use the Anaconda Python
distribution, you can add `vs2015_runtime` as a platform-dependent runtime
requirement for you package: see the `conda.recipe/meta.yaml` file in this example.


