bspline is a method for determining curves from :

* control points forming the control polygon
* order of the curve k which is one more than the degree
* n is the order when every single point in the control point is a part of the bezier curve
* so essentially n is number of control points minus one
* knot vector determines the parameter range for any set of k control points in order

the parameter starts at knot(i) and ends at knot(i+1) and N(j,parameter) is generated for each parameter within a given resolution, j ranging from 0 to k

r(parameter)=N(j,parameter)*control point(j)

extending to surfaces:

r(para1,para2)=sigma (i=0 to n) (sigma (j=0 to m) (control point(i,j)*Nj,l(para1)) * Ni,k(para2))

