//~3 hello 99 true +3.141592e+000
//~0  0 +0.000000e+000

package main

type salary float64

type employee struct {
	id int
	name string
	sex rune
	wage salary
}

func main() {
	println(3, "hello", 'c', true, 3.141592)

	var e employee
	println(e.id, e.name, e.sex, e.wage)
}
