package com


import (
	"crypto/sha256"
	"errors"
	"fmt"
	"github.com/itchyny/base58-go"
	"math/big"
)

const ADDR_LEN = 20

type Address [ADDR_LEN]byte

var ADDRESS_EMPTY = Address{}

var ADDR_PREFIX = byte(23)

// ToBase58 returns base58 encoded address string
func (f *Address) ToBase58() string {
	fmt.Println(f[:])
	data := append([]byte{ADDR_PREFIX}, f[:]...)
	temp_1 := sha256.Sum256(data[:])
	temp_2 := sha256.Sum256(temp_1[:])

	fmt.Println(temp_2[0:4])
	data = append(data, temp_2[0:4]...)

	bi := new(big.Int).SetBytes(data).String()
	fmt.Println("bi : ",bi)

	encoded, _ := base58.BitcoinEncoding.Encode([]byte(bi))
	fmt.Println(encoded)

	return string(encoded)

}

// AddressParseFromBytes returns parsed Address
func AddressParseFromBytes(f []byte) (Address, error) {
	if len(f) != ADDR_LEN {
		return ADDRESS_EMPTY, errors.New("[Common]: AddressParseFromBytes err, len != 20")
	}

	var addr Address
	copy(addr[:], f)
	fmt.Println(f)

	fmt.Println("5")

	return addr, nil
}

