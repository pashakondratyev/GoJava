// invalid break - GoLite doesn't support labeled breaks

x := 4
var label = "hi"

func main() {
	for {
		x--;
		if x==1 {
			break label
		}
	}
}