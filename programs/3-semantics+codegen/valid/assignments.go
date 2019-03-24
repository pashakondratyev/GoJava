//^0
//^5
//^0
//^1
//^0
//^5

// Assignments
package main

func foo(x int) int {
	println(x)
	return x
}

func main() {
	var aa, bb int
	aa = 5
	_ = 5
	aa, bb = bb, aa
	println(aa)
	println(bb)


	var a , b [5]int 	// pass by value
	b = a
	a[0] = 1
	println(b[0])

	var c , d []int 		// pass by header value, changes d
	c = append(c , 0)
	d = c
	c[0] = 1
	println(d[0])


	var e , f struct { f int ; } 	// pass by value
	f = e
	e.f = 1
	println(f.f)

	_ = foo(5)	// evaluate RHS even if result stored in blank id

}