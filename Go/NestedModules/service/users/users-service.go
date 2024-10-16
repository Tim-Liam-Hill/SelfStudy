package usersService

import (
	"fmt"

	"example.com/service"
)

func Run(r service.Response) {
	fmt.Println("Inside usersService and received some data")
	fmt.Println(r.Data)
}
