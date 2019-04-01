//~
//~compilers!
//~\a \b \f \n \r \t \v \\ \"

package main

func scan_rawstring() {
	var r string
	r = ``
	println(r)
	r = `compilers!`
	println(r)
	r = `\a \b \f \n \r \t \v \\ \"`
	println(r)
} 

func main() {
	scan_rawstring()
}
