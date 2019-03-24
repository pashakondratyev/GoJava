//^bar
//^2

// Blank identifiers
package main

func _() {
	println("can't reach")
}

func foo ( _ int , a int , _ int ) { 
	println(a)
}

func bar() int {
	println ("bar")
	return 0
}

func main() {
	var _ int = bar()
	var _ float64
	var _ string
	var _ rune
	var _ []int
	var _ [12]rune

	type _ struct { a int ; }

	type ypoint struct { 
		_ int;
		y int;
		_ int;
	}

	foo(1,2,3)

}

func _() {
	println("can't reach")
}