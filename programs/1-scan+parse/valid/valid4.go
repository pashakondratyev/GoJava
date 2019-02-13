// bubble sort a slice
package test

func sweep(nums []int) {
	var firstIndex int = 0
	var secondIndex int = 1
	for secondIndex < len(nums) {
		var firstNum int = nums[firstIndex]
		var secondNum int = nums[secondIndex]

		if firstNum > secondNum {
			nums[firstIndex] = secondNum
			nums[secondIndex] = firstNum
		}

		firstIndex++
		secondIndex++
	}
}

func bubbleSort(nums []int) {
	for i:=0; i<len(nums); i++ {
		sweep(nums)	
	}
}

func printSlice(nums []int) {
	for i:=0; i<len(nums); i++ {
		print(nums[i])
		print(" ")
	}
	println()
}

var nums []int
nums = append(nums,12)
nums = append(nums, 75)
nums = append(nums, 23)
nums = append(nums, 84)
nums = append(nums, 12)
printSlice(nums)
bubbleSort(nums)
printSlice(nums)

