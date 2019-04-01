//~0
//~1
//~0

// Call Expressions
package main

func foo(a [5]int, b []int , c struct{ f int ; }) {
	a[0] = 1
	b[0] = 1
	c.f = 1
}


func main () {
	var a [5]int
	var b [] int 
	b = append (b , 0)
	var c struct { f int ; }
	foo (a, b, c)

	println(a[0])
	println(b[0])
	println(c.f)
}