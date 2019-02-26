// valid comments
package test

var x = 7
/* block
comment
*/

func main(){
	/* block
	* comment
	*/

	if x == 2 {
		println("it is two")	// this is a valid comment
		println("or is it")		/* also a valid comment*/
	}

	// valid code shown in class
	var /*
		*/ a int;
}