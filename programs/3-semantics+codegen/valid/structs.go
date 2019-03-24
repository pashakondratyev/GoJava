//^true
//^false
//^true
//^false
//^true
//^false
//^true
//^false

// Structs
package main

func main() {
	// equality
	var a, b struct {
		j int
		k string
		_ rune
		_ float64
		l rune
		m float64
	}
	println(a == b)
	b.j = 2
	println(a == b)
	a.j = 2 
	println(a == b)
	b.k = "hi"
	println(a == b)
	a.k = "hi"
	println(a == b)
	b.l = 'r'
	println(a == b)
	a.l = 'r'
	println(a == b)
	b.m = .25
	println(a == b)
	a.m = .25  


}