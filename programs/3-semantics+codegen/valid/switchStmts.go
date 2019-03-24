//^hi
//^hello
//^yo
//^hi
//^yo
//^hi
//^hey

// Switch statements
package main

func foo() int{
	println("hi")
	return 1
}

func baz() int {
	println("hello")
	return 2
}

func main() {
	b,c := 9, 4
	switch foo() {
		case baz(), 1:
			if (b>c) {
				println("yo")
				break
			}
		default:
	}

	switch foo() {
		case 1, baz():	// baz() never called since 1 matches foo()
			if (b>c) {
				println("yo")
				break
			}
		default:
	}

	switch foo() {
		case 2,3,4,5:
			println("doesn't print")
		case 6,7,8,9:
			println("doesn't print")
		case 10:
			println("doesn't print")
		default:
				println("hey")
	}
}