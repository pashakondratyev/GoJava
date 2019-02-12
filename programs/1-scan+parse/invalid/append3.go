// invalid append - appending multiple elements at once not allowed in GoLite
package test

var s []int
s = append(s, 0, 5)