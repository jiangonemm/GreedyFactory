package main

import (
	"errors"
	"fmt"
	"github.com/itchyny/base58-go"
	"math/big"
	"./com"
)


const ADDR_LEN = 20

var ADDRESS_EMPTY = com.Address{}

var ADDR_PREFIX = byte(80)

func main() {
	fromAdrr, _ := ConvertAddressFromBase58("ZCc6Mz2uuoB5dokNdWhHYFaNot7TXVxNw7")
	fmt.Println(fromAdrr.ToBase58())
}

// AddressFromBase58 returns Address from encoded base58 string
func ConvertAddressFromBase58(encoded string) (com.Address, error) {

	if encoded == "" {

		return ADDRESS_EMPTY, fmt.Errorf("invalid address")
	}

	decoded, err := base58.BitcoinEncoding.Decode([]byte(encoded))

	if err != nil {

		return ADDRESS_EMPTY, err
	}


	x, ok := new(big.Int).SetString(string(decoded), 10)
	if !ok {

		return ADDRESS_EMPTY, fmt.Errorf("invalid address")
	}

	buf := x.Bytes()
	if len(buf) != 1+ADDR_LEN+4 || buf[0] != byte(ADDR_PREFIX) {

		return ADDRESS_EMPTY, errors.New("wrong encoded address")
	}
	ph, err := com.AddressParseFromBytes(buf[1:21])

	if err != nil {

		return ADDRESS_EMPTY, err
	}

	addr := ph.ToBase58()

	fmt.Println(addr)

	return ph, nil
}
