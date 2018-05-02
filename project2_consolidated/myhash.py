import os
from Crypto.Hash import MD5

def checksum_image(orig_img, new_img):
    md5a = MD5.new()
    md5b = MD5.new()
    
    md5a.update(orig_img)
    md5b.update(new_img)
        
    print "MD5 hash for our original image: " + md5a.hexdigest()
    print "MD5 hash for our decrypted image: " + md5b.hexdigest()
    
    if md5a.hexdigest() == md5b.hexdigest():
        return True 
    else:
        return False
