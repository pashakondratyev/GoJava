//~
//~compilers!
//~ 
//~ 
//~ 	   \ "

package main

func scan_interstring() {
	var s string
	s = ""
	println(s)
	s = "compilers!"
	println(s)
	s = "\a \b \f \n \r \t \v \\ \""
	println(s)
} 

func main() {
	scan_interstring()
}
