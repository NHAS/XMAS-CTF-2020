import itertools
import os
import random


def get_seed(size):
    return int(os.urandom(size).hex(), 16)


input_data = None
output_data = ""

seed = get_seed(4)


def takeSecond(elem):
    return elem[1]


random.seed(seed)

old_sigma = "0123456789abcdef"
new_sigma = list(old_sigma)
random.shuffle(new_sigma)
new_sigma = ''.join(new_sigma)
print(old_sigma, new_sigma)

with open("input.txt", "r") as in_file:
    input_data = in_file.read().replace('\n', '')

for alpha in input_data:
    encoded = (bytes(alpha.encode()).hex())
    output_data += new_sigma[old_sigma.index(encoded[0])]
    output_data += new_sigma[old_sigma.index(encoded[1])]


with open("output.txt", "w") as out_file:
    out_file.write(str(output_data))

with open("output.txt", "r") as in_file:
    input_data = in_file.read().replace('\n', '')


allperms = list(itertools.permutations(old_sigma))

print(allperms)
