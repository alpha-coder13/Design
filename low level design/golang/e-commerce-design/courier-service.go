package ecommercedesign

import (
	"fmt"
	"math"
)

type PostOrderResponse struct {
	totalCost       int32
	serviceResponse map[string]interface{}
}

type CourierSortStrategy func(csr []CourierServiceInterface, order *Order) CourierServiceInterface

func pickCheapest(csr []CourierServiceInterface, order *Order, comp func(csi CourierServiceInterface, order *Order) int32) CourierServiceInterface {
	var res CourierServiceInterface
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
	SORT_BY_TOTAL_PRICE CourierSortStrategy = func(csr []CourierServiceInterface, order *Order) CourierServiceInterface {
		return pickCheapest(csr, order, CourierServiceInterface.calculatePriceTotal)
	}
	SORT_BY_DISTANCE_PRICE CourierSortStrategy = func(csr []CourierServiceInterface, order *Order) CourierServiceInterface {
		return pickCheapest(csr, order, CourierServiceInterface.calculatePriceDistance)
	}
	SORT_BY_WEIGHT_PRICE CourierSortStrategy = func(csr []CourierServiceInterface, order *Order) CourierServiceInterface {
		return pickCheapest(csr, order, CourierServiceInterface.calculatePriceWeight)
	}
	SORT_BY_VOLUME_PRICE CourierSortStrategy = func(csr []CourierServiceInterface, order *Order) CourierServiceInterface {
		return pickCheapest(csr, order, CourierServiceInterface.calculatePriceVolume)
	}
)

type CourierServiceExternalVendorInterface interface {
	postOrder(order *Order) (map[string]interface{}, error)
}
type CourierServiceInterface interface {
	PostOrder(order *Order) (PostOrderResponse, error)
	calculatePriceTotal(order *Order) int32
	calculatePriceDistance(order *Order) int32
	calculatePriceVolume(order *Order) int32
	calculatePriceWeight(order *Order) int32
	// ProcessOrderSnapShot(PostOrderResponse)
}

type CourierService struct {
	name          string
	courierVendor CourierServiceExternalVendorInterface
	location      [2]float64
	distanceRate  int32
	volumeRate    int32
	weightRate    int32
}

func (csr *CourierService) calculatePriceTotal(order *Order) int32 {
	distPrice := csr.calculatePriceDistance(order)
	volPrice := csr.calculatePriceVolume(order)
	wtPrice := csr.calculatePriceVolume(order)
	return distPrice + volPrice + wtPrice
}
func (csr *CourierService) calculatePriceDistance(order *Order) int32 {
	var tdp int32
	for _, val := range order.items {
		dist := math.Sqrt(math.Pow(val.dropLocation[0]-csr.location[0], 2) + math.Pow(val.dropLocation[1]-csr.location[1], 2))
		tdp += int32(dist * float64(csr.distanceRate))
	}
	return tdp
}
func (csr *CourierService) calculatePriceVolume(order *Order) int32 {
	var tvp int32
	for _, val := range order.items {
		vol := val.Dimension.Width * val.Dimension.Height * val.Dimension.Breadth
		tvp += vol * csr.volumeRate
	}
	return tvp
}
func (csr *CourierService) calculatePriceWeight(order *Order) int32 {
	var twp int32
	for _, val := range order.items {
		twp += val.Weight * csr.weightRate
	}
	return twp
}
func (csr *CourierService) PostOrder(order *Order) (PostOrderResponse, error) {
	var reponseStruct PostOrderResponse
	var totalPrice = csr.calculatePriceTotal(order)
	//
	serviceResponse, err := csr.courierVendor.postOrder(order)

	if err != nil {
		return reponseStruct, err
	}

	reponseStruct.serviceResponse = serviceResponse
	reponseStruct.totalCost = totalPrice
	return reponseStruct, nil
}

type CourierServiceRegistryInterface interface {
	AddCourierService(CourierServiceInterface)
	getBestCourierService(*Order) CourierServiceInterface
}

type CourierServiceRegistry []CourierServiceInterface

func (csr *CourierServiceRegistry) AddCourierService(cs CourierServiceInterface) {
	(*csr) = append((*csr), cs)
}

func (csr *CourierServiceRegistry) getBestCourierService(order *Order) CourierServiceInterface {
	for {

		fmt.Println(`
		Select GetBestCourier Strategy
			\n\t1. low price
			\n\t2. low overall
			\n\t3. low distance rate
			\n\t4. low weight rate
			\n\t5. low volume rate
			\n
		`)
		var input int
		fmt.Scanf("%d", input)
		var sortStrategy CourierSortStrategy

		switch input {
		case 1:
			sortStrategy = SORT_BY_TOTAL_PRICE
		case 2:
			sortStrategy = SORT_BY_TOTAL_PRICE
		case 3:
			sortStrategy = SORT_BY_DISTANCE_PRICE
		case 4:
			sortStrategy = SORT_BY_WEIGHT_PRICE
		case 5:
			sortStrategy = SORT_BY_VOLUME_PRICE
		default:
			fmt.Println("Please Enter a valid input")
			continue
		}
		courierService := sortStrategy(*csr, order)
		return courierService
	}
}
