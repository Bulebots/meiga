"""
This script defines the rim body.
"""
import cadquery
from Helpers import show

FDM_MARGIN = 0.1
REEL_H0 = 8
TIRE_W = 2.5
WHEEL_D = 20.5
PINION_D = 6.70

OUTER_D = WHEEL_D - TIRE_W * 2 - FDM_MARGIN

PINION_D = 6.70
INNER_D = PINION_D + FDM_MARGIN


# Rim body
rim = cadquery.Workplane('XY')\
    .circle(radius=OUTER_D/2.).extrude(distance=REEL_H0)\
    .faces('>Z').workplane()\
    .hole(diameter=INNER_D)

show(rim)
