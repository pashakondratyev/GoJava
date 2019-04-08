//~97
//~-97
//~97
//~-97
//~a
//~a
//~122
//~-122
//~122
//~-122
//~z
//~z

package test

func main() {
  type char rune
	var c rune = 'a'
  var d rune = -c
  var s char = char(122)
  var t char = -s
  println(c)
  println(d)
  println(int(c))
  println(int(d))
  println(string(c))
  println(string(-d))
  println(s)
  println(t)
  println(int(s))
  println(int(t))
  println(string(s))
  println(string(-t))
}
