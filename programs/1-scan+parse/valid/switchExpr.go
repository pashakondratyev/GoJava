// valid switch using expressions
package test

var x = -5

func main(){
	switch {
	case x < 0:
		println("Negative")
		break
	case x > 0: 
		println("Positive")
		break
	case x==0:
		println("Zero")
		break
	default:
		println("Done")
	}
}
