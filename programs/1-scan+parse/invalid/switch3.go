// invalid switch - multiple default cases
package test

var x = 7
y:=8
switch x, y {
	case 0,1,2,3,4 : print("small")
	default: print("other")
}