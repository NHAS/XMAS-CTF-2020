package main

import (
	"encoding/hex"
	"fmt"
	"io/ioutil"
	"log"
	"os"
)

func main() {
	if len(os.Args) < 2 {
		log.Fatal("Please enter filename")
	}

	contents, err := ioutil.ReadFile(os.Args[1])
	if err != nil {
		log.Fatal(err)
	}

	decoded := make([]byte, hex.DecodedLen(len(contents)))

	_, err = hex.Decode(decoded, contents)
	if err != nil {
		log.Fatal(err)
	}

	frequencyTable := make(map[byte]int)
	for _, v := range decoded {
		if _, ok := frequencyTable[v]; !ok {
			frequencyTable[v] = 1
			continue
		}
		frequencyTable[v] = frequencyTable[v] + 1
	}

	for k, v := range frequencyTable {
		fmt.Printf("%02x : %d\n", k, v)
	}

}
