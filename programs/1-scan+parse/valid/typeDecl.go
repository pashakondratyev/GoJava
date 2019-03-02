// valid type declarations
package test

type natural int
type real float64

type (
	point struct {
		x, y, z real
	}
)

type (
	num int
	point struct {
	x, y float64
	}
)