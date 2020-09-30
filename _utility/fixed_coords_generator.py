#!/usr/bin/env python3

import random as r

NPALLINE = 48


xs = []
ys = []

xspace = 100.0 / 12.0
yspace = 30.0 / 4.0

for j in range(4):
	for i in range(12):
		xs.append( round(xspace * i, 2) )

for j in range(4):
	for i in range(12):
		ys.append( round(yspace * j, 2) )
	
	
def printXs():
	print("float xs[] = {", end = '')
	for x in xs:
		print( x, end = '' )
		print(",", end = '')
	print("};")
	
def printYs():
	print("float ys[] = {", end = '')
	for y in ys:
		print( y, end = '' )
		print(",", end = '')
	print("};")
	
printXs()
printYs()
