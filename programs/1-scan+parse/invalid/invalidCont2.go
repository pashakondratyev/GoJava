// continue inside a switch statement
package test

func bar(){
  var i = 1
  switch i {
    case 1:
      continue
    case 2:
      return
  }
}
