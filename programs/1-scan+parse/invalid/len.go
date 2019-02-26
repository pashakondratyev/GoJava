// invalid len - missing parentheses
package test


func main() {
	var s []int
	s = append(s, 0)
	var x = len s
}