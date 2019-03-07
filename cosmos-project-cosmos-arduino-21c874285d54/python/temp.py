# -*- coding: utf-8 -*-
"""
Created on Thu Aug  2 12:00:58 2018

@author: khagi
This program reads a Dallas DS18B20 Temperature Sensor from on a raspberry pi
and prints the output to stdout in a COSMOS JSON string 
ex : {"device_tsen_temp_000": 74.00}
"""

import os
import glob
import time
import sys


os.system('modprobe w1-gpio')
os.system('modprobe w1-therm')
 
base_dir = '/sys/bus/w1/devices/'
device_folder = glob.glob(base_dir + '28*')[0]
device_file = device_folder + '/w1_slave'
 
def read_temp_raw():
    f = open(device_file, 'r')
    lines = f.readlines()
    f.close()
    return lines
 
def read_temp():
    lines = read_temp_raw()
    while lines[0].strip()[-3:] != 'YES':
        time.sleep(0.2)
        lines = read_temp_raw()
    equals_pos = lines[1].find('t=')
    if equals_pos != -1:
        temp_string = lines[1][equals_pos+2:]
        temp_c = float(temp_string) / 1000.0
        temp_f = temp_c * 9.0 / 5.0 + 32.0
        return temp_c, temp_f

while True:
    temp_c, temp_f = read_temp()
    sys.stdout.write("{\"device_tsen_temp_000\": " + str(temp_f)+"} \n")
    sys.stdout.flush()

    time.sleep(1)