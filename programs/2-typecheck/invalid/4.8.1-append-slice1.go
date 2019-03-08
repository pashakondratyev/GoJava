// invalid append: first expression not a slice
package test

func foo() {
	var a [4]int
	append(a,4)
}