"""
This script defines the rim body.
"""
from math import cos
from math import sin
from math import radians

import cadquery

TIRE_W = 2.5 * 2
WHEEL_D = 20.5

REEL_D0 = WHEEL_D - TIRE_W
REEL_H0 = 8

PINION_D = 6.70


# Rim body
rim = cadquery.Workplane('XY')\
    .circle(radius=REEL_D0/2.).extrude(distance=REEL_H0)\
    .faces('>Z').workplane()\
    .hole(diameter=PINION_D)