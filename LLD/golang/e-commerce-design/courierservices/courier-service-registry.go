package courierservices

import (
	"fmt"

	"github.com/alpha-coder13/Design/LLD/golang/e-commerce-design/utils"
)

var CourierRegistry = CourierServiceRegistry{}

type CourierServiceRegistry struct {
	utils.CourierServiceRegistryInterface
	cs []utils.CourierServiceInterface
}

func (csr *CourierServiceRegistry) AddCourierService(cs utils.CourierServiceInterface) {
	if csr.cs == nil {
		csr.cs = make([]utils.CourierServiceInterface, 0)
	}
	csr.cs = append(csr.cs, cs)
}

func (csr *CourierServiceRegistry) GetBestCourierService(order utils.OrderInterface) utils.CourierServiceInterface {
	for {

		fmt.Println(`
		Select GetBestCourier Strategy
			\n\t1. low price
			\n\t2. low overall
			\n\t3. low distance rate
			\n\t4. low weight rate
			\n\t5. low volume rate
			\n
		`)
		var input int
		fmt.Scanf("%d", input)
		var sortStrategy CourierSortStrategy

		switch input {
		case 1:
			sortStrategy = SORT_BY_TOTAL_PRICE
		case 2:
			sortStrategy = SORT_BY_TOTAL_PRICE
		case 3:
			sortStrategy = SORT_BY_DISTANCE_PRICE
		case 4:
			sortStrategy = SORT_BY_WEIGHT_PRICE
		case 5:
			sortStrategy = SORT_BY_VOLUME_PRICE
		default:
			fmt.Println("Please Enter a valid input")
			continue
		}
		courierService := sortStrategy(csr.cs, order)
		return courierService
	}
}
