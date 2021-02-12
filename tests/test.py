# -*- coding: utf-8 -*-
import grid_map_raycasting as m
import faulthandler; faulthandler.enable()
from matplotlib import pyplot as plt
import numpy as np

# Vantage point (e.g. robot position) is at x=0, y=0, z=1
vantage_point = np.array([0., 0., 1.], dtype=np.double)
# We define a flat Digital Elevation Map (DEM) at the ground-level
grid_map = np.zeros((64, 64), dtype=np.double)
grid_map[0:30, 0:10] = 3
# We define the resolution of the grid of the DEM in x and y direction
grid_resolution = np.array([1, 1], dtype=np.double)

# we perform ray-casting to receive the occlusion mask
occlusion_mask = m.rayCastGridMap(vantage_point, grid_map, grid_resolution)

# The occlusion mask we expect to receive as the output of the ray-casting
target_occlusion_mask = np.full((64, 64), False)
target_occlusion_mask[0:29, 0:9] = True

fig, axes = plt.subplots(nrows=1, ncols=2)
axes[0].set_title("Grid map")
axes[0].matshow(grid_map)
axes[1].set_title("Occlusion mask")
axes[1].matshow(occlusion_mask)
plt.show()

assert m.__version__ == "0.0.1"
assert (occlusion_mask == target_occlusion_mask).all()
