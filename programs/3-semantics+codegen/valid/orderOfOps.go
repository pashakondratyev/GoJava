//~init 1
//~init 2
//~init 3
//~init 4
//~main
//~foo
//~bar
//~baz

// order of execution - functions
package main

func baz() {
	println("baz")
}

func init() {
	println("init 1")
}

func foo() {
	println("foo")
}

func init() {
	println("init 2")
}

func bar() {
	println("bar")
}

func hi() {
	println("never called")
}

func main() {
	println("main")
	foo()
	bar()
	baz()
}

func init() {
	println("init 3")
}

func init() {
	println("init 4")
}