//^hello
//^hi
//^hey
//^yup
//^<=2
//^=2
//^>=2

package test


func main() {

	// if statements used to check for correct branch evaluation
	if x:=5; x<32 {
		println("hi")
	} else if x:=2; x<5 {
		println("hello")
	} else {
		println("hey")
	}

	if x:=5; x>32 {
		println("hi")
	} else if x:=2; x<5 {
		println("hello")
	} else {
		println("hey")
	}

	if x:=5; x>32 {
		println("hi")
	} else if x:=2; x>5 {
		println("hello")
	} else {
		println("hey")
	}

	foo();
	bar();

}

// if statement with shadowed x variable in all scopes
func foo() {
	var x=12;
	if x,i:="hi",1; i<15 {
		var x = 15
		println("yup", x)
	} else {
		var x = 'r'
		println("nope")
	}
}

// basic if statements without init statement
func bar() {
	var i = 2
	if i<=2 {
		println("<=2")
	}

	if i!=2 {
		println("not 2")
	} else {
		println("is 2")
	}
}