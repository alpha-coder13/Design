package orderservices

import (
	"encoding/json"
	"errors"
	"fmt"
	"sync"

	courierservice "github.com/alpha-coder13/Design/courierservices"
	"github.com/alpha-coder13/Design/utils"
)

var ItemStoreRWMutex sync.RWMutex
var ItemStore map[utils.IID]struct {
	*OrderItem
	bool
}
var OrderStoreRWMutex sync.RWMutex

var OrderStore map[utils.OID]*Order

type OrderItem struct {
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

func (oi *OrderItem) setOrderID(orderID utils.OID) {
	oi.orderID = orderID
}

func (oi *OrderItem) GetOrderID() utils.OID {
	return oi.orderID
}

type Order struct {
	orderId        utils.OID
	Items          []OrderItem
	courierService courierservice.CourierServiceInterface
	orderStatus    utils.OrderStatus
}

func (o *Order) assignCourierService() {
	cs := courierservice.CourierRegistry.getBestCourierService(o)
	o.courierService = cs
}

func (o *Order) AddItem(oi *OrderItem) bool {
	o.Items = append(o.Items, *oi) // check for successful posting
	oi.orderID = o.orderId
	return true
}

func (o *Order) PostOrder() {
	o.orderStatus = utils.ORDER_SHIPPED
	if o.courierService == nil {
		// error courier service not selected
		o.assignCourierService()
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

func (o *Order) getOrderID() utils.OID {
	for {
		value := utils.OrderCounter.Load()
		if utils.OrderCounter.CompareAndSwap(value, value+1) {
			o.orderId = utils.OID(value + 1)
			return o.orderId
		}
	}
}

func CreateOrderService(itemArray []utils.IID) utils.OID {
	// create a new order
	order := new(Order)
	order.orderStatus = utils.ORDER_WAITING
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

func CreateItemService(data string) (utils.IID, error) {
	var newItem OrderItem
	err := json.Unmarshal([]byte(data), &newItem)
	if err != nil {
		return 0, errors.New("Unable to unmarshal the json data")
	}
	return newItem.setItemID(), nil
}

func AddOrderItemService(orderID utils.OID) {
	// create new orderItem
}

func PostOrderService(orderID utils.OID) utils.OrderStatus {
	// create new orderItem
	var postStatus utils.OrderStatus
	OrderStoreRWMutex.RLock()
	postStatus = OrderStore[orderID].orderStatus
	OrderStoreRWMutex.Unlock()

	if postStatus == utils.ORDER_WAITING {
		OrderStoreRWMutex.Lock()
		OrderStore[orderID].PostOrder()
		postStatus = OrderStore[orderID].orderStatus
		OrderStoreRWMutex.Unlock()

	}
	return postStatus
}
