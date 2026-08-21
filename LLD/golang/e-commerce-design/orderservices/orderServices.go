package orderservices

import (
	"encoding/json"
	"errors"
	"fmt"
	"sync"

	"github.com/alpha-coder13/Design/LLD/golang/e-commerce-design/courierservices"
	"github.com/alpha-coder13/Design/LLD/golang/e-commerce-design/utils"
)

func CreateOrderService(itemArray []utils.IID) utils.OID {
	order := new(Order)
	order.orderMtx = new(sync.RWMutex)
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

func AddOrderItemService(orderID utils.OID, itemId utils.IID) {
	// create new orderItem
	order, orderOk := orderStore[orderID]
	itemStoreRWMutex.RLock()
	value, itemOk := itemStore[itemId]
	itemStoreRWMutex.RUnlock()
	if itemOk && orderOk {
		success := order.AddItem(value)
		if success {
			fmt.Println("Item Added succesfully to orderID")
		} else {
			fmt.Println("Item is Already Added To different OrderID")
		}
	} else {
		if orderOk == false {
			fmt.Print("Not a valid orderID, Pass a valid Order Id")
		}
		if itemOk == false {
			fmt.Print("Not a valid ItemID, Pass a valid Item Id")
		}

	}
}

func PostOrderService(orderID utils.OID) utils.OrderStatus {
	// create new orderItem
	var postStatus utils.OrderStatus
	postStatus = orderStore[orderID].GetOrderStatus()

	if postStatus == utils.ORDER_WAITING {
		cs := courierservices.GetAllCourierServices().GetBestCourierService(orderStore[orderID])
		orderStore[orderID].PostOrder(cs)
		postStatus = orderStore[orderID].GetOrderStatus()
	}
	return postStatus
}

func UpdateOrderStatus(orderID utils.OID, status utils.OrderStatus) {
	err := orderStore[orderID].SetOrderStatus(status)

	if err == nil {
		// the order status os updated successfully
		return
	}

	// the orderstatus is being updated to the same value
}

func GetOrderStatus(orderID utils.OID) {
	fmt.Println(orderStore[orderID].GetOrderStatus())
}
