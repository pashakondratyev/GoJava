//~true
//~false
//~false
//~false
//~true
//~false
//~false
//~true
//~false
//~true
//~false
//~true
//~false
//~false
//~false
//~false
//~true
//~true
//~false
//~true
//~--------------------------------
//~false
//~false
//~false
//~true
//~false
//~true
//~false
//~true
//~false
//~true
//~true
//~true
//~--------------------------------
//~false
//~false
//~false
//~true
//~false
//~true
//~false
//~true
//~false
//~false
//~true
//~false
//~true
//~false
//~--------------------------------
//~true
//~false
//~true
//~false
//~true
//~true
//~false
//~--------------------------------
//~true
//~false
//~false
//~true
//~false


// Expressions
package main

func main () {
	// Integer
	var c = 5;
	var d = 9;
	println((7&^2) == 5)
	println((c-d) == 5)
	println((c*d) == 5)
	println((c/d) == 5)
	println((d%c) == 4)
	println((c&d) == 5)
	println((c|d) == 5)
	println((c==d) == false)
	println((c!=d) == false)
	println((c>=d) == false)
	println((c<=d) == false)
	println((c>d) == false)
	println((c<d) == false)
	println((c^d) == 5)
	println((c<<d) == 5)
	println((c>>d) == 5)
	var e = -8
	println((+e) == -8)
	println((-e) == 8)
	println((^c) == 2)
	var f = (e)
	println((f) == -8)

	println("--------------------------------")

	// Double
	var cc = 5.0;
	var dd = 9.0;
	println((cc-dd) == 5.0)
	println((cc*dd) == 5.0)
	println((cc/dd) == 5.0)
	println((cc==dd) == false)
	println((cc!=dd) == false)
	println((cc>=dd) == false)
	println((cc<=dd) == false)
	println((cc>dd) == false)
	println((cc<dd) == false)
	var ee = -8.0
	println((+ee) == -8.0)
	println((-ee) == 8.0)
	var ff = (ee)
	println((ff) == -8.0)

	println("--------------------------------")

	// Rune
	var g = 'r';
	var h = 's';
	println((g-h) == 'a')
	println((g*h) == 'a')
	println((g/h) == 'a')
	println((g==h) == false)
	println((g!=h) == false)
	println((g>=h) == false)
	println((g<=h) == false)
	println((g>h) == false)
	println((g<h) == false)
	println((g^h) == 'a')
	var i = 'a'
	println((+i) == 'a')
	println((-i) == 'a')
	var j = (i)
	println((j) == 'a')
	println((^i) == 'a')


	println("--------------------------------")

	// String
	var a string = "apple"
	var b string = "Apple"

	println((a+b) == "appleApple")
	println((a>=b) == false)
	println((a<=b) == false)
	println((a>b) == false)
	println((a<b) == false)
	println((a==b) == false)
	println((a!=b) == false)


	println("--------------------------------")

	// Boolean
	println((true && false) == false)
	println((true || false) == false)
	println((!true) == true)
	println((true==false) == false)
	println((true!=false) == false)

	
}