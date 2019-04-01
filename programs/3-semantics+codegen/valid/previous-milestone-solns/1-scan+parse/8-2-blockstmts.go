package main

func block_stmts() {
	var x int
	{
		var x int
	}
	var y int
	{
		{
		}
	}
} 

func main() {
	block_stmts()
}
