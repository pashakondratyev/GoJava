// find first unique character in a string
package test

func foundDuplicate(str string, curIndex int) bool {
	var found = false
	for i:= 0; i<len(str); i++ {
		if curIndex != i && string(str[curIndex]) == string(str[i]) {
			found = true
			break
		}
	}
	return found
} 

func getFirstUniqueChar(str string) string {
	var output = " "

	var allChars []string;
	for i:=0; i<len(str); i++ {
		allChars = append(allChars, string(str[i]))
	}

	for i:=0; i<len(str); i++ {
		if foundDuplicate(str, i) == false {
			output = string(str[i])
			break
		}
	}
	return output
}

func main(){
	println(getFirstUniqueChar("ccoommpp520"))
	println(getFirstUniqueChar("comp520"))
}