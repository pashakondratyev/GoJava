// invalid variable declaration - number of identifiers don't match number of expressions
package test

var (
	x1, x2 = 12, 13, 14
	_, y2 = 12
	z1 int = 12, 13
	a1, _ int = 12
)