//~nice
//~wow!!
//~apple
//~this
//~yes
//~true true

package test

var x int = 12
var y int = 11

func b(m, n int) int {
  if m<n {
    return -1
  } else {
    return 1
  }
}

func foo() {
  switch y:=3; x/y {
    case 3:
      println("not nice")
    case 4:
      println("nice")
      x = 15
    case 5:
      println("wow!!")
    default:
      println("very bad")
  }
}

func bar() {
  switch z:=5; b(x,z) {
    case 0,1,2:
      println("apple")
    case 1,-1:
      println("banana")
  }
}

func baz() {
  switch x,y:=y,x; {
    case x<y:
      println("this")
    case y>x:
      println("that")
  }
}

func wag() {
  switch t:=10; {
    case t>0:
      switch s:=5; {
        case s>t:
          println("NONONO")
        default:
          println("yes")
          break
      }
    default:
      println("nonono")
  }
}

func buzz() {
  var m = 12
  switch m {
    case 10:
      var m = 1
      println(m)
    case 11:
      var m = "one"
      println(m)
    default:
      var m = 1.0
      println(m)
    case 12:
      switch m:=true; {
        case false, false:
          println(m)
        default:
          println(m,m)
      }
  }
}

func main() {
  foo()
  foo()
  bar()
  baz()
  wag()
  buzz()
}
