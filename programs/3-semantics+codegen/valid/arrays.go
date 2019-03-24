//^true
//^false
//^0
//^0
//^5
//^0
//^0

// Arrays
package main

func main() {
	// equality
	var a, b [5]int
	println(a == b)
	b[2] = 5
	println(a==b)

	// bounds
	println(b[0])
	println(b[1])
	println(b[2])
	println(b[3])
	println(b[4])

}