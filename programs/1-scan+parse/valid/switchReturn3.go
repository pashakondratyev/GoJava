// default case falls through to return 
package test

func main() int {
	var i = 3
	switch i {
		case 0: return 0
		case 1: return 5
		default: fallthrough
		case 2: return 9
	}
}