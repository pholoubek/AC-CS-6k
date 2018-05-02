from Crypto.Cipher import DES
from Crypto import Random
import Crypto.Util
import binascii
    
def encrypt(img, block_mode): 
    block_size = DES.block_size
    key = Random.get_random_bytes(8)      
    img = pad(img, block_size)
      
    if block_mode == 'MODE_ECB':
        iv = 0
        des = DES.new(key, DES.MODE_ECB)
    else:
        iv = Random.get_random_bytes(8)   
        if block_mode == 'MODE_CBC':
            des = DES.new(key, DES.MODE_CBC, iv)
        if block_mode == 'MODE_CFB':
            des = DES.new(key, DES.MODE_CFB, iv)
        
    new_img = des.encrypt(img)    
    return new_img, key, iv
    
def decrypt(img, key, iv, block_mode):
    if block_mode == 'MODE_ECB':
        des = DES.new(key, DES.MODE_ECB)
    else:  
        if block_mode == 'MODE_CBC':
            des = DES.new(key, DES.MODE_CBC, iv)
        if block_mode == 'MODE_CFB':
            des = DES.new(key, DES.MODE_CFB, iv)
            
    new_img = des.decrypt(img)      
    return new_img
    
def pad(img, block_size):
    while len(img) % block_size != 0:
        img += b"\0"
    return img
