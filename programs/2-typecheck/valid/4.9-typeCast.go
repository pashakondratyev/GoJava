// valid type casts
package test

func foo() {
	type num int
	type natural num

	var a num = num(5)
	var b natural = natural(a+7)

	var c int = int(5.72)
	var d float64 = float64(4)

	var e string = string(456)
	var f string = string('r')
}