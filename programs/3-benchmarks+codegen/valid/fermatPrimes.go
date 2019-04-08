//&
// In Go, 4.629 sec.
// In Java, 4.775 sec.

//~2 prime
//~3 prime
//~4 composite
//~5 probably prime
//~6 composite
//~7 probably prime
//~8 composite
//~9 composite
//~10 composite
//~11 probably prime
//~12 composite
//~13 probably prime
//~14 composite
//~15 composite
//~16 composite
//~17 probably prime
//~18 composite
//~19 probably prime
//~20 composite
//~21 composite
//~22 composite
//~23 probably prime
//~24 composite
//~25 composite
//~26 composite
//~27 composite
//~28 composite
//~29 probably prime
//~30 composite
//~31 probably prime
//~32 composite
//~33 composite
//~34 composite
//~35 composite
//~36 composite
//~37 probably prime
//~38 composite
//~39 composite
//~40 composite
//~41 probably prime
//~42 composite
//~43 probably prime
//~44 composite
//~45 composite
//~46 composite
//~47 probably prime
//~48 composite
//~49 composite
//~50 composite
//~51 composite
//~52 composite
//~53 probably prime
//~54 composite
//~55 composite
//~56 composite
//~57 composite
//~58 composite
//~59 probably prime
//~60 composite
//~61 probably prime
//~62 composite
//~63 composite
//~64 composite
//~65 composite
//~66 composite
//~67 probably prime
//~68 composite
//~69 composite
//~70 composite
//~71 probably prime
//~72 composite
//~73 probably prime
//~74 composite
//~75 composite
//~76 composite
//~77 composite
//~78 composite
//~79 probably prime
//~80 composite
//~81 composite
//~82 composite
//~83 probably prime
//~84 composite
//~85 composite
//~86 composite
//~87 composite
//~88 composite
//~89 probably prime
//~90 composite
//~91 composite
//~92 composite
//~93 composite
//~94 composite
//~95 composite
//~96 composite
//~97 probably prime
//~98 composite
//~99 composite
//~100 composite
//~101 probably prime
//~102 composite
//~103 probably prime
//~104 composite
//~105 composite
//~106 composite
//~107 probably prime
//~108 composite
//~109 probably prime
//~110 composite
//~111 composite
//~112 composite
//~113 probably prime
//~114 composite
//~115 composite
//~116 composite
//~117 composite
//~118 composite
//~119 composite
//~120 composite
//~121 composite
//~122 composite
//~123 composite
//~124 composite


// Fermat Primality Test
// https://en.wikipedia.org/wiki/Fermat_primality_test
// n > 3, odd integer to be tested for primality
// k, the number of rounds of testing to perform
// output: "composite" if n is found to be composite, "probably prime" otherwise

package main


func pow(a, b, mod int) int {
	var value = 1
	a %= mod
	for b > 0 {
		// b is odd
		if (b%2)!=0 {
			value = (value * a) % mod
		}
		// b is even
		b >>= 1	// b = b/2
		a = (a*a) % mod

	}
	return value
}

// random number [low, high), low >=0
var a,c, seed int = 214903, 11, 75
func pseudoRand(low, high int) int {
	var result = -1
	for result < low {
		result = (a*seed) % high
		result += c
		result %= high
		seed = result
	}
	return result
}

func fermat(n, k int) string {
	for k > 0 {
		var rand = pseudoRand(2, n-1)
		var output = pow(rand, n-1, n)
		if output != 1 {
			return "composite"
		}
		k--
	}
	return "probably prime"
}

func main() {
	for i := 2; i < 125; i += 1 {
		switch {
			case i==2 || i==3:
				println(i, "prime")
			case i%2==0:
				println(i, "composite")
			default:
				println(i, fermat(i, 1000000))
		}
	}
}
