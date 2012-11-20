#! /usr/bin/env python

number = 600851475143

def prime_factors(n):
    "Returns all the prime factors of a positive integer"
    factors = []
    d = 2
    while (n > 1):
        while (n%d==0):
            factors.append(d)
            n /= d
        d = d + 1

    return factors 

primes = prime_factors(600851475143)

print max(primes)
	
	
	