// invalid continue - GoLite doesn't support labeled continues

x := 4
var label = "hi"

for {
	x--;
	if x==1 {
		continue label
	}
}