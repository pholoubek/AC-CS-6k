from PIL import Image
import des, aes, myhash, myrsa
import numpy as np
import binascii
import math


#  insert image object loaded through Pillow library
#  allowed formats: JPEG, PNG, GIF, BITMAP
#  Returns: string of hexidecimal represantation of the image:
#           size of the original image
#           color mode of the picture
#           format of the image             
def load_image(img_name):
    img = Image.open('static/' + img_name, 'r')
    size, mode, form = img.size, img.mode, img.format
    pixels = np.array(img)
    pixels = binascii.hexlify(pixels)
    return pixels, size, mode, form

#   insert hexlifyied representation of the image which is decrypted
#   return string name of the newly saved image
def save_image(img, size, form, mode):
    img = binascii.unhexlify(img)
    new_img = Image.new(mode, size)
    new_img = Image.frombytes(mode, size, img)
    new_img.save('static/newImage2.' + form.lower())
    return 'newImage2.' + form.lower()
    
#   inser hexlified representation of the image which is not decrypted
#   return string name of the newly encrypted and showed image
def show_encrypted(img, size, form, mode):
    new_img = img + '\0' * (size[0] * size[1] * 4 - len(img))
    new_img = Image.frombytes(mode, size, img)
    new_img.save('static/newEncryptedImage.' + form.lower())
    return 'newEncryptedImage.' + form.lower()

#   insert hexlyfied image, group which will be used to encrypt image and which mode if aplicable    
def encrypt(img, group, block_mode):
    if group == 'DES':
        return des.encrypt(img, block_mode)  #  returns new_img, key, iv
    if group == 'AES':
        return aes.encrypt(img, block_mode)  #  returns new_img, key, iv
    if group == 'HYBRID':
        key = myrsa.generate_key('hello world') #  generates RSA key
        return myrsa.encrypt(img, key)  #  returns new_img, tag, iv
        
#   insert string with key, nonce and group with block mode received 
def decrypt(img, key, iv, group, block_mode):
    if group == 'DES':
        return des.decrypt(img, key, iv, block_mode)
    if group == 'AES':
        return aes.decrypt(img, key, iv, block_mode)
    if group == 'HYBRID':
        return myrsa.decrypt(img, 'hello world')
        
def unpad(img, size):
    img = img[:size]
    return img
    
def hash_it(img, new_img):
    return myhash.checksum_image(img, new_img)

    
    
    
    
    
    
