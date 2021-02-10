# -*- coding: utf-8 -*-
import grid_map_raycasting as m
import numpy as np


vantage_point = np.array([0., 0., 1.], dtype=np.double)
dem = np.zeros((64, 64), dtype=np.double)
occlusion_mask = np.full((64, 64), False)

assert m.__version__ == "0.0.1"
assert (m.raycast_grid_map(vantage_point, dem) == occlusion_mask).all()
