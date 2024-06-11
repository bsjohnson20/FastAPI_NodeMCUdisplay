<div align="center" id="top"> 
  <img src="./.github/app.gif" alt="FastAPI_NodeMCUdisplay" />

  &#xa0;

  <!-- <a href="https://fastapi_nodemcudisplay.netlify.app">Demo</a> -->
</div>

<h1 align="center">FastAPI_NodeMCUdisplay</h1>

<p align="center">
  <img alt="Github top language" src="https://img.shields.io/github/languages/top/bsjohnson20/fastapi_nodemcudisplay?color=56BEB8">

  <img alt="Github language count" src="https://img.shields.io/github/languages/count/bsjohnson20/fastapi_nodemcudisplay?color=56BEB8">

  <img alt="Repository size" src="https://img.shields.io/github/repo-size/bsjohnson20/fastapi_nodemcudisplay?color=56BEB8">

  <img alt="License" src="https://img.shields.io/github/license/bsjohnson20/fastapi_nodemcudisplay?color=56BEB8">

  <!-- <img alt="Github issues" src="https://img.shields.io/github/issues/bsjohnson20/fastapi_nodemcudisplay?color=56BEB8" /> -->

  <!-- <img alt="Github forks" src="https://img.shields.io/github/forks/bsjohnson20/fastapi_nodemcudisplay?color=56BEB8" /> -->

  <!-- <img alt="Github stars" src="https://img.shields.io/github/stars/bsjohnson20/fastapi_nodemcudisplay?color=56BEB8" /> -->
</p>

<!-- Status -->

<!-- <h4 align="center"> 
	🚧  FastAPI_NodeMCUdisplay 🚀 Under construction...  🚧
</h4> 

<hr> -->

<p align="center">
  <a href="#dart-about">About</a> &#xa0; | &#xa0; 
  <a href="#sparkles-features">Features</a> &#xa0; | &#xa0;
  <a href="#rocket-technologies">Technologies</a> &#xa0; | &#xa0;
  <a href="#white_check_mark-requirements">Requirements</a> &#xa0; | &#xa0;
  <a href="#checkered_flag-starting">Starting</a> &#xa0; | &#xa0;
  <a href="#memo-license">License</a> &#xa0; | &#xa0;
  <a href="https://github.com/bsjohnson20" target="_blank">Author</a>
</p>

<br>

## :dart: About ##

A small project using a NodeMCU + OLED display, and a python fastapi server.
The fastapi serves text for the OLED to display, over a certain amount of characters the text is cut off. However new lines will be scolled through every second. 

Once the NodeMCU has displayed the contents from the GET request, it'll wait a second and fetch more data.

The fastapi serves as a sort of screen, in the default configuration it cycles through various things, like a calendar, time, weather and so forth.

On start the NodeMCU starts an AP (access-point) displaying the SSID and password to join + the IP of the nodemcu such that you can open a basic web server from the nodemcu to set the details of the wifi SSID+Password as well as an IP for the API server.

## :sparkles: Features ##

:heavy_check_mark: WIFI setup page;\
:heavy_check_mark: FASTAPI display server;\
:heavy_check_mark: OLED text displaying "screens";

## :rocket: Technologies ##

The following tools were used in this project:

Arduino libraries:
- arduino-libraries/ArduinoHttpClient@^0.6.0
- thingpulse/ESP8266 and ESP32 OLED driver for SSD1306 displays@^4.6.1
- yoprogramo/QRcodeDisplay

Python libraries are available in the requirements.txt

## :white_check_mark: Requirements ##

Before starting :checkered_flag:, you need to install the python requirements, have a NodeMCU with builtin OLED display / attached (you may need to configure the pinouts)

An Arduino flashing tool, like the arduino app, or PlatformIO. The platformIO.ini is available in the NodeMCU folder

## :checkered_flag: Starting ##

```bash
# Clone this project
$ git clone https://github.com/bsjohnson20/fastapi_nodemcudisplay

# Access
$ cd fastapi_nodemcudisplay

# Install dependencies
$ pip install -r requirements.txt

# Run the project
$ fastapi run mainapi.py -p 9000

# The server will initialize in the <http://localhost:9000>
```

Connect to the NodeMCU AP, open the webpage on the IP printed to the OLED screen, input the wifi SSID and Password to connect, as well as the API server ipaddr.

The NodeMCU will attempt to connect, then send a get request to the ipaddr:9000/

## :memo: License ##

This project is under license from MIT. For more details, see the [LICENSE](LICENSE.md) file.


Made with :heart: by <a href="https://github.com/bsjohnson20" target="_blank">{{YOUR_NAME}}</a>

&#xa0;

<a href="#top">Back to top</a>
