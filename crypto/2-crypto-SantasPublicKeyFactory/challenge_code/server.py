import os
import sys
from hashlib import sha256
from chall import *
from PoW import *
import binascii

action_cnt = 256
secret_message = os.urandom(8).hex()
cipher = chall(1024, 16)

for i in range(action_cnt):

    x = input()
    if not x in ["1", "2", "3"]:

        exit()

    if x == "1":
        msg = int(secret_message, 16)
        pubkey, privkey = cipher.get_key()
        ct = hex(cipher.encrypt(msg, pubkey))[2:]
        n, e = pubkey

    else:

        exit()

sys.stdout.flush()
