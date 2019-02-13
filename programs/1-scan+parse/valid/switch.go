// valid switch statement
package test

x := 6

switch x {
case 0,1,2,3: 
	println("small")
case 4,5,6,7: 
	println("large")
	fallthrough
case 6,7,8,9: 
	println("very large")
default:
	prinln("other")
}