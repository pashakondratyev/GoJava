//~true
//~false
//~true
//~7 2
//~false

package main

func main() {

	var true = true
	println(true)
	true, x := false, 2
	var false = 7	// different variable type
	if 0==0 {
		var true = true
		println(true)
		true = 0==0
		println(true)
		println(false, x)
	}
	println(true)
}