//~2
//~5
//~2
//~1

// Length
package main

func main () {
	var a = "hi"
	println(len(a))

	var b [5]int
	println(len(b))

	var c, d []int
	c = append(c, 0)
	d = c
	c = append (c , 1)
	println(len(c))
	println(len(d))
}