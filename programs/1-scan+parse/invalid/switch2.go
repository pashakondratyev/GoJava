// invalid switch - missing brackets
package test

var x = 7
func main() {
	switch x 
		case 0,1,2,3,4 : print("small")
}