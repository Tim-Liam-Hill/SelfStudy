package service

import (
	"fmt"
)

func MyService() {
	fmt.Println("Inside my service. Yay!")
}

type Response struct {
	Data string
}
