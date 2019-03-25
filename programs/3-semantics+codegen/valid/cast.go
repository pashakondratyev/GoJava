//^5
//^5
//^+6.000000e+000
//^@
//^r
//^5

// Casting
package main

func main () {
	type num int
	var a num = num(5)
	var b int = int(a)
	var c float64 = float64(6)
	var d string = string(64)
	var e string = string('r')
	var f rune = rune(a)

	println(int(a))
	println(b)
	println(c)
	println(d)
	println(e)
	println(f)
}