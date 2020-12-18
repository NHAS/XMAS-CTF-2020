# Cat Clicker

Buy a flag with more than 12 cats. 

# Solution

Effectively this was a length extension attack against `MD5()`. 
From looking at the stateless nature of the webpage, and the hashes returned we can see guess that the hash is something like:
`secret|12|0`, where the last number is the number of cats you currently have. 

I solved this in conjunction with a teammate, we used https://github.com/bwall/HashPump to find generate a number of guesses to find the secret length. Then add extend the hash, to have >13 cats. 

https://en.wikipedia.org/wiki/Length_extension_attack