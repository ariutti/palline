#!/usr/bin/env python3

import random as r

NPALLINE = 48

xs = []
ys = []

for i in range(NPALLINE):
	xs.append( float(r.randint(0.0, 100.0)) )
	ys.append( float(r.randint(0.0, 100.0)) )
	
	
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
