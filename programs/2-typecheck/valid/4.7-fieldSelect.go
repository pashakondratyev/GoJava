// valid field selection 
package test

func foo() {
	type structType struct {
		a int
		b float64 
		c rune
		d string
		e [5]int
	}
	var structElem structType

	var e int = structElem.a
	var f float64 = structElem.b
	var g rune = structElem.c
	var h string = structElem.d
	var i [5]int = structElem.e
	var j int = structElem.e[2]
}