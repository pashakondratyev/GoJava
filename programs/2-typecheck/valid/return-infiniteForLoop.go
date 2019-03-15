//valid non-void function declarations with infinite for loops
package test

func yolo() int {
  for {
    for {
      break
    }
  }
}

func swag() int {
  for {
    for {
      if True {
        break
      }
    }
  }
}
