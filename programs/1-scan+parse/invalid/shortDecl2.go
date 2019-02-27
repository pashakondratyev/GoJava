// can only have identifiers on the lhs of a short variable declaration (no parentheses)
package test

var x = 12

func main() {
	(y) := x
}