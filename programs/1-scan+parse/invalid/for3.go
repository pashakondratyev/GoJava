// invalid use of var in for clause
package test

func main() {
	for var i=0; i<10; i++ {
		println("hi")
	}
}