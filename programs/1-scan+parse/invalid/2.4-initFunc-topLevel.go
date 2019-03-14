// init function - only top-level scope

func f() {
	func init() {
		println("hello")
	}
}