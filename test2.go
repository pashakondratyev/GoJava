package test


func wag() {
	switch t:=10; {
	  case t>0:
		switch s:=5; {
		  case s>t:
			println("NONONO")
		  default:
			println("yes")
			break
		}
	  default:
		println("nonono")
	}
  }

func main(){
	wag()
}