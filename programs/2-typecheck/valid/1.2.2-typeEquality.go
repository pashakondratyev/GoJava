// valid type equality
package test

func main() {
	type num int

	// arrays
	var a [12]int
	var b [12]int
	var c [12]num
	a=b
	b=c
	c=a

	// slices
	var d []int
	var e []int
	var f []num
	d=e
	e=f
	f=d

	// struct
	type s1 struct { 
		a int
	 	b float64
	}
	type s2 struct{ 
		a int
		b float64
		
	}
	var x s1
	var y s2
	x=y

	// base types
	var g int
	var h int
	g=h

	var i float64
	var j float64
	i=j

	var k string
	var l string
	k=l

	var m rune
	var n rune
	m=n

	var o bool
	var p bool
	o=p
}