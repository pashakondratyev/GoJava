//~0
//~0
//~true
//~0

package main

func unary_exprs() {
	var x int
	var y int
	var b bool
	var c bool
	// single
	y = -x
	y = +x
	c = !b
	y = ^x

	// multiple
	y = - -x
	println(y)
	y = + +x
	println(y)
	c = !!c
	println(c)
	y = ^^x
	println(y)
} 

func main() {
	unary_exprs()
}
