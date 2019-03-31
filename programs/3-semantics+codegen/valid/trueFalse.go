//^false
//^true
//^false

package main

func main() {

	var true = false
	if 0==0 {
		var true = true
		println(true)
		true = 0==0
		println(true)
	}
	println(true);
}