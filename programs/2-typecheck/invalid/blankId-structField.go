// invalid blank id struct field access
package test

var a struct {
	_ int
}

func main() {
	a._
}