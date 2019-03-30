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
//^---
//^12
//^11
//^10
//^9
//^7
//^6
//^4
//^10
//^9
//^8
//^6
//^5
//^3

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

	println("---")
	var z,y = 10, 12
	for z>0 {
		if (z==4 || z==7) {
			z--
			continue
		} else if z==2 {
			break
		}
		for y>1 {
			if y==3 {
				break
			} 
			if y==5 || y==8 {
				y--
				continue
			}
			println(y)
			y--
		}
		println(z)
		z--
	}
}