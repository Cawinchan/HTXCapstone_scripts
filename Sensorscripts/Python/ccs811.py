#!/usr/bin/env python3

import time
import board
import adafruit_ccs811
import smbus
import busio
from datetime import datetime

i2c = busio.I2C(board.SCL_1,board.SDA_1) # uses board.SCL and board.SDA

ccs811 = adafruit_ccs811.CCS811(i2c)



# wait for sensor to be ready
while not ccs811.data_ready:
	pass
with open("/home/nanocat/ccs811/ccs811_co2.csv", "a+") as log:
	while True:
		#print("C02: {} PPM, TVOC: {} PPB".format(ccs811.eco2,ccs811.tvoc))
		log.write("{},{},{}\n".format(datetime.timestamp(datetime.now()),ccs811.eco2,ccs811.tvoc))
		time.sleep(1)
