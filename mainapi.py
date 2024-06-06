from typing import Union
from datetime import datetime
from fastapi import FastAPI, Depends
from fastapi.security import APIKeyQuery
import os
app = FastAPI()

query_scheme = APIKeyQuery(name="api_key")

@app.get("/")
def read_root():
    return {"Hello": "World"}

def load_file_apikeys():
    if not os.path.exists("apikeys.txt"):
        with open("apikeys.txt", "w") as f:
            f.write("12345\n")
    with open("apikeys.txt", "r") as f:
        apikeys = f.read().splitlines()
    return apikeys

def query_scheme(api_key: str = Depends(query_scheme)):
    apikeys = load_file_apikeys()
    if api_key in apikeys:
        return api_key
    else:
        return "INVALID"
    


@app.get("/q")
# print current time if authenticated
def read_item(api_key: str = Depends(query_scheme)):
    if api_key == "INVALID":
        return {"error": "Invalid API Key"}
    else:
        return {"current_time": datetime.now()}



# @app.get("/items/{item_id}")
# def read_item(item_id: int, q: Union[str, None] = None):
#     return {"item_id": item_id, "q": q}
