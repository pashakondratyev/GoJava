// valid multiple return statements in GoLite
package test

func f(a, b int) int {
	if a > 0 {
		return a;
	} else {
		return b;
	}
}