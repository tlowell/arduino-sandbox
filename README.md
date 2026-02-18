# arduino-sandbox
Some of my Arduino code that may be useful to others. 

The [waveshare](waveshare) folder is organized by board types using the  board nomenclature found on ther site. These boards require their own sketches to support their unique pin outs,  differing touch technology, etc.

The code in [waveshare-esp32-s3-touch-lcd-1.47](waveshare/waveshare-esp32-s3-touch-lcd-1.47/) emulates the example made by YouTuber Volus in this [video](https://www.youtube.com/watch?v=oPAOMTf5dVE&t=54s) but customized for this board. 

## License

This repository is released under the MIT License.

You are free to use, modify, and reuse this code for any purpose, including commercial projects.
Attribution is appreciated but not required beyond preserving the license notice.

## Third-party components

Some sketches include third-party configuration files (e.g., `lv_conf.h`) derived from LVGL.
LVGL is MIT-licensed. See the LVGL project for details.
