from Crypto.PublicKey import RSA
from Crypto.Random import get_random_bytes
from Crypto.Cipher import AES, PKCS1_OAEP

def generate_key(user_password):
    key = RSA.generate(2048)
    encrypted_key = key.exportKey(passphrase = user_password, pkcs = 8, protection = "scryptAndAES128-CBC")
    with open('encrypted_data/my_private_rsa_key.bin', 'wb') as f:
        f.write(encrypted_key)
    with open('encrypted_data/my_public_rsa_key.pem', 'wb') as f:
        f.write(key.publickey().exportKey())
    return key
    
def encrypt(img, key):
    recipient_key = RSA.import_key(open('encrypted_data/my_public_rsa_key.pem').read())
    session_key = get_random_bytes(16)

    cipher_rsa = PKCS1_OAEP.new(recipient_key)
    with open('encrypted_data/encrypted_session_key.bin', 'wb') as f:
        f.write(cipher_rsa.encrypt(session_key))
    
    aes = AES.new(session_key, AES.MODE_EAX)
    new_img, tag = aes.encrypt_and_digest(img)
    
    with open('encrypted_data/encrypted_nonce.bin', 'wb') as f:
        f.write(aes.nonce)
    with open('encrypted_data/encrypted_data.bin', 'wb') as f:
        f.write(tag)
        
    return new_img, 0, 0

def decrypt(img, user_password):

    private_key = RSA.import_key(open('encrypted_data/my_private_rsa_key.bin').read(), passphrase = user_password)
    
    iv = open('encrypted_data/encrypted_nonce.bin', 'rb').read()    
    key = open('encrypted_data/encrypted_session_key.bin', 'rb').read()
    tag = open('encrypted_data/encrypted_data.bin', 'rb').read()
        
    cipher_rsa = PKCS1_OAEP.new(private_key)
    session_key = cipher_rsa.decrypt(key)
    
    aes = AES.new(session_key, AES.MODE_EAX, iv)
    new_img = aes.decrypt_and_verify(img, tag)
    
    return new_img
