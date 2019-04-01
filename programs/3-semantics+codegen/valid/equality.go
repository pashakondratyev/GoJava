//~true
//~true
//~true
//~true
//~true
//~true
//~true
//~true
//~---
//~false
//~true
//~false
//~true
//~false
//~true
//~false
//~true
//~---
//~false
//~false
//~false
//~false
//~false
//~false
//~false
//~false

// Equality
package main

func main() {
	type newType struct {
		w [3]struct {
			a float64;
		}
		x [2][5]string
		y [5][7]rune
		z struct {
			a int;
			b float64;
		}
	}

	var a struct {
		a float64
	}
	var b struct {
		a float64
	}
	a.a = 2.
	b.a = 2.0

	var c [3]struct {
			a float64;
		}
	var d [3]struct {
			a float64;
		}
	c[2] = a;
	d[2] = b;

	var e [5]string
	var f [5] string
	e[0] = "hi"
	f[0] = "hi"

	var g [2][5]string
	var h [2][5]string
	g[0] = e
	h[0] = f

	var i [7]rune
	var j [7]rune
	i[1] = 'r'
	j[1] = 'r'

	var k [5][7]rune
	var l [5][7]rune
	k[0] = i
	l[0] = j

	var m struct {
			a int;
			b float64;
		}
	var n struct {
			a int;
			b float64;
		}
	m.a = 5
	n.a = 5

	var struct1 newType
	var struct2 newType
	struct1.w = c;
	struct2.w = d;
	struct1.x = g
	struct2.x = h
	struct1.y = k
	struct2.y = l
	struct1.z = m
	struct2.z = n


	println(a == b)
	println(c == d)
	println(e == f)
	println(g == h)
	println(i == j)
	println(k == l)
	println(m == n)
	println(struct1 == struct2)

	a.a = .31
	e[3] = "hello"
	i[1] = 's'
	m.b = 523.1

	println("---")

	println(a == b)
	println(c == d)
	println(e == f)
	println(g == h)
	println(i == j)
	println(k == l)
	println(m == n)
	println(struct1 == struct2)


	c[1] = a
	g[0] = e
	k[4] = i
	struct1.z = m

	println("---")

	println(a == b)
	println(c == d)
	println(e == f)
	println(g == h)
	println(i == j)
	println(k == l)
	println(m == n)
	println(struct1 == struct2)

}


