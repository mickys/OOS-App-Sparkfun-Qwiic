package main

import (
	// "fmt"
	"time"
	"encoding/json"

	"github.com/yosssi/gmq/mqtt"
	"github.com/yosssi/gmq/mqtt/client"
)

const (
	I2C_DEV_NUM = 0
)

func setup() {
	// device setup
	amg8833_setup()
}

func main() {
	// Create an MQTT Client.
	cli := client.New(&client.Options{
		// Define the processing of the error handler.
		ErrorHandler: func(err error) {
			panic(err)
		},
	})

	// Terminate the Client.
	defer cli.Terminate()

	// Connect to the MQTT Server.
	err := cli.Connect(&client.ConnectOptions{
		Network:  "tcp",
		Address:  "127.0.0.1:1883",
		ClientID: []byte("qwiic-client"),
	})
	if err != nil {
		panic(err)
	}

	// device setup
	setup()

	for {
		var pixels []float32 = amg8833_readPixels()
		// fmt.Println(pixels)
		jsonPixels, _ := json.Marshal(pixels)
		// fmt.Println(string(jsonPixels))

		// Publish a message.
		err = cli.Publish(&client.PublishOptions{
			QoS:       mqtt.QoS0,
			TopicName: []byte("/console/app-thermal"),
			Message:   []byte(string(jsonPixels)),
		})
		if err != nil {
			panic(err)
		}
		// fmt.Println("sent!")

		time.Sleep(100 * time.Millisecond)
	}
}
