// valid switch statement - default doesn't need to be the last case
package test

var x = 5

func main() {
	switch x {
		default: println("doesn't print")
		case 5: println("this prints")
	}
}