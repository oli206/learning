#! /usr/bin/env python

def fib2(n): # return Fibonacci series up to n
    """Return a list containing the Fibonacci series up to n."""
    result = []
    a, b = 0, 1
    while a < n:
        result.append(a)    # see below
        a, b = b, a+b
    return result

fibNumbers = fib2(4000000)

sum = 0

for x in fibNumbers:
	if x % 2 != 0:
		sum += x

print sum