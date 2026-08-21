package orderservices

import (
	"encoding/json"
	"errors"

	"github.com/alpha-coder13/Design/LLD/golang/e-commerce-design/courierservices"
	"github.com/alpha-coder13/Design/LLD/golang/e-commerce-design/utils"
)

func CreateOrderService(itemArray []utils.IID) utils.OID {
	order := new(Order)
	order.SetOrderStatus(utils.ORDER_WAITING)
	orderID := order.GetOrderID()
	OrderStoreRWMutex.Lock()
	OrderStore[orderID] = order
	OrderStoreRWMutex.Unlock()

	for _, val := range itemArray {
		ItemStoreRWMutex.RLock()
		value, ok := ItemStore[val]
		ItemStoreRWMutex.RUnlock()
		if ok {
			if !value.bool {
				order.AddItem(value.OrderItemInterface)
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
	postStatus = OrderStore[orderID].GetOrderStatus()
	OrderStoreRWMutex.Unlock()

	if postStatus == utils.ORDER_WAITING {
		OrderStoreRWMutex.Lock()
		cs := courierservices.CourierRegistry.GetBestCourierService(OrderStore[orderID])
		OrderStore[orderID].PostOrder(cs)
		postStatus = OrderStore[orderID].GetOrderStatus()
		OrderStoreRWMutex.Unlock()

	}
	return postStatus
}
