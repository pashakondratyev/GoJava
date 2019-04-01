//~12
//~15
//~0
//~"hi"
//~0
//~5

// Function declarations
package main

type kind struct { a int ; }

func foo(a kind) { 
	println(a.a)
}

func bar(b [12]int, c []string) {
	println(b[1])
	println(c[0])
}

func baz(a struct { a int ; }) {
	println(a.a)
}

func bah() struct {a int;} {
	var temp struct {
		a int;
	}
	temp.a = 5
	return temp
}

func main() {
	var a kind
	a.a = 12
	foo(a)
	a.a=15
	foo(a)
	var b [12]int
	var c []string
	c = append(c,"hi")
	bar(b, c)
	var d struct { a int ; }
	baz(d)
	d.a = 5
	baz(d)
}


