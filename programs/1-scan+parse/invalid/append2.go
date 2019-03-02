// invalid append - appending multiple elements at once not allowed in GoLite
package test


func main() {
	var s []int
	s = append(s, 0, 5)
}
