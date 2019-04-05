//~1
//~hello
//~hello
//~+1.000000e+000

package test

var i int
var s string
var b bool
var f float64

func main() {
  i := int(f)
  s := s
  var b = "hello"
  s = b
  f = float64(i)

  println(i)
  println(s)
  println(b)
  println(f)
}

func init() {
  i = 3
  s = "hi"
  b = false
  f = 1.1
}
