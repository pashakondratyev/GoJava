//^hi
//^hey
//^in loop
//^goodbye
//^in loop
//^hello
//^in loop
//^goodbye
//^in loop
//^hello
//^in loop

// For Loops
package main

func main() {
	for {
		println("hi")
		break
	}

	var a, b int
	for a+b==0 {
		println("hey")
		b++
	}

	for a,b,c:=0,1,2; a<b || b<a; a,b = b,a {
		println("in loop")
		c++
		if (a>b) {
			println("hello")
			continue
		}
		if c>5 {
			break
		}
		println("goodbye")
	}
}