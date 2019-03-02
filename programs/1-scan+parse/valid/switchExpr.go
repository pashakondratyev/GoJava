// valid switch using expressions
package test

var x = -5

func main() {
	var y int
	switch y=12; {
	case x < 0:
		println("Negative")
	case x > 0: 
		println("Positive")
	case x==0:
		println("Zero")
	default:
		println("Done")
	}
}