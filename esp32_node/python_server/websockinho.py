import asyncio
import websockets
import wfdb
import numpy as np
import json
from hashlib import md5
from base64 import b64decode
from base64 import b64encode

from Crypto.Cipher import AES
from Crypto.Cipher import DES
from Crypto.Random import get_random_bytes
np.set_printoptions(threshold=np.inf)

class ESP32_Client:
    def __init__(self, MAC, IP):
        self.mac = MAC
        self.ip = IP
    def update_state(self,state):
        self.state = state

current_key = b""
current_iv = b""

#lista que vai guardar todas instancias de clientes
clientes = []

def get_crypto_cfg(message):
    try:
        data = json.loads(message)
        if("crypto" in data):
            print("message header",data)
            return data
    except:
        print("not crypto cfg")

def decrypt_data(crypto_cfg,message):
    if(crypto_cfg["crypto"] == "none"):
        print("Processing raw data")
        my_data = np.frombuffer(message)
        #print(my_data) 
        wfdb.plot_items(signal=my_data,fs=500, title='ECG') #plotar dados recebidos
    elif(crypto_cfg["crypto"] == "aes_cbc"):
        print("processing aes cbc crypto mode")
        key = bytearray.fromhex(crypto_cfg["key"])
        iv = bytearray.fromhex(crypto_cfg["iv"])

async def handler(websocket, path):
    async for message in websocket:
        #print("data ",len(message))
        if(len(message) > 1000):#mensagem de dados eh grande 
            decrypt_data(crypto_cfg,message)
        else:
            crypto_cfg = get_crypto_cfg(message)
        if(type(message) is bytes):
            #print("bytes ->",message)
            pass
        else:
            #print("string ->",message)
            pass
            

async def main():
    async with websockets.serve(handler, "192.168.15.11", 8765):
        await asyncio.Future()  # run forever

asyncio.run(main())