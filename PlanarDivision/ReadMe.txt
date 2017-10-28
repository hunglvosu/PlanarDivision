/********************************************************************
Copyright 2017 Hung Le

 ********************************************************************/

Overview
========
Compute r-division of a planar graph where parameter r is the maximum size of any region
The program takes a planar embedding (rotational system) of the input graph and output the r-division of the graph
The embedding file has format as in the output of John M. Boyer's implementation of Boyer-Myrvold planar embedding
The planar embedding implementation of Boyer can be found at http://jgaa.info/accepted/2004/BoyerMyrvold2004.8.3/planarity.zip. 
Note that in the rotational system of the output, neighbors of each vertex are ordered counter-clockwise while our planar representation is clockwise.

Usage
======================
The program is written in Visual C+++ (Visual Studio 2015) and an exe file (named PlanarDivision.exe) is created in release mode (x86).
A sample format of the command line is:
	PlanarDivision.exe input.txt output.txt 5
where "input.txt" is the input planar embedding file (output of Boyer's program) and "output.txt" is the output r-division. The last number is the value of r (in this case is 5).

Output Format
======================

The output file has the following sample format:

#0
N=3
2 4 6
#1
N=3 M=4
5: 8
7: 8
8: 7 5
#2
N=3 M=4
0: 1 3
1: 0
3: 0

The output above is obtained by applying r-division to a 3x3 grid (vertices are numberred from 0-9 row-by-row) with r = 3
where #0 is preserved for the id of the boundary in r-division. Next line is the number of vertices in the boundary and the third line contains vertices in the boundary.

Each #i corresponds to a region in r-division, with N and M is the number of vertices and arcs inside the region. Remember #-arcs = 2 * #-of-edges so M is always even.
Next lines are rotational system representation of each region such that each line has:
	_ The first number is the id of the vertex
	_ The following numbers are neighbors of the vertex in the rotational system (clockwise order) 

There are cases when each region consist of only 1 vertex. In the output file, it would have the following sample format:

#4
N=1 M=0
9:

The above sample is a region of a single vertex (id = 5) and has no edge. 
