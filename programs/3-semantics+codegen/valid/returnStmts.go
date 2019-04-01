//~0
//~1
//~0

// Return statements
package main

var a [5] int
var b [] int 
var c struct { f int ; }

func foo () [5] int { 
	return a; 
}

func bar () [] int { 
	return b;
}

func baz () struct { f int ; } { 
	return c; 
}

func main () {
	b = append(b,0)
	var d, e, f = foo(), bar(), baz()
	d[0], e[0], f.f = 1, 1, 1
	println(d[0])
	println(e[0])
	println(f.f)
}