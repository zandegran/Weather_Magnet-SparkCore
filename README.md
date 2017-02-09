Weather Magnet Spark-Core
=========================
Weather magnet using Particle(spark) Core
and 4 digit 7 Segment display

Building Tutorial: coming Soon
#### Digit Configuration
![Digit Config](http://www.learningaboutelectronics.com/images/4-digit-7-segment-LED-display.png)<br/>
Image source: [www.learningaboutelectronics.com](http://www.learningaboutelectronics.com/Articles/4-digit-7-segment-LED-circuit-with-an-arduino.php) <br/>
1, 2 & 3 	- 	Shows the temperature<br/>
4		- 	Shows the weather status (see status codes)<br/>
#### Status Codes
* `U`	-	Unknown
* `t`	-	Thunders
* `G`	-	Good or Clear sky
* `c`	-	Partly 
* `C`	-	Cloudy
* `d`	-	Drizzle
* `r`	-	Rain
* `S`	-	Snow
* `F`	-	Fog or Mist
* `E`	-	Extreme


#### Configuration
* Create a free account in openweathermap.org.
* Add appid in the weather.json.
* Change the City name to yours.
* Change units to imperial, if you want temperature in farenheit.
* Check the api call using this URL (Change attributes according to the config)
`http://api.openweathermap.org/data/2.5/weather?q=stockholm,SE&units=metric&appid=<App-ID>`

##### Creating Webhook
Once configuration is done checked, Use `particle webhook create webhook.json` to create the webhook.

#### How to Build
##### Using Online IDE
* Copy the content of `weather.app` in a new project
* Add the library `elapsedMillis` from libraries
* Verify and Flash 

##### Manual