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
		break
	case 1:
		println("one")
		break
	case 2:
		println("two")
		break
	case 3:
		println("three")
		break
	case 4:
		println("four")
		break
	case 5:
		println("five")
		break
	case 6:
		println("six")
		break
	case 7:
		println("seven")
		break
	case 8:
		println("eight")
		break
	case 9:
		println("nine")
		break
	case 10:
		println("ten")
		break
	case 11:
		println("eleven")
		break
	case 12:
		println("twelve")
		break
	case 13:
		println("thirteen")
		break
	case 14:
		println("fourteen")
		break
	case 15:
		println("fifteen")
	}
	println()
}

var w = 1;
var x = 7;
var y = -1;
var z = 4;

func main(){
	printNumTo15(w)
	printNumTo15(x)
	printNumTo15(y)
	printNumTo15(z)
}