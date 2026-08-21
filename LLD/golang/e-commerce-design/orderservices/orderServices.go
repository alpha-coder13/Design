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
	orderStore[orderID] = order

	for _, val := range itemArray {
		itemStoreRWMutex.RLock()
		value, ok := itemStore[val]
		itemStoreRWMutex.RUnlock()
		if ok {
			orderID := value.GetOrderID()
			if orderID == 0 {
				order.AddItem(value)
			}
		}
	}
	return orderID
}

func CreateItemService(data string) (utils.IID, error) {
	newItem := new(OrderItem)
	err := json.Unmarshal([]byte(data), &newItem)
	if err != nil {
		return 0, errors.New("Unable to unmarshal the json data")
	}
	itemID := newItem.SetItemID()
	itemStore[itemID] = newItem
	return itemID, nil
}

func AddOrderItemService(orderID utils.OID) {
	// create new orderItem
}

func PostOrderService(orderID utils.OID) utils.OrderStatus {
	// create new orderItem
	var postStatus utils.OrderStatus
	orderStoreRWMutex.RLock()
	postStatus = orderStore[orderID].GetOrderStatus()
	orderStoreRWMutex.Unlock()

	if postStatus == utils.ORDER_WAITING {
		orderStoreRWMutex.Lock()
		cs := courierservices.GetAllCourierServices().GetBestCourierService(orderStore[orderID])
		orderStore[orderID].PostOrder(cs)
		postStatus = orderStore[orderID].GetOrderStatus()
		orderStoreRWMutex.Unlock()

	}
	return postStatus
}
