// invalid struct literal in GoLite
package test

type point struct {
	x, y, z int
}

var p point = point{ 1, 2, 3 }