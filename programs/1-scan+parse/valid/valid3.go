// Approximate the square root of x
package test

var x float64 = 12.0
var guess float64 = 1.0
var iter int = 15

func main(){
	for iter > 0 {
		var quot float64 = x/guess
		guess = 0.5 * (guess + quot)
		iter--
	}
	
	println(guess)
	println(guess*guess)
}