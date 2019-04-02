//~2.600000e00
//~1.200000e00

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

	println(pointList[0].y)
	println(lineList[0].points[0].x)
}
