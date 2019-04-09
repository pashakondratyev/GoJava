//&
//~Mean: +1.302120e+003
//~Max: +9.071082e+003

//Generate random matrices and perform Gaussian elimination on each of them
//Compute growth factor each time and output mean growth factor and maximum growth factor
//Growth factor is defined as highest value in the matrix in absolute value
package main

var seed int = 420
var u,v int = 3103013, 17
var top int = 50

func randomFloat() float64 {
	var frac [2]int
	frac[0] = -1
	frac[1] = -1
	for i:=0; i<2; i++ {
		frac[i] = 0
		for frac[i]<=0 {
			frac[i] = (u*seed)%top
			frac[i] += v
			frac[i] %= top
			seed = frac[i]
		}
	}
	return float64(frac[0])/float64(frac[1])

}

func ge(m [8][8]float64) [8][8]float64 {
	for i:=0; i<len(m); i++ {
		var pivot = m[i][i]
		for j:=i+1; j<len(m); j++ {
			var mult = m[j][i]/pivot
			for k:=i; k<len(m); k++ {
				m[j][k] = m[j][k]-mult*m[i][k]
			}
		}
	}
	return m
}

func maxMatrix(m [8][8]float64) float64 {
	var max = m[0][0]
	if max<0.0 {
		max = -max
	}
	for i:=0; i<len(m); i++ {
		for j:=0; j<len(m); j++ {
			if m[i][j]>max {
				max = m[i][j]
			} else if -m[i][j]>max {
				max = -m[i][j]
			}
		}
	}
	return max
}

func printMatrix(m [8][8]float64) {
	for i:=0; i<len(m); i++ {
		for j:=0; j<len(m); j++ {
			print(m[i][j])
			print(" ")
		}
		println()
	}
}

func randomMatrix() [8][8]float64 {
	var m [8][8]float64
	for i:=0; i<len(m); i++ {
		for j:=0; j<len(m); j++ {
			m[i][j] = randomFloat()
		}
	}
	return m
}

func main() {
	var n = 1000000
	var mean = 0.0
	var max = 0.0
	for i:=0; i<n; i++ {
		var m = randomMatrix()
		m = ge(m)
		var x = maxMatrix(m)
		if x>max {
			max=x
		}
		mean = mean+(x/float64(n))
	}
	println("Mean:",mean)
	println("Max:",max)
}


