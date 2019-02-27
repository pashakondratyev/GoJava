// invalid short variable declaration in post statement
package test

func main() {
	for (i:=0; i<10; j:=12) {
		println("hi")
	}
}