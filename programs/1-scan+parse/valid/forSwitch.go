// valid break and continue statements
package test

func wow() {
  for true {
    switch 5 {
      case 1:
        continue
      default:
        break
    }
  }
}
