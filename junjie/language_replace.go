package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"strings"
)

func main() {

	wFile, _ := os.OpenFile("../../../Desktop/android dest.txt", os.O_RDWR, 0666)
	rFile, _ := os.OpenFile("../../../Desktop/android source.csv", os.O_RDWR, 0666)
	nFile, _ := os.OpenFile("../../../Desktop/android.txt", os.O_RDWR, 0666)

	defer wFile.Close()
	defer rFile.Close()
	defer nFile.Close()

	buf1 := bufio.NewReader(wFile)
	buf2 := bufio.NewReader(rFile)

	for {
		line, err1 := buf1.ReadString('\n')
		line = strings.TrimSpace(line)
		start := strings.Index(line, ">")
		end := strings.Index(line, "</")

		data, err2 := buf2.ReadString('\n')
		data = strings.TrimSpace(data)

		line = strings.Replace(line, line[start+1 : end], data, -1)
		nFile.WriteString(line + "\r\n")
		fmt.Println(line)

		if err1 != nil && err2 != nil {
			if err1 == io.EOF && err2 == io.EOF {
				fmt.Println("File read OK!")
				break
			} else {
				fmt.Println("Read file error!")
				return
			}
		}
	}

}
