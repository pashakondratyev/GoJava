//~5
//~kac

//Reuse alias
package test

type paul struct {a int;}

type int struct {c string;}

type kab struct {a int;}

type kaylee struct {c []kaylee; d string;}

func main() {
	type alex struct {f int;}
	type alex2 struct {g float64;}
	type alex3 struct {h [5]kab; i [5][3]string; a [][]int; c [][]alex3; d [][3]string; e [3][]float64;}

	var kab paul
	kab.a = 5
	var a alex 
	a.f.c = "kac"
	println(kab.a)
	println(a.f.c)
	
}
