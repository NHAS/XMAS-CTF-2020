# Too Low Voltage

Fault injection attack against RSA under low power situations.

# Solution

As I have an example given in `main.go`, I'll give a brief rundown here. 
Essentially, we hit the target with a large number of key generation requests in order for a fault to occur. 

One we have a faulty signature, we use a known good signature, and the faulty one to factor the public key N.

Math:
```
q = gcd(y-y', n)
p = n/q
```

Once we have these we can easily forge a signature.

Thanks to the article below, I dont think I would have gotten this without them.

https://www.computer.org/csdl/magazine/sp/2020/05/09104908/1kj0ZDdPjS8