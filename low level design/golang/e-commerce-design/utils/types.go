package utils

type IID uint64
type OID uint64
type Dimension struct {
	Height  int32 `json:"h"`
	Breadth int32 `json:"b"`
	Width   int32 `json:"w"`
}

type OrderStatus int

const (
	ORDER_WAITING OrderStatus = iota
	ORDER_TRANSIT
	ORDER_SHIPPED
	ORDER_FAILED
)
