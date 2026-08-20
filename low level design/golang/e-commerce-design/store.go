package ecommercedesign

import "sync"

var ItemStoreRWMutex sync.RWMutex
var ItemStore map[IID]struct {
	*OrderItem
	bool
}
var OrderStoreRWMutex sync.RWMutex

var OrderStore map[OID]*Order
var CourierRegistry CourierServiceRegistryInterface
