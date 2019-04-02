package main

func for_stmts() {
	var a int
	var i int
	// Infinite loop
	for {
		i++ 
		if (i==2) {
			break
		}
	}

	// "while" loop
	for a < 10 {
		a++
	}

	// three-part loop, all parts missing
	for ;; {
		i++
		if i==5 {
			break
		}
	}

	// three-part loop, init statement only
	for a := 0; ; {
		i++
		if i==7 {
			break
		}
	}

	// three-part loop, expression only
	for ; a < 10; {
		a++
	}

	// three-part loop, update statement only
	for ; ; a++ {
		i++
		if i==10 {
			break
		}
	}

	// three-part loop, init statement and expression
	for a := 0; a < 10; {
		a++
	}

	// three-part loop, init and update statements
	for a := 10; ; a++ {
		if (a>15) {
			break
		}
	}

	a = 0
	// three-part loop, expr and update statements
	for ; a < 10; a++ {
	}

	// three-part loop, all parts
	for a := 0; a < 10; a++ {
	}
}
 
func main() {
	for_stmts()
}
