// valid switch using expressions
package test

var x = -5

func main(){
	switch {
	case x < 0:
		println("Negative")
		fallthrough
	case x > 0: 
		println("Positive")
		fallthrough
	case x==0:
		println("Zero")
		fallthrough
	default:
		println("Done")
	}
}