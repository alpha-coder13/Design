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
	// ProcessOrderSnapShot(PostOrderResponse)
}

type CourierServiceRegistryInterface interface {
	AddCourierService(CourierServiceInterface)
	GetBestCourierService(OrderInterface) CourierServiceInterface
}
