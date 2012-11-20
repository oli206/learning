#! /usr/bin/env python

vector = [7,1,4,9,2,5,3,8]

def mergesort(v, li, ld):
	if li < ld:
		med = (li + ld) / 2
		mergesort(v,li,med)
		mergesort(v,med + 1,ld)
		w = v[:]
		i = li
		j = med + 1
		k = li
		while i <= med and j <= ld:
			if w[i] <= w[j]:
				v[k] = w[i]
				i = i + 1
			else:
				v[k] = w[j]
				j = j + 1
			k = k + 1
		for l in range(i,med):
			v[k] = w[l]
			k = k + 1
		for l in range(j,ld):
			v[k] = w[l]
			k = k + 1

mergesort(vector,0,7)