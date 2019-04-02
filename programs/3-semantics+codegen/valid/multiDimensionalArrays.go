//~9
//~0
//~2
//~0
//~+1.200000e+00
//~+2.600000e+00
//~+2.600000e+00
//~+1.200000e+00
//~+1.200000e+00
//~+2.600000e+00

package test

func main(){
	type point struct {x float64; y float64;}
	type line struct {points [2]point;}
	type shape struct {vertices []point;}

	var intList [5]int
	var intList2D [5][2]int
	var pointList [2]point
	var lineList [3]line
	var shapeList [1]shape

	intList[0] = 9
	intList2D[1][4] = 2
	pointList[0].x = 1.2
	pointList[0].y = 2.6
	pointList[1].x = pointList[0].y
	pointList[1].y = pointList[0].x
	lineList[0].points[0] = pointList[0]
	lineList[0].points[1] = pointList[1]
	shapeList[0].vertices = append(shapeList[0].vertices, pointList[0])

  println(intList[0])
  println(intList[1])
  println(intList2D[1][4])
  println(intList2D[0][0])
  println(pointList[0].x)
	println(pointList[0].y)
  println(lineList[0].points[1].x)
	println(lineList[0].points[1].y)
  println(shapeList[0].vertices[0])
}
