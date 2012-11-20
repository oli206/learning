#! /usr/bin/env python

candidates = []

for number1 in range(999,100,-1):
	for number2 in range(999,100,-1):
		product = number1 * number2;
		print "n1: {0}, n2 {1}, product {2}".format(number1,number2,product)
		productStr = str(product)
		productReverse = productStr[::-1]
	
		if productStr == productReverse:
			candidates.append(product)
	
print max(candidates)
	
		
	