package orderservices

import (
	"fmt"
	"sync"

	"github.com/alpha-coder13/Design/LLD/golang/e-commerce-design/utils"
)

var ItemStoreRWMutex sync.RWMutex
var ItemStore map[utils.IID]struct {
	*OrderItem
	bool
}
var OrderStoreRWMutex sync.RWMutex

var OrderStore map[utils.OID]*Order

type OrderItem struct {
	utils.OrderItemInterface
	itemId       utils.IID
	orderID      utils.OID
	Dimension    utils.Dimension `json:"dimen"`
	Weight       int32           `json:"wt"`
	DropLocation [2]float64
	// few other parameters
}

func (oi *OrderItem) setItemID() utils.IID {
	currenItemValue := utils.ItemCounter.Load()
	for {
		if utils.ItemCounter.CompareAndSwap(currenItemValue, currenItemValue+1) {
			oi.itemId = utils.IID(utils.ItemCounter.Load())
			break
		}
	}

	return oi.itemId
}

func (oi *OrderItem) SetOrderID(orderID utils.OID) {
	oi.orderID = orderID
}

func (oi *OrderItem) GetOrderID() utils.OID {
	return oi.orderID
}

type Order struct {
	utils.OrderInterface
	orderId        utils.OID
	Items          []utils.OrderItemInterface
	courierService utils.CourierServiceInterface
	orderStatus    utils.OrderStatus
}

func (o *Order) assignCourierService(cs utils.CourierServiceInterface) {
	o.courierService = cs
}

func (o *Order) AddItem(oi utils.OrderItemInterface) bool {
	o.Items = append(o.Items, oi) // check for successful posting
	oi.SetOrderID(o.orderId)
	return true
}

func (o *Order) PostOrder(cs utils.CourierServiceInterface) {
	o.orderStatus = utils.ORDER_SHIPPED
	if o.courierService == nil {
		// error courier service not selected
		o.assignCourierService(cs)
	}
	response, postOrderError := o.courierService.PostOrder(o)

	if postOrderError != nil {
		// orderPosting unsuccefull
		o.orderStatus = utils.ORDER_FAILED
		return
	}

	fmt.Println(response) // will do something with response
	o.orderStatus = utils.ORDER_TRANSIT
}

func (o *Order) GetOrderID() utils.OID {
	for {
		value := utils.OrderCounter.Load()
		if utils.OrderCounter.CompareAndSwap(value, value+1) {
			o.orderId = utils.OID(value + 1)
			return o.orderId
		}
	}
}
