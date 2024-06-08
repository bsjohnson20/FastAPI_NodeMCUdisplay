# encryption for token.json

import os
from cryptography.fernet import Fernet

def encryptToken():
    # check if token.json exists and isn't encrypted
    if not os.path.exists("secrets/token.json"):
        return
    with open("secrets/token.json", "rb") as f:
        token = f.read()
    if token.startswith(b"gAAAAAB"):
        return
    
    
    key = Fernet.generate_key()
    cipher_suite = Fernet(key)
    with open("secrets/token.json", "rb") as f:
        token = f.read()
    encrypted_token = cipher_suite.encrypt(token)
    with open("secrets/token.json", "wb") as f:
        f.write(encrypted_token)
    with open("secrets/key.key", "wb") as f:
        f.write(key)
    return

def decryptToken():
    if not os.path.exists("secrets/token.json") or not os.path.exists("secrets/secrets/key.key"):
        return
    with open("secrets/key.key", "rb") as f:
        key = f.read()
    cipher_suite = Fernet(key)
    with open("secrets/token.json", "rb") as f:
        token = f.read()
    decrypted_token = cipher_suite.decrypt(token)
    with open("secrets/token.json", "wb") as f:
        f.write(decrypted_token)
    return

if __name__ == "__main__":
    if input("Encrypt or decrypt? (1/2) ") == "1":
        encryptToken()
    else:
        decryptToken()