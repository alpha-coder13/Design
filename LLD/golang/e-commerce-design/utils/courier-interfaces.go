package utils

type CourierServiceExternalVendorInterface interface {
	PostOrder(order OrderInterface) (map[string]interface{}, error)
}
type CourierServiceInterface interface {
	PostOrder(order OrderInterface) (any, error)
	CalculatePriceTotal(order OrderInterface) int32
	CalculatePriceDistance(order OrderInterface) int32
	CalculatePriceVolume(order OrderInterface) int32
	CalculatePriceWeight(order OrderInterface) int32
	GetName() string
	// ProcessOrderSnapShot(PostOrderResponse)
}

type CourierServiceRegistryInterface interface {
	GetAllCourierServices() map[string]CourierServiceInterface
	AddCourierService(CourierServiceInterface)
	GetBestCourierService(OrderInterface) CourierServiceInterface
}
