// invalid break - GoLite doesn't support labeled breaks

x := 4
var label = "hi"

for {
	x--;
	if x==1 {
		break label
	}
}