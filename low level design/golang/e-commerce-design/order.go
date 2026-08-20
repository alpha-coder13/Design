package ecommercedesign

import (
	"encoding/json"
	"errors"
	"fmt"
)

type OrderItem struct {
	itemId       IID
	orderID      OID
	Dimension    Dimension `json:"dimen"`
	Weight       int32     `json:"wt"`
	dropLocation [2]float64
	// few other parameters
}

func (oi *OrderItem) setItemID() IID {
	currenItemValue := itemCounter.Load()
	for {
		if itemCounter.CompareAndSwap(currenItemValue, currenItemValue+1) {
			oi.itemId = IID(itemCounter.Load())
			break
		}
	}

	return oi.itemId
}

func (oi *OrderItem) setOrderID(orderID OID) {
	oi.orderID = orderID
}

func (oi *OrderItem) GetOrderID() OID {
	return oi.orderID
}

type Order struct {
	orderId        OID
	items          []OrderItem
	courierService CourierServiceInterface
	orderStatus    orderStatus
}

func (o *Order) assignCourierService() {
	cs := CourierRegistry.getBestCourierService(o)
	o.courierService = cs
}

func (o *Order) AddItem(oi *OrderItem) bool {
	o.items = append(o.items, *oi) // check for successful posting
	oi.orderID = o.orderId
	return true
}

func (o *Order) PostOrder() {
	o.orderStatus = ORDER_SHIPPED
	if o.courierService == nil {
		// error courier service not selected
		o.assignCourierService()
	}
	response, postOrderError := o.courierService.PostOrder(o)

	if postOrderError != nil {
		// orderPosting unsuccefull
		o.orderStatus = ORDER_FAILED
		return
	}

	fmt.Println(response) // will do something with response
	o.orderStatus = ORDER_TRANSIT
}

func (o *Order) getOrderID() OID {
	for {
		value := orderCounter.Load()
		if orderCounter.CompareAndSwap(value, value+1) {
			o.orderId = OID(value + 1)
			return o.orderId
		}
	}
}

func CreateOrderService(itemArray []IID) OID {
	// create a new order
	order := new(Order)
	order.orderStatus = ORDER_WAITING
	orderID := order.getOrderID()
	OrderStoreRWMutex.Lock()
	OrderStore[orderID] = order
	OrderStoreRWMutex.Unlock()

	for _, val := range itemArray {
		ItemStoreRWMutex.RLock()
		value, ok := ItemStore[val]
		ItemStoreRWMutex.RUnlock()
		if ok {
			if !value.bool {
				order.AddItem(value.OrderItem)
				value.bool = true
				ItemStoreRWMutex.Lock()
				ItemStore[val] = value
				ItemStoreRWMutex.Unlock()
			}
		}
	}
	return orderID
}

func CreateItemService(data string) (IID, error) {
	var newItem OrderItem
	err := json.Unmarshal([]byte(data), &newItem)
	if err != nil {
		return 0, errors.New("Unable to unmarshal the json data")
	}
	return newItem.setItemID(), nil
}

func AddOrderItemService(orderID OID) {
	// create new orderItem
}

func PostOrderService(orderID OID) orderStatus {
	// create new orderItem
	var postStatus orderStatus
	OrderStoreRWMutex.RLock()
	postStatus = OrderStore[orderID].orderStatus
	OrderStoreRWMutex.Unlock()

	if postStatus == ORDER_WAITING {
		OrderStoreRWMutex.Lock()
		OrderStore[orderID].PostOrder()
		postStatus = OrderStore[orderID].orderStatus
		OrderStoreRWMutex.Unlock()

	}
	return postStatus
}
