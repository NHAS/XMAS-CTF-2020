# Lost List

Effectively this boiled down to flipping bits in an unauthenticated encrypted message. 



# Solution

In examining the provided pcap file with wireshark we are given a hex encoded block, which appears to be running `ls` on the current directory.

`53616e74612773313333374956343230ab0c288b0ae26eaf8adbcf00bddf35fa`

When this block is decoded we see:

`"Santa's1337IV420 \xab\x0c(\x8b\n\xe2n\xaf\x8a\xdb\xcf\x00\xbd\xdf5\xfa"`

Which clearly shows an IV (Initialisation Vector), and the encrypted data. When I saw this, I immediately assumed it was AES in CBC mode. 

Since this encryption mode is 'unauthenticated' which means you can change bits without the decryption rejecting your input, you can flip bits in the ciphertext/IV. Which changes the resulting plaintext.

My thoughts were to flip the bits of the starting two bytes of the IV to change `ls` to `sh`. 


```
m = '53616e74612773313333374956343230ab0c288b0ae26eaf8adbcf00bddf35fa'

We know:
plaintext = "ls"

Thus we know that the first two bytes need to be flipped (53, 61). 

m1 = hex(0x53 ^ ord('l') ^ ord('s'))
   = 4c
m2 = hex(0x61 ^ ord('s') ^ ord('h'))
   = 7a


Our new message is:
4c7a6e74612773313333374956343230ab0c288b0ae26eaf8adbcf00bddf35fa
```

Entering `4c7a6e74612773313333374956343230ab0c288b0ae26eaf8adbcf00bddf35fa`, shows.... nothing. 
This is because the shell is reading out input, but the command only prints the commands execution when it finishes.

So if we type: 
```
cat *
exit
```

We get:
```
#!/usr/bin/python3

key = b'\xd1\x1c\x07\xca\xfe\xba\xbe\xde\xad\xbe\xef\x42\xf0\x0d\xba\xbe'
#!/usr/bin/python3
import os
from Crypto.Cipher import AES
from Crypto.Util.number import long_to_bytes
from key import key
import subprocess

def unpad(ct):
    return ct[:-(ct[-1])]

def decrypt(ct):
    return AES.new(key, AES.MODE_CBC, ct[:16]).decrypt(ct[16:])

blacklist = [b'tac', b'cat', b'less', b'more', b'head', b'tail', b'nl', b'grep', b'key']

for i in range(64):
    try:
        enc_cmd = long_to_bytes(int(input('santa@northpole ~$ '), 16))
        cmd = unpad(decrypt(enc_cmd))
    except:
        print('Command not found')
        continue

    test_cmd = cmd
    for i in [b'\\', b'`', b'$', b'(', b')', b'{', b'}', b'&', b'*', b"'", b'"']:
        test_cmd = test_cmd.replace(i, b'')
    
    if not any(i in test_cmd for i in blacklist):
        print(subprocess.check_output([b'/bin/bash', b'-c', cmd]).decode(), flush=True)
    else:
        print('Command not found')
Eve
Grinch
The weird guy that winked at me in the bus 4 years ago and then left without a bye (I'll never forget you random dude)
Alice
Bob
Galf
X-MAS{s33ms_1ik3_y0u_4r3_0n_7h3_1is7_700_h0_h0_h0}
```

Also, I want to note this article below. It was super helpful and I dont think I would have gotten this as quickly as I did without it:


https://dr3dd.gitlab.io/cryptography/2019/01/10/simple-AES-CBC-bit-flipping-attack/


