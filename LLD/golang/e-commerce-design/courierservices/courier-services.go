package courierservices

import "github.com/alpha-coder13/Design/LLD/golang/e-commerce-design/utils"

func GetAllCourierServices() utils.CourierServiceRegistryInterface {
	return &courierRegistry
}
