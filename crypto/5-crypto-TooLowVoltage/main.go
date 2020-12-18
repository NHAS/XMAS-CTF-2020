package main

import (
	"bufio"
	"crypto/sha256"
	"encoding/binary"
	"encoding/hex"
	"fmt"
	"log"
	"math"
	"math/big"
	"net"
	"os"
	"strings"
)

func poW(suffix string) string {

	var nonce uint64
	for ; nonce < math.MaxUint64; nonce++ {

		b := make([]byte, 8)
		binary.LittleEndian.PutUint64(b, nonce)

		testHash := sha256.Sum256(b)

		dst := make([]byte, hex.EncodedLen(len(testHash)))
		hex.Encode(dst, testHash[:])
		if string(dst[len(dst)-5:]) == suffix {
			return hex.EncodeToString(b)
		}

	}

	log.Fatal("Couldnt find suffix that matches")
	return ""
}

func main() {

	if len(os.Args) > 1 {
		fmt.Println(poW(os.Args[1]))

		return
	}

	conn, err := net.Dial("tcp", "challs.xmas.htsp.ro:1006")
	if err != nil {
		log.Fatal(err)
	}

	connbuf := bufio.NewReader(conn)
	regular := ""
	n := big.NewInt(0)

	var p, q *big.Int
	forgeryTime := false

	for {
		str, err := connbuf.ReadString('\n')
		if err != nil {
			log.Fatal(err)
		}

		if strings.Contains(str, "Provide a hex") {

			str = strings.TrimSpace(str)

			suff := (string([]byte(str)[len(str)-5:]))

			fmt.Println("Line: ", str)
			fmt.Println("Suffix: ", suff)

			proof := poW(suff)
			fmt.Println("PoW: ", proof)

			conn.Write([]byte(proof + "\n"))
		}

		if strings.Contains(str, "3. exit") {
			if forgeryTime {
				conn.Write([]byte("2\n"))
				continue
			}
			conn.Write([]byte("1\n"))
		}

		if strings.Contains(str, "Give me the signature") {
			parts := strings.Split(str, ":")
			message := strings.TrimSpace(parts[1])
			message = strings.TrimSuffix(message, "'")
			message = strings.TrimPrefix(message, "b'")
			fmt.Printf("Forging singature of '%s'\n", message)

			conn.Write([]byte(sign(p, q, message)))
			conn.Write([]byte("\n"))
		}

		if forgeryTime {
			fmt.Println(str)
		}

		if strings.Contains(str, "n:") {
			parts := strings.Split(str, ":")
			_, worked := n.SetString(strings.TrimSpace(parts[1]), 16)
			if !worked {
				log.Fatal("Unable to create public key out of ", parts[1])
			}
			fmt.Println("Pub key received: ", n)
		}

		// Keep writing a bunch of data to the host in order to stress it out to produce faults
		if strings.Contains(str, "Let's sign something for you.") {
			conn.Write([]byte("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"))
		}

		if strings.Contains(str, "signature: ") {
			parts := strings.Split(strings.TrimSpace(str), ":")
			if regular == "" {
				regular = parts[1] // We assume that the first signature we get is all good
			}

			//If we get anything other than the first one, its got to be a fault as we havent changed the input
			if parts[1] != regular {
				fmt.Println("Got a fault!")

				fmt.Printf("y: '%s'\ny': '%s'\n\n", strings.TrimSpace(regular), strings.TrimSpace(parts[1]))

				y := big.NewInt(0)
				y, worked := y.SetString(strings.TrimSpace(regular), 16)
				if !worked {
					log.Fatal("Unable to make int out of y")
				}

				y_prime := big.NewInt(0)
				y_prime, worked = y_prime.SetString(strings.TrimSpace(parts[1]), 16)
				if !worked {
					log.Fatal("Unable to make int out of y_prime")
				}

				result := big.NewInt(0)
				result.Sub(y, y_prime)

				//Essentially factor the two
				//q = gcd(y-y_prime, n)
				//p = n/q
				//From https://www.computer.org/csdl/magazine/sp/2020/05/09104908/1kj0ZDdPjS8
				q = big.NewInt(0).GCD(nil, nil, result, n)
				fmt.Println("Factor:", q)

				p = big.NewInt(0).Div(n, q)

				forgeryTime = true
			}

		}
	}
}

//This is just the signature function from python using golangs big ints.
// THere is probably a better way of writing this out that isnt so.... big.NewInt(...) heavy... :P
func sign(p, q *big.Int, msg string) string {

	msgInt, worked := big.NewInt(0).SetString(msg, 16)
	if !worked {
		log.Fatal("Unable to create int from message: ", msg)
	}

	e := big.NewInt(0x10001)

	d_p := big.NewInt(0).ModInverse(e, big.NewInt(0).Sub(p, big.NewInt(1)))

	d_q := big.NewInt(0).ModInverse(e, big.NewInt(0).Sub(q, big.NewInt(1)))

	inv_q := big.NewInt(0).ModInverse(q, p)

	s_p := big.NewInt(0).Exp(msgInt, d_p, p)

	s_q := big.NewInt(0).Exp(msgInt, d_q, q)

	dspsq := big.NewInt(0).Sub(s_p, s_q)

	imds := big.NewInt(0).Mul(inv_q, dspsq)

	mimp := big.NewInt(0).Mod(imds, p)

	qmm := big.NewInt(0).Mul(q, mimp)

	saq := big.NewInt(0).Add(s_q, qmm)

	return fmt.Sprintf("%x", saq)
}
