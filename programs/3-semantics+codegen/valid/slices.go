//^1
//^2
//^3
//^4
//^5
//^2

// Slices
package main

func main() {
	var b []int
	b = append(b,1)
	b = append(b,2)
	b = append(b,3)
	b = append(b,4)
	b = append(b,5)

	// bounds
	println(b[0])
	println(b[1])
	println(b[2])
	println(b[3])
	println(b[4])

	var c []int;
  c = append(c, 5)
  c[0]=2;
  println(c[0])
}          