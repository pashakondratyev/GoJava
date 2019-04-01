//~0 
//~+0.000000e+000
//~
//~0 
//~0 
//~+0.000000e+000
//~
//~0 
//~false
//~hi
//~hi
//~hi

// Variable declarations
package main

func main() {
	// default values
	var a int
	var b float64
	var c string
	var d rune

	type num int
	type float float64
	type str string
	type char rune

	var e num
	var f float
	var g str
	var h char

	println(a)
	println(b)
	println(c)
	println(d)
	println(e)
	println(f)
	println(g)
	println(h)

	// multi-declarations
	var aa, ab, ac, ad, ae,af,ag int

	// shadowing of true & false
	var true = false
	println(true)

	// scoping
	var hi = "hi"
	{
		var hello = hi
		var hi = hi
		println(hello)
		println(hi)
		hi = hi
		println(hi)
	}
}


