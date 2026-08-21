package utils

type OrderItemInterface interface {
	SetItemID() IID
	SetOrderID(OID)
	GetOrderID() OID
	GetLocation() [2]float64
	GetDimesions() Dimension
	GetWeight() int32
}

type OrderInterface interface {
	AddItem(OrderItemInterface) bool
	GetOrderID() OID
	GetItems() []OrderItemInterface
	GetOrderStatus() OrderStatus
	SetOrderStatus(OrderStatus)
	PostOrder(CourierServiceInterface)
}
