//&
package main

func f(x float64) float64 {
	return x*(x-3.0)*(x+5.0)*(x-7.0)*(x+1237.0)*(x-577.0)*(x+691.0)
}

func abs(x float64) float64 {
	if (int(x) < 0) {
		return -x
	} else {
		return x
	}
}

func graddesc (a float64) float64 {
	next, gamma, precision, max_iter := a, 0.01, 0.0001, 100000
	for i:=1 ; i < max_iter; i++ {
		var curr = next
		next = curr - gamma * f(curr)
		step := next - curr
		if(abs(step) <= precision) {
			break
		}
		
	}
	return next
}


func main() {
	var a []float64
	for i:= -1500; i < 1500; i++ {
		a = append(a, float64(i));
	}
	for i:= 0; i < 3000; i++ {
		a[i] = graddesc(a[i])
	}
}