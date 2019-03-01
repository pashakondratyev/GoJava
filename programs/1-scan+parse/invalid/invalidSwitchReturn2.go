// not all cases return or fall through to a return
package test

func main() int {
	var i = 3
	switch i {
		case 0: fallthrough
		case 1: fallthrough
		default: fallthrough
		case 3: println("three?")
		case 4: return 2
	}
}