# Help a Santa Helpers

For this, you had to find two inputs that would generate the same 'hashed' output. Otherwise known as a collision. 

# Solution

Looking at the `Hash` class in `chall.py`, we can see that AES in ECB mode is being used as a hashing function. With the last 16 bytes of the encryption operation being output as the hash. 

Here `^` is used to denote XOR, and `E(...)` is used to denote encryption with AES.
`self.hash` is an internal state of the hash function. Each 16 byte block that is computed sets the `self.hash` as the result.

Important thing to know: 
Anything XOR itself is equal to `0`.
e.g `5 ^ 5 = 0`.

Anything XOR `0` is equal to itself.


1.
So, we can tell that for an input size of < 16 bytes (32 hex bytes) that `self.hash` = `0`

This means we can find `E(0)`. How? 

Set our message to `0`, get the output.

```
m = 00000000000000000000000000000000
m ^ E(self.hash ^ m)
0 ^ E(0 ^ 0) 
= 
E(0)
```

For demonstration purposes we'll think of `E(0)` as `7f7f527d80e8070ed1c87ce6f7b716ca`.

2.

Now we have `E(0)` we can effectively use it to control `self.hash`.


```
message = 000000000000000000000000000000007f7f527d80e8070ed1c87ce6f7b716ca
Thus:
first_block = 00000000000000000000000000000000
second_block = 7f7f527d80e8070ed1c87ce6f7b716ca (<- Remember this is `E(0)`)

self.hash = 0
self.hash = first_block ^ E(self.hash ^ first_block)
          = 0 ^ E(0^0)
          = E(0)

self.hash = E(0) = 7f7f527d80e8070ed1c87ce6f7b716ca
self.hash = second_block ^ E(self.hash ^ second_block)
          = 7f7f527d80e8070ed1c87ce6f7b716ca ^ E(7f7f527d80e8070ed1c87ce6f7b716ca ^ 7f7f527d80e8070ed1c87ce6f7b716ca)
          = 7f7f527d80e8070ed1c87ce6f7b716ca ^ E(0)
          = 0

self.hash = 0

Resulting hash: 00000000000000000000000000000000
```

3.
Now, since `self.hash` is set to `0` we effectively are back at the start.
So we can just chain these messages together, a different message but the same hash, solving the problem. 

e.g
```
hash(000000000000000000000000000000007f7f527d80e8070ed1c87ce6f7b716ca) =
hash(000000000000000000000000000000007f7f527d80e8070ed1c87ce6f7b716ca000000000000000000000000000000007f7f527d80e8070ed1c87ce6f7b716ca)
```


# Example

```
Hello, thanks for answering my call, this time we have to break into Krampus' server and recover a stolen flag.
We have to solve a hash collision problem to get into the server.
Sadly, we're on a hurry and you only have 2 actions you can make until we get detected.
Choose what you want to do:
1. hash a message
2. provide a collision
3. exit

1
Ok, give me a hex-encoded string to hash.

Give me a message.
00000000000000000000000000000000
Here is your hash: b'7f7f527d80e8070ed1c87ce6f7b716ca'.

Choose what you want to do:
1. hash a message
2. provide a collision
3. exit

2
Now give me two different hex-encoded strings to check for a collision.

Give me a message.
000000000000000000000000000000007f7f527d80e8070ed1c87ce6f7b716ca
Give me a message.
000000000000000000000000000000007f7f527d80e8070ed1c87ce6f7b716ca000000000000000000000000000000007f7f527d80e8070ed1c87ce6f7b716ca
Damn, that was a really clever approach, you should be proud of yourself.
Here's the flag: X-MAS{C0l1i5ion_4t7ack5_4r3_c0o1!_4ls0_ch3ck_0u7_NSUCRYPTO_fda233}
```