// method to write the numbers 0-15 using a switch statement
package test

func printNumTo15(num int) {
	if num < 0 || num > 15 {
		println("Invalid input\n")
		return
	}
	switch num {
	case 0:
		println("zero")
		fallthrough
	case 1:
		println("one")
		fallthrough
	case 2:
		println("two")
		fallthrough
	case 3:
		println("three")
		fallthrough
	case 4:
		println("four")
		fallthrough
	case 5:
		println("five")
		fallthrough
	case 6:
		println("six")
		fallthrough
	case 7:
		println("seven")
		fallthrough
	case 8:
		println("eight")
		fallthrough
	case 9:
		println("nine")
		fallthrough
	case 10:
		println("ten")
		fallthrough
	case 11:
		println("eleven")
		fallthrough
	case 12:
		println("twelve")
		fallthrough
	case 13:
		println("thirteen")
		fallthrough
	case 14:
		println("fourteen")
		fallthrough
	case 15:
		println("fifteen")
	}
	println()
}

var w = 1;
var x = 7;
var y = -1;
var z = 4;

printNumTo15(w)
printNumTo15(x)
printNumTo15(y)
printNumTo15(z)