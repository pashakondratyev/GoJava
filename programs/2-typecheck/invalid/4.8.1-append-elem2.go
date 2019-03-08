// invalid append: second expression has wrong type
package test

func foo() {
	var a []int
	append(a,'r')
}