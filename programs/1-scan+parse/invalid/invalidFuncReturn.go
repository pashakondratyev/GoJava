// not guaranteed to reach return statement in non-void function
package test

func foo(bool x) int {
  if x {
    return 3
  }
}
