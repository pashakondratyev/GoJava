// check equality of slices
package test


func equals(s1, s2 []int) bool {
	var result bool = true
	if cap(s1) != cap(s2) {
		result = false
	} else if len(s1) != len(s2) {
		result = false
	} else {
		for i:=0; i<len(s1) && i<len(s2); i++ {
			if int(s1[i]) != int(s2[i]) {
				result = false
				break
			}
		}
	}
	return result;
}


var s1, s2, s3 []int

s1 = append(s1, 5)
s2 = append(s2, 5)
s1 = append(s1, 7)
s2 = append(s2, 7)
s1 = append(s1, 4)
s2 = append(s2, 4)

s3 = append(s3, 2)

println(equals(s1,s2))
println(equals(s1,s3))