package main

import (
  "log"
  "fmt"
  "time"
	"github.com/d2r2/go-i2c"
)

const (
  AMG8833_I2C_ADDR = 0x69
  AMG8833_PCTL 	= 0x00
  AMG8833_RST 	= 0x01
  AMG8833_FPSC 	= 0x02
  AMG8833_INTC 	= 0x03
  AMG8833_PIXEL_TEMP_CONVERSION = .25
)

func privateHelperFunc() int {
    return 25
}

func amg8833_setup() {
  // Create new connection to I2C bus
  i2c, err := i2c.NewI2C(AMG8833_I2C_ADDR, I2C_DEV_NUM)
  if err != nil { log.Fatal(err) }
  // Free I2C connection on exit
  defer i2c.Close()

  // perform setup
  err = i2c.WriteRegU8(AMG8833_PCTL, 0x00)  // normal mode
  err = i2c.WriteRegU8(AMG8833_RST, 0x3f)   // software reset
  err = i2c.WriteRegU8(AMG8833_INTC, 0x00)  // disable interrupts
  err = i2c.WriteRegU8(AMG8833_FPSC, 0x00)  // 10 fps

  time.Sleep(100 * time.Millisecond)
}

func twoCompl12(val uint16) (float32) {
  if 0x7FF & val == val {
    return float32(val)
  } else {
    return float32(val)-float32(4096)
  }
}

func amg8833_readPixels() ([]float32) {
  // Create new connection to I2C bus
  i2c, err := i2c.NewI2C(AMG8833_I2C_ADDR, I2C_DEV_NUM)
  if err != nil { log.Fatal(err) }
  // Free I2C connection on exit
  defer i2c.Close()

  var raw uint16
  var val float32
  var buf []float32
  var offset uint8 = 128
  for i := uint8(0); i < 64; i += 2 {
    // fmt.Println("reading from reg", i+offset)
		raw, err = i2c.ReadRegU16LE(i+offset)
    fmt.Printf("read from 0x%02x: 0x%04x\n", i+offset, raw)
    val = twoCompl12(raw) * float32(AMG8833_PIXEL_TEMP_CONVERSION)
    buf = append(buf, val)
	}
  return buf
}
