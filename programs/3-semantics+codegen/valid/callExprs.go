//~1
//~1
//~1
//~7
//~+2.500000e-001
//~115
//~hello hi
//~false
//~0
//~1
//~0
//~0
//~+0.000000e+000
//~114
//~hello
//~false

// Call Expressions
package main

func foo(a [5]int, b []int , c struct{ f int ; }, d int, e float64, f rune, g string, h bool) {
	a[0] = 1
	b[0] = 1
	c.f = 1
	d = d + 7
	e = e + 0.25
	f = f + rune(1)
	g = g + " hi"
	h = h && true

	println(a[0])
	println(b[0])
	println(c.f)
	println(d)
	println(e)
	println(f)
	println(g)
	println(h)
}


func main () {
	var a [5]int
	var b []int
	b = append (b , 0)
	var c struct { f int ; }
	var d, e, f, g, h = 0, 0.0, 'r', "hello", false;
	foo(a, b, c, d, e, f, g, h)

	println(a[0])
	println(b[0])
	println(c.f)
	println(d)
	println(e)
	println(f)
	println(g)
	println(h)
}
