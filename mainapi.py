from typing import Union
from datetime import datetime, timedelta, timezone
from fastapi import FastAPI, Depends
from fastapi.security import APIKeyQuery
import os
from dotenv import load_dotenv
from test import main
import encrypt
import requests as rq
from cryptography.fernet import InvalidToken
app = FastAPI()

load_dotenv()

query_scheme = APIKeyQuery(name="api_key")

try:
    encrypt.decryptToken()
except InvalidToken:
    print("Token is not encrypted.")

screenIndex = 0

def fetchCalendar():
    # load from google calendar
    events = main() # date, event name
    # calc days for each event from time now
    # return events
    if not events:
        return "No upcoming events found."
    else:
        # calc
        for event in events:
            # extract days till event
            event[0] = event[0].split("T")[0]
            event[0] = datetime.strptime(event[0], "%Y-%m-%d")
            event[0] = event[0] - datetime.now()
            event[0] = event[0].days
        # sort by days
        events = sorted(events, key=lambda x: x[0])
            
        return "\n".join([f"{event[1]}: {event[0]}" for event in events])
            



timeSinceLastPoll = 0
def getWeather():
    global timeSinceLastPoll
    # load from weather api
    weatherKey = os.environ["API_KEY"]
    # check if timeSince is 0 then set timeSince to current time
    # update timeSinceLastPoll
    timeSinceLastPoll = datetime.now()
    # load from api
    r = rq.get(f"http://api.weatherapi.com/v1/current.json?key={weatherKey}&q=London&aqi=no")
    data = r.json()
    temperature = data["current"]["temp_c"]
    condition = data["current"]["condition"]["text"]
    print(f"Temperature: {temperature}°C\nCondition: {condition}")
    with open("weather.txt", "w") as f:
        f.write(f"Temperature: {temperature}°C\nCondition: {condition}")
    return f"Temperature: {temperature}°C\nCondition: {condition}"

def readTextSend():
    if not os.path.exists("text.txt"):
        with open("text.txt", "w") as f:
            f.write("Hello, World!")
    with open("text.txt", "r") as f:
        text = f.read()
    return text


screenList = [
    "weather",
    # "chores",
    "time",
    "calendar",
    "favouritePony",
]

def parseTime():
    # return current time in human readable format
    day = datetime.now().strftime("%A")
    date = datetime.now().strftime("%d")
    month = datetime.now().strftime("%B")
    year = datetime.now().strftime("%Y")
    hour = datetime.now().strftime("%H")
    minute = datetime.now().strftime("%M")
    string = f"It is {day}\ndate: {date}\nmonth: {month}\nyear: {year}\ntime: {hour}:{minute}"
    return string

def favouritePony():
    return "Princess Luna"

cache = dict(lifetime=0, calendar="", weather="")

@app.get("/")
def read_root():
    global cache
    # if cache is empty, load all data
    if not cache:
        cache["calendar"] = fetchCalendar()
        cache["weather"] = getWeather()
    # if cache is older than 5m, reload all data
    if cache["lifetime"] == 0:
        cache["lifetime"] = datetime.now()
    elif datetime.now() - cache["lifetime"] > timedelta(seconds=5):
        cache["calendar"] = fetchCalendar()
        cache["weather"] = getWeather()
        cache["lifetime"] = datetime.now()
    
    # Text should be displayed such that the controller reads it.
    global screenIndex, screenList
    if screenIndex == len(screenList):
        screenIndex = 0
    else:
        screenIndex += 1
    
    if screenIndex == -1 or screenIndex == len(screenList):
        screenIndex = 0
    
    if screenList[screenIndex] == "favouritePony":
        return "Favourite pony:\n"+favouritePony()
    elif screenList[screenIndex] == "calendar":
        return "Calendar - days till:\n"+cache["calendar"]
    elif screenList[screenIndex] == "time":
        return "Time:\n"+parseTime()
    elif screenList[screenIndex] == "weather":
        return "Weather:\n"+cache["weather"]
    return "This should not be displayed."
    # return readTextSend()

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

# handle fastapi exit

encrypt.encryptToken()

# on app exit, encrypt token.json


# @app.get("/items/{item_id}")
# def read_item(item_id: int, q: Union[str, None] = None):
#     return {"item_id": item_id, "q": q}
