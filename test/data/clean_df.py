#!/usr/bin/env python2
#-*- coding:utf-8 -*-

import sys


def clean_elem(elem_str):
	if elem_str.startswith("df:"):
		elem_str = elem_str[3:]
	return elem_str;	

for line in sys.stdin:
	if not line:
		break;
	line_arr = line.strip().split("\t")
	if len(line_arr) != 4:
		continue
	cleaned_str = map(clean_elem,line_arr[:3])
	print "\t".join(cleaned_str)	

	
