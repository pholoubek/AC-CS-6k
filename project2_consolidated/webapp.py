from flask import Flask, render_template, request, session, url_for, redirect
import hashlib
from hashlib import sha512
import datetime
import sys
import os
from werkzeug.utils import secure_filename

import img_cipher as ic

app = Flask(__name__)

app.config['UPLOAD_FOLDER'] = "./static"

##### ENCRYPT #####

#Define route for Encrypt Page 1
@app.route('/', methods=['GET', 'POST'])
def encrypt():
    return render_template('index.html')

#Define route for Encrypt Page 2
@app.route('/encryptStep2', methods=['GET', 'POST'])
def encryptStep2():
    #If URL is typed in
    if request.method == 'GET':
        error = "You do not have permission to view this page."
        return render_template('error.html', error=error)
    
    #If submit on Encrypt Page 1 is clicked
    if request.method == 'POST':
        #Create NEW text file (overwrite old)
        with open('info.txt', "w") as info:
            #Check if image was uploaded
            if 'image' not in request.files:
                error = "Please select an image to upload"
                return render_template('error.html', error = error)

            #Form info
            image = request.files['image']
            imageName = secure_filename(image.filename)

            #Save the image in the /static folder
            destination = os.path.join(app.config['UPLOAD_FOLDER'], imageName)
            image.save(destination)

            encryptionScheme = request.form['encryptionScheme']

            #Add image name to text file
            info.write(imageName)

            #Add delimiter to text file
            info.write(';')

            #Add encryption scheme to text file
            info.write(encryptionScheme)

            #Check which encryption scheme was selected
            if encryptionScheme == 'AES':
                return render_template('encryptMode.html')

            if encryptionScheme == 'DES':
                return render_template('encryptMode.html')

            if encryptionScheme == 'HYBRID':
##                print("encryptStep2")
##                print("imageName: " + imageName)
##                print("encryptionScheme: " + encryptionScheme)
                return hybridEncrypt(imageName, encryptionScheme)

        
            else:
                error = "Invalid encryption scheme"
                return render_template('error.html', error = error)


#Define route for aes and des encrypt
@app.route('/encryptMode', methods=['GET', 'POST'])
def encryptMode():
    #if GET
    if request.method == 'GET':
        error = "You do not have permission to view this page."
        return render_template('error.html', error=error)

    #If submit is clicked on encryptMode.html
    if request.method == 'POST':
        #Open EXISTING text file to read info
        with open('info.txt', "r") as infoRead:
            #Line has the imageName;encryptionScheme
            line = infoRead.readline().strip()
            #print("encryptMode line: " + line)
            infoRead.close()
            
        #Create a new list to hold encrypt info
        encryptInfo = []
        encryptInfo = line.split(";")

        #Save info into variables
        originalImageName = encryptInfo[0]
        encryptionScheme = encryptInfo[1]

        #Get mode from form
        mode = request.form['mode']

        #Add mode to encrypt info
        encryptInfo.append(mode)

        #Do encryption
        img, size, colorMode, form = ic.load_image(originalImageName)    
        encPix, key, iv = ic.encrypt(img, encryptionScheme, mode)
        encryptedImageName = ic.show_encrypted(encPix, size, form, colorMode)
        #print encryptedImageName
        
        #Add the encrypted image name to encryptInfo
        encryptInfo.append(encryptedImageName)

        #Do decryption
        decryptionScheme = encryptionScheme
        
        #
        new_img = ic.decrypt(encPix, key, iv, decryptionScheme, mode)
        
        if len(img) != len(new_img):
            new_img = ic.unpad(new_img, len(img))


        sentinel = ic.hash_it(img, new_img)

        if sentinel:
            decryptedImageName = ic.save_image(new_img, size, form, colorMode)
        else:
            print("Error with decrypt")


        #Add the decrypted image name to encryptInfo
        encryptInfo.append(decryptedImageName)

        print encryptInfo

        #Return list of encryptedInfo to html page
        #data[0] is the original image name
        #data[1] is the encryption scheme
        #data[2] is the mode
        #data[3] is the encrypted image name
        #data[4] is the decrypted image name
        return render_template('result.html', data=encryptInfo)

#Function for hybrid encrypt
def hybridEncrypt(originalImageName, encryptionScheme):
    mode = "N/A"

    #Store encrypt info
    encryptInfo = []
    encryptInfo.append(originalImageName)
    encryptInfo.append(encryptionScheme)
    encryptInfo.append(mode)

    img, size, colorMode, form = ic.load_image(originalImageName)    
    encPix, key, iv = ic.encrypt(img, encryptionScheme, mode)
    encryptedImageName = ic.show_encrypted(encPix, size, form, colorMode)

    #Add the encrypted image name to encryptInfo
    encryptInfo.append(encryptedImageName)

    #Do decryption
    decryptionScheme = encryptionScheme
    
    new_img = ic.decrypt(encPix, key, 0, decryptionScheme, 0)
    print new_img

    if len(img) != len(new_img):
        new_img = ic.unpad(new_img, len(img))

    sentinel = ic.hash_it(img, new_img)
    
    if sentinel is True:
        decryptedImageName = ic.save_image(new_img, size, form, colorMode)
    else:
        print("Error with decrypt")


    #Add the decrypted image name to decryptInfo
    encryptInfo.append(decryptedImageName)
    print decryptedImageName
    #Test print
    print(encryptInfo)
        

    #Return list of encryptedInfo to html page
    #data[0] is the original image name
    #data[1] is the encryption scheme
    #data[2] is the mode
    #data[3] is the encrypted image name
    #data[4] is the decrypted image name
    return render_template('result.html', data=encryptInfo)


app.secret_key = 'janinasoriano'
    
#Run the app on localhost port 3000
if __name__ == "__main__":
    app.run('127.0.0.1', 3000, debug = True)    
