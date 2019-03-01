// can only have identifiers on the lhs of a short variable declaration (no parentheses)
package test

func main() {
	y, (4+5) := 1,2;
}