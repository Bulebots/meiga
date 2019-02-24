"""
This script defines the rim body.
"""
from math import cos
from math import sin
from math import radians

import cadquery
from Helpers import show

FDM_MARGIN = 0.1

TIRE_W = 2.5 * 2
WHEEL_D = 20.5
WHEEL_D_FDM = WHEEL_D - FDM_MARGIN

REEL_D0 = WHEEL_D_FDM - TIRE_W
REEL_H0 = 8

PINION_D = 6.70
PINION_D_FDM = PINION_D + FDM_MARGIN


# Rim body
rim = cadquery.Workplane('XY')\
    .circle(radius=REEL_D0/2.).extrude(distance=REEL_H0)\
    .faces('>Z').workplane()\
    .hole(diameter=PINION_D_FDM)

show(rim)