// invalid append: first expression not a slice
package test

func foo() {
	var a int
	append(a,4)
}