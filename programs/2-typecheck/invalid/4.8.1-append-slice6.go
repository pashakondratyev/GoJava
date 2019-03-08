// invalid append: first expression not a slice or array
package test

func foo() {
	type point struct {
		x,y int
	}
	var a point
	append(a,4)
}