Weather Magnet Spark-Core
=========================
Weather magnet using Particle(spark) Core
and 4 digit 7 Segment display

Building Tutorial: coming Soon
#### Digit Configuration
![Digit Config](http://www.learningaboutelectronics.com/images/4-digit-7-segment-LED-display.png)<br/>
Image source: [www.learningaboutelectronics.com](http://www.learningaboutelectronics.com/Articles/4-digit-7-segment-LED-circuit-with-an-arduino.php) <br/>
1 		- 	Shows minus in case of negative weather <br/>
2 & 3 	- 	Shows the temperature<br/>
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

#### How to Build
##### Using Online IDE
* Copy the content of `weather.app` in a new project
* Add the library `elapsedMillis` from libraries
* Verify and Flash 

##### Manual

