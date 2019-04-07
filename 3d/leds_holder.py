"""
This script defines the LEDS holder
"""
import cadquery
from Helpers import show

HOLDER_X = 14		
HOLDER_Y = 8
HOLDER_Z = 8

LED_D = 6.1
LED_POS_X = -2.5
SENSOR_D = 4.1
SENSOR_POS_X = 3.5

LINK_X = 1
LINK_Y = 1 

LINK_POS_X = HOLDER_X/2 + LINK_X/2 + 0.5

# LEDs holder body
leds = cadquery.Workplane('XY')\
	.rect(HOLDER_X, HOLDER_Y)\
	.center(LED_POS_X,0).circle(LED_D/2).center(-LED_POS_X,0)\
	.center(SENSOR_POS_X,0).circle(SENSOR_D/2).center(-SENSOR_POS_X,0)\
	.center(LINK_POS_X,0).rect(LINK_X, LINK_Y)\
	.extrude(HOLDER_Z)

show(leds)
