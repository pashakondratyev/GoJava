// switch with multiple default branches
package test

var x = 5

func main () {
	switch x {
	case 2:
		println("Hi")
	default:
		println("hello")
	default:
		println("hey")
	}
}