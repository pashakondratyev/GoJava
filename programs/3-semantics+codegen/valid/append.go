//^Cap: 0 , len: 0
//^Cap: 2 , len: 1
//^Cap: 2 , len: 2
//^Cap: 4 , len: 3
//^Cap: 4 , len: 4
//^Cap: 8 , len: 5
//^Cap: 8 , len: 6
//^Cap: 8 , len: 7
//^Cap: 8 , len: 8
//^Cap: 16 , len: 9
//^a: len= 2 , cap= 2
//^b: len= 1 , cap= 2
//^aa: len= 2 , cap= 2
//^bb: len= 2 , cap= 2
//^1 1
//^13 0

// Append
package main

func main () {
	var z []int
	for i := 0; i < 10; i ++ {
		println ("Cap:", cap(z), ", len:", len(z))
		z = append(z,0)
	}

	var a, b []int
	a = append(a, 0)
	b = a
	a = append (a , 1)
	println("a: len=", len(a), ", cap=", cap(a))
	println("b: len=", len(b), ", cap=", cap(b))

	var aa, bb []int
	aa = append(aa, 0)
	bb = aa
	aa = append(aa, 1)
	bb = append(bb, 2)
	println("aa: len=", len(aa), ", cap=", cap(aa))
	println("bb: len=", len(bb), ", cap=", cap(bb))

	var aaa, bbb []int
	aaa = append(aaa, 0)
	bbb = aaa
	aaa = append(aaa, 13)
	aaa[0] = 1
	println(aaa[0], bbb[0])


	var aaaa, bbbb []int
	aaaa = append(aaaa, 0)
	aaaa = append(aaaa, 1)
	bbbb = aaaa
	aaaa = append(aaaa, 2)
	aaaa[0] = 13
	println(aaaa[0], bbbb[0])
}