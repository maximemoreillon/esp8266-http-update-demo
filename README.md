# ESP8266 HTTP update demo

Usage:

1. Update the semver in version.h
2. Export the compiled binary of the sketch
3. Upload version.h and .bin to the server using a POST request

```
curl  -F firmware=@httpUpdateDemo.ino.generic.bin -F version=@version.h http://192.168.1.2:7070/firmware
```
