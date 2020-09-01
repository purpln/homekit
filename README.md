#  homekit
## arduino based, for esp8266, esp8285

homekit without a bridge

[HomeKit Arduino library](https://github.com/Mixiaoxiao/Arduino-HomeKit-ESP8266)

On my GitHub you can find only some arduino ide examples for this library. You need only to change ssid and password in .ino file. Use ``111-11-111`` in your home.app to set up an accessory

[full instructions](https://github.com/mcsrgx/homekit/tree/master/instructions/) - arduino ide, esp8266/esp8285, library set up

examples I have:
- [led](https://github.com/mcsrgx/homekit/tree/master/examples/led) - led
- [led_brightness](https://github.com/mcsrgx/homekit/tree/master/examples/led_brightness) - led with brightness slide
- [sensor_temperature](https://github.com/mcsrgx/homekit/tree/master/examples/sensor_temperature) - temperature sensor
- [sensor_door](https://github.com/mcsrgx/homekit/tree/master/examples/sensor_door) - door open/closed sensor
- [sensor_leak](https://github.com/mcsrgx/homekit/tree/master/examples/sensor_leak) - leak sensor
- [sensor_occupancy](https://github.com/mcsrgx/homekit/tree/master/examples/sensor_occupancy) - occupancy sensor
- [outlet](https://github.com/mcsrgx/homekit/tree/master/examples/outlet) - outlet
- led_strip - (WIP)
- lock - (WIP)

some tips:
- if there is random connection loss, use ``MDNS.announce()`` in the ``arduino_homekit_loop()`` in **arduino_homekit_server.cpp**
- if your phone does not see your esp, uncomment ``homekit_storage_reset();`` in **void homekit_setup()** for one boot, then delete it
- [examples](https://github.com/maximkulkin/esp-homekit-demo) that you can base
- [apple's specification](https://developer.apple.com/support/homekit-accessory-protocol/)



<img src="https://freepngimg.com/thumb/apple_logo/25366-7-apple-logo-file.png" width="20"/> 

###### HomeKit Accessory Protocol (HAP) is Apple’s proprietary protocol that enables third-party accessories in the home (e.g., lights, thermostats and door locks) and Apple products to communicate with each other. HAP supports two transports, IP and Bluetooth LE. The information provided in the HomeKit Accessory Protocol Specification (Non-Commercial Version) describes how to implement HAP in an accessory that you create for non-commercial use and that will not be distributed or sold.

###### The HomeKit Accessory Protocol Specification (Non-Commercial Version) can be downloaded from the [HomeKit Apple Developer page.](https://developer.apple.com/homekit/)

###### Copyright © 2020 Apple Inc. All rights reserved.
