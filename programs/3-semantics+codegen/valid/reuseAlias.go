//~5
//~kac

//Reuse alias
package test

type paul struct {a int;}

type int struct {c string;}

type kab struct {a int;}

type kaylee struct {c []kaylee; d string;}

func main() {
  var p paul
  var k kab
  p.a = 5
  k.a.c = "kac"
  println(p.a)
  println(k.a.c)
}
