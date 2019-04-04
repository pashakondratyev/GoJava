package main

func breakcontinue_stmts() {
	// break loop
	for {
		break
	}

	// break switch
	switch {
		default:
			break
	}

	var i = 0
	// continue loop
	for {
		i++
		if i==10 {
			break
		}
		continue
	}
} 

func main() {
	breakcontinue_stmts()
}
