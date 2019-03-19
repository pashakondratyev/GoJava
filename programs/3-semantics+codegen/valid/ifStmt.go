//^hello
//^hi
//^hey
//^yup

package test


func main() {
	if x:=5; x+20<32 {
		println("hi")
	} else if x:=2; x<5 {
		println("hello")
	} else {
		println("hey")
	}

	if x:=5; x+20>32 {
		println("hi")
	} else if x:=2; x<5 {
		println("hello")
	} else {
		println("hey")
	}

	if x:=5; x+20<32 {
		println("hi")
	} else if x:=2; x>5 {
		println("hello")
	} else {
		println("hey")
	}
	yo();
}

func yo() {
	var x=12;
	if x,i:="hi",1; i<15 {
		var x = 15
		println("yup", x)
	} else {
		var x = 'r'
		println("nope")
	}
}