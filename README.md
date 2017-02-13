Weather Magnet Spark-Core
=========================
WiFi connected Weather magnet using Particle(spark) Core
and 4 digit 7 Segment display

Building Tutorial: coming Soon

![Digit Config](https://github.com/zandegran/Weather_Magnet-SparkCore/raw/master/blob/magnet.JPG)<br/>
#### Digit Configuration
1, 2 & 3 	- 	Shows the temperature<br/>
4		- 	Shows the weather status (see status codes)<br/>
#### Status Codes
* `U`	-	Unknown
* `t`	-	Thunders
* `C`	-	Clear sky
* `o`	-	Partly Overcast
* `O`	-	Overcast
* `d`	-	Drizzle
* `r`	-	Rain
* `S`	-	Snow
* `F`	-	Fog or Mist
* `E`	-	Extreme


#### Configuration
* Create a free account in openweathermap.org and create appid
* Add appid in the weather.json.
* Change the City name to yours.
* Change units to `imperial`, if you want temperature in farenheit.
* Check the api call using this URL (Change attributes according to the config)
`http://api.openweathermap.org/data/2.5/weather?q=stockholm,SE&units=metric&appid=<App-ID>`

##### Creating Webhook
Once configuration is done and checked, Use `particle webhook create webhook.json` command to create the webhook.

#### How to Build Firmware
##### Using Online IDE
* Copy the content of `weather.app` in a new project
* Add the library `elapsedMillis` from builtin libraries
* Verify and Flash 

##### Manual build for core
* Get base firmware from `https://github.com/spark/firmware`
* Copy the folder weather to `user/applications`
* cd to the `main` folder in base directory
* Build using `make clean all PLATFORM=core PARTICLE_DEVELOP=1 APP=weather`

##### Schematics Help
[www.learningaboutelectronics.com](http://www.learningaboutelectronics.com/Articles/4-digit-7-segment-LED-circuit-with-an-arduino.php)
