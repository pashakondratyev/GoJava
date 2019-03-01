// all cases of switch statement fallthrough to a return statement
package test

func main() int {
	var i = 3
	switch i {
		case 0: fallthrough
		case 1: fallthrough
		default: return -1
	}
}