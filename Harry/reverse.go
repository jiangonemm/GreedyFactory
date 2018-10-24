package main

import (
	"crypto/sha256"

	"fmt"
	"github.com/itchyny/base58-go"
	"math/big"
)

const ADDR_LEN = 20

type Address [ADDR_LEN]byte

var ADDRESS_EMPTY = Address{}

var ADDR_PREFIX = byte(80)

var ADDR_PREFIX_1 = byte(23)


func main() {
	var encoded string
	fmt.Scan(&encoded)
	fromAdrr, _ := ConvertAddressFromBase58(encoded)
	fmt.Println(fromAdrr.ToBase58())
}

func ConvertAddressFromBase58(encoded string) (Address, error) {
    e := fmt.Errorf("输入错误")
	if encoded == "" {
		panic(e)
	}

	decoded, err := base58.BitcoinEncoding.Decode([]byte(encoded))
	if err != nil {
		panic(e)
	}

	x, ok := new(big.Int).SetString(string(decoded), 10)
	if !ok {
		panic(e)
	}

	buf := x.Bytes()
	if len(buf) != 1+ADDR_LEN+4 || buf[0] != byte(ADDR_PREFIX) {
		panic(e)
	}

	ph,err := AddressParseFromBytes(buf[1:21])
	if err != nil {
		panic(e)
	}

	addr1 := ph.ToBase58()
	fmt.Println(addr1)

	return ph, nil
}

func (f *Address) ToBase58() string {
	data := append([]byte{ADDR_PREFIX_1}, f[:]...)
	temp_1 := sha256.Sum256(data[:])
	temp_2 := sha256.Sum256(temp_1[:])

	data = append(data, temp_2[0:4]...)
	bi := new(big.Int).SetBytes(data).String()
	encoded, _ := base58.BitcoinEncoding.Encode([]byte(bi))
	return string(encoded)
}

func AddressParseFromBytes(f []byte) (Address, error) {
	if len(f) != ADDR_LEN {
		panic("输入错误")
	}

	var addr Address
	copy(addr[:], f)

	return addr, nil
}


