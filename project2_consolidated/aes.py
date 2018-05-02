from Crypto.Cipher import AES
from Crypto import Random
import Crypto.Util
import binascii
    
def encrypt(img, block_mode): 
    block_size = AES.block_size
    key = Random.get_random_bytes(16)
    img = pad(img, block_size)    
    
    if block_mode == 'MODE_ECB':
        iv = 0
        aes = AES.new(key, AES.MODE_ECB)
    else:
        iv = Random.get_random_bytes(16)
        if block_mode == 'MODE_CBC':
            aes = AES.new(key, AES.MODE_CBC, iv)
        if block_mode == 'MODE_CFB':
            aes = AES.new(key, AES.MODE_CFB, iv)
    
    new_img = aes.encrypt(img)
    return new_img, key, iv
      
def decrypt(img, key, iv, block_mode):
    if block_mode == 'MODE_ECB':
        aes = AES.new(key, AES.MODE_ECB)
    else:
        if block_mode == 'MODE_CBC':
            aes = AES.new(key, AES.MODE_CBC, iv)
        if block_mode == 'MODE_CFB':
            aes = AES.new(key, AES.MODE_CFB, iv)
    
    new_img = aes.decrypt(img)
    return new_img
        
def pad(img, block_size):
    while len(img) % block_size != 0:
        img += b"\0"
    return img
