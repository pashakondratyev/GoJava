// vector manipulation
package test

type Vector struct {
	x, y, z int
}

func createVector(x, y, z int) Vector {
	var v Vector
	v.x = x
	v.y = y
	v.z = z
	return v
} 

func addVectors(v1, v2 Vector) Vector {
	var v Vector
	v.x = v1.x + v2.x
	v.y = v1.y + v2.y
	v.z = v1.z + v2.z
	return v
}

func multiplyVector(v1 Vector, multiple int) Vector {
	var v Vector
	v.x = v1.x * multiple
	v.y = v1.y * multiple
	v.z = v1.z * multiple
	return v
}

func equal(v1, v2 Vector) bool {
	var isEqual = false
	if v1.x==v2.x && v1.y==v2.y && v1.z==v2.z {
		isEqual = true
	}
	return isEqual
}


var v1 = createVector(1,2,3)
var v2 = createVector(2,4,6)
var v3 = addVectors(v1, v2)
var v4 = multiplyVector(v1,2)
println(equal(v2, v4))
println(equal(v1, v3))

