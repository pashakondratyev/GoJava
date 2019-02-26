// invalid continue - GoLite doesn't support labeled continues

x := 4
var label = "hi"

func main() {
	for {
		x--;
		if x==1 {
			continue label
		}
	}
}