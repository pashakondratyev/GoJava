// invalid for loop - GoLite doesn't support range loops
package test

for range [5]int{} {
	println("hi")
}