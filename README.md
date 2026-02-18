# arduino-sandbox
A collection of Arduino sketches that may be useful to others. 

The [waveshare](waveshare)  folder is organized by board type using the model naming convention found on the Waveshare website (e.g., esp32-s3-touch-lcd-1.47).
These boards require their own sketches to support unique pinouts, differing touch controllers, display variants, etc.

The code in [waveshare-esp32-s3-touch-lcd-1.47](waveshare/waveshare-esp32-s3-touch-lcd-1.47/) emulates the example created by YouTuber Volos Projects in this [video](https://www.youtube.com/watch?v=oPAOMTf5dVE&t=54s), but customized for this specific board.


## License

This repository is released under the MIT License.

You are free to use, modify, and reuse this code for any purpose, including commercial projects.
Attribution is appreciated but not required beyond preserving the license notice.

## Third-party components

Some sketches include third-party configuration files (e.g., `lv_conf.h`) derived from LVGL.
LVGL is MIT-licensed. See the LVGL project for details.
