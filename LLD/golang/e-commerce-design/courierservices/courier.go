package courierservices

import (
	"math"

	"github.com/alpha-coder13/Design/LLD/golang/e-commerce-design/utils"
)

type PostOrderResponse struct {
	totalCost       int32
	serviceResponse map[string]interface{}
}

type CourierSortStrategy func(csr map[string]utils.CourierServiceInterface, order utils.OrderInterface) utils.CourierServiceInterface

func pickCheapest(csr map[string]utils.CourierServiceInterface, order utils.OrderInterface, comp func(csi utils.CourierServiceInterface, order utils.OrderInterface) int32) utils.CourierServiceInterface {
	var res utils.CourierServiceInterface
	for _, v := range csr {
		if res == nil {
			res = v
		} else {
			curr := comp(res, order)
			nxt := comp(v, order)
			if curr > nxt {
				res = v
			}
		}
	}
	return res
}

var (
	// always the lowest Price
	SORT_BY_TOTAL_PRICE CourierSortStrategy = func(csr map[string]utils.CourierServiceInterface, order utils.OrderInterface) utils.CourierServiceInterface {
		return pickCheapest(csr, order, utils.CourierServiceInterface.CalculatePriceTotal)
	}
	SORT_BY_DISTANCE_PRICE CourierSortStrategy = func(csr map[string]utils.CourierServiceInterface, order utils.OrderInterface) utils.CourierServiceInterface {
		return pickCheapest(csr, order, utils.CourierServiceInterface.CalculatePriceDistance)
	}
	SORT_BY_WEIGHT_PRICE CourierSortStrategy = func(csr map[string]utils.CourierServiceInterface, order utils.OrderInterface) utils.CourierServiceInterface {
		return pickCheapest(csr, order, utils.CourierServiceInterface.CalculatePriceWeight)
	}
	SORT_BY_VOLUME_PRICE CourierSortStrategy = func(csr map[string]utils.CourierServiceInterface, order utils.OrderInterface) utils.CourierServiceInterface {
		return pickCheapest(csr, order, utils.CourierServiceInterface.CalculatePriceVolume)
	}
)

type CourierService struct {
	name          string
	courierVendor utils.CourierServiceExternalVendorInterface
	location      [2]float64
	distanceRate  int32
	volumeRate    int32
	weightRate    int32
}

func (csr *CourierService) CalculatePriceTotal(order utils.OrderInterface) int32 {
	distPrice := csr.CalculatePriceDistance(order)
	volPrice := csr.CalculatePriceVolume(order)
	wtPrice := csr.CalculatePriceWeight(order)
	return distPrice + volPrice + wtPrice
}
func (csr *CourierService) CalculatePriceDistance(order utils.OrderInterface) int32 {
	var tdp int32
	for _, val := range order.GetItems() {
		location := val.GetLocation()
		dist := math.Sqrt(math.Pow(location[0]-csr.location[0], 2) + math.Pow(location[1]-csr.location[1], 2))
		tdp += int32(dist * float64(csr.distanceRate))
	}
	return tdp
}
func (csr *CourierService) CalculatePriceVolume(order utils.OrderInterface) int32 {
	var tvp int32
	for _, val := range order.GetItems() {
		dimension := val.GetDimesions()
		vol := dimension.Width * dimension.Height * dimension.Breadth
		tvp += vol * csr.volumeRate
	}
	return tvp
}
func (csr *CourierService) CalculatePriceWeight(order utils.OrderInterface) int32 {
	var twp int32
	for _, val := range order.GetItems() {
		wt := val.GetWeight()
		twp += wt * csr.weightRate
	}
	return twp
}
func (csr *CourierService) PostOrder(order utils.OrderInterface) (PostOrderResponse, error) {
	var reponseStruct PostOrderResponse
	var totalPrice = csr.CalculatePriceTotal(order)
	//
	serviceResponse, err := csr.courierVendor.PostOrder(order)

	if err != nil {
		return reponseStruct, err
	}

	reponseStruct.serviceResponse = serviceResponse
	reponseStruct.totalCost = totalPrice
	return reponseStruct, nil
}
