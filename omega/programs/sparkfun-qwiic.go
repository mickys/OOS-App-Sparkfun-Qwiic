package main

import (
	"fmt"
)

const (
	I2C_DEV_NUM = 0
)

func setup() {
	amg8833_setup()
}

func main() {
	fmt.Println("Hello, world.", privateHelperFunc())

	setup()

	var pixels []float32 = amg8833_readPixels()
	fmt.Println(pixels)
}
