// invalid function declaration - no variable number parameters
package test

func Greeting(prefix string, who ...string) {
	println(string, " ", who)
}