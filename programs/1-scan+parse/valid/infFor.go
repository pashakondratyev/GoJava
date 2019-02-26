// infinite for loop
package test

var x = 8
var y = 12

func main(){
	for {
		x++
		y--
		if y == 5 {
			continue
		}
		println(x)
		if x == 45 || x == 987 {
			break
		}
	}
}