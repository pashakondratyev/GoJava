// absence of default case means return value isn't guaranteed
package test

func main() int {
	var i = 3
	switch i {
		case 0: return 0
		case 1: return 3 
	}
}