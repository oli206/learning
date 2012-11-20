#! /usr/bin/env python
# smallest number divisible by numbers from 1 to 20

finished = False
numberToCheck = 380

while not finished:
	finished = numberToCheck % 20 == 0 and numberToCheck % 19 == 0 and \
	numberToCheck % 18 == 0 and numberToCheck % 17 == 0 and numberToCheck % 16 == 0 and \
	numberToCheck % 15 == 0 and numberToCheck % 14 == 0 and numberToCheck % 13 == 0 and \
	numberToCheck % 12 == 0 and numberToCheck % 11 == 0 and numberToCheck % 10 == 0
	
	if not finished:
		numberToCheck += 380
	
print numberToCheck