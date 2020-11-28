[![GPL](https://img.shields.io/github/license/syncBlink/station)](https://github.com/syncBlink/station/blob/master/LICENSE)   
[![ko-fi](https://www.ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/A0A01MQZP)

# syncBlink
**syncBlink** is a WiFi connected, programmable and music reactive light system. It supports custom *MOD*s which are executable scripts to influence the light patterns produced by **syncBlink**. The station comes with a custom web application to write and control these scripts.

Furthermore **syncBlink** creates a naive mesh including a *websocket*. **syncBlink Nodes** are able to connect to this mesh and will create an additional access point and websocket for other nodes to connect to. This way nodes not in range of the **syncBlink Station** are also able to join the mesh as long as at least one node is in range. The nodes are able to receive *commands* created by the *Station*.

*Everything combined, it is possible to span a mesh of WiFi connected devices which are synchronously reacting to music.*

> *Click the image for a video of it in action!*
[![syncBlink](https://raw.githubusercontent.com/syncBlink/station/master/img/cover2.jpg)](https://www.youtube.com/watch?v=Lw6lD8utsBI)

# Table of Contents
1. [3D Model](#3d-model)
2. [syncBlink](#syncBlink)  
    a) [BOM](#bom)  
    b) [Firmware](#firmware)  
    c) [Build](#build)
3. [The Application](#the-application)
4. [Create MODs](#create-mods)

## 3D Model
The models were designed in Fusion 360. The exported STLs are included in the Github repository (*stl* folder). Furthermore there are Fusion 360 archive files ready to download (*cad* folder).

## syncBlink

### BOM

- 3D Printed Parts
- 1x Wemos D1 Mini
- 1x 1K Resistor
- 1x Capacitor 680uF
- 1x MAX4466
- [9x WS2812B LEDs (I used a strip and cut it)](https://www.amazon.de/dp/B01CDTED80)
- 4x M2x12mm screws
- 4x M3x4mm screws
- Wire
- Hotglue & Superglue

### Firmware

Before building **syncBlink** you should flash the firmware onto the Wemos D1 Mini. This way you are able to make sure that it is functional. Open the project in [Visual Studio Code](https://code.visualstudio.com/). The project uses [PlatformIO](https://platformio.org/platformio-ide) to develop the firmware. Install the PlatformIO extension into Visual Studio Code, if not already done. 

To flash the firmware onto the Wemos connect it to your PC and press the *Upload* button in the status bar.

![syncBlink - Flash](https://raw.githubusercontent.com/syncBlink/station/master/img/flash.png)

You also have to flash the file system to the Wemos.

![syncBlink - Filesystem](https://raw.githubusercontent.com/syncBlink/station/master/img/fsupload.png)

Afterwards start the *serial monitor* and you should see some outputs like in the screenshot below. If you are able to see the output, the device is working and you are ready to build the **syncBlink Station**.

![syncBlink - Serial](https://raw.githubusercontent.com/syncBlink/station/master/img/serial.png)

### Build

First of all, make yourself comfortable with the following schematics. It shows you all parts of the *Station* which are necessary to connect to each other. We will use a piece of stripboard for each wire which is not directly connected to the Wemos D1 Mini. Mostly to provide power to each part.

![syncBlink - Schematics](https://raw.githubusercontent.com/syncBlink/station/master/img/syncBlink-circuit.png)

Now that you made yourself comfortable with the wiring, take the *Top* part and the *Frame* and glue them together with the superglue. I recommend to place something heavy on top to make sure that they bond well.

![syncBlink - Frame Glued](https://raw.githubusercontent.com/syncBlink/station/master/img/frame-glued.jpg)

Solder wires to your display and mount it to the *Top* part.

![syncBlink - Display](https://raw.githubusercontent.com/syncBlink/station/master/img/display.jpg)

To diffuse the LEDs a bit, the *middle* part has a small slot for a paper insert. Take a bit of paper and cut it into a narrow stripe and insert it into the slot.

![syncBlink - LED Paper](https://raw.githubusercontent.com/syncBlink/station/master/img/led-paper.jpg)

![syncBlink - LED Inserted](https://raw.githubusercontent.com/syncBlink/station/master/img/led-paper-insert.jpg)

Solder three wires to your nine LED stripe and insert it into the *middle* part, too - **pay attention to the data line direction**. I bent the cables away from the back of the shell to avoid any possible interference between the microphone cables and the LEDs.

![syncBlink - LED Inserted](https://raw.githubusercontent.com/syncBlink/station/master/img/led-insert.jpg)

Solder three wires to your microphone and attach it with hotglue to the *Bottom* part of the shell.

![syncBlink - Mic](https://raw.githubusercontent.com/syncBlink/station/master/img/mic.jpg)

Create a 2x7 sized piece out of the stripboard. Picture below as a reference - pay attention to the copper line directions.  
Solder all necessary components and wires as shown in the image. The ground wires are only indicated (read the red comment) to keep the image tidy.

![syncBlink - Veroboard](https://raw.githubusercontent.com/syncBlink/station/master/img/syncBlink-Veroboard.png)

Mount the Wemos D1 Mini to the *Bottom* part of the shell.

![syncBlink - Mounted](https://raw.githubusercontent.com/syncBlink/station/master/img/mounted.jpg)

Power the Wemos and insert a *MOD*. You should see the *decibel* in the top right of the display. Set the gain of you microphone with a small screwdriver. It should float between -40 and -43dB in a silent room. If done, place the stripboard into the shell, place one of the *divider* parts on top of the *bottom* part, then place the *middle* part on top of it. Followed by an additional *divider* and the *top* part.
Insert the *bolts* into the four holes and use the M2x12mm screws to close the shell. Your **syncBlink Station** is done!

### The Application

[TODO]

### Create MODs

**syncBlink** uses [mJS](https://github.com/cesanta/mjs/) to execute the MODs.
On every loop the scripts will get some variables injected by the script engine.

- **vol** *The calculated volume is a value between 0 and 100*
- **freq** *The dominant frequency of the current loop*
- **lVol** *The volume of the last loop*
- **lFreq** *The dominant frequency of the last loop*
- **minF** *The lowest frequency registered by syncBlink - 130hz -> Lowest note for viola, mandola*
- **midF** *The middle frequency value - 1046hz -> Highest note reproducible by average female*
- **maxF** *The highest frequency registered by syncBlink - 3140 -> Between highest note on a flute and on a 88-key piano* 
- **mLedC** *The total number of LEDs in the longest mesh route*
- **pLedC** *Number of LEDs prior of the current executing node*
- **nLedC** *Number of LEDs of the current executing node*
- **pNodeC** *Number of previous nodes in route*

Every script has to implement the *init*, *update* and *getName* method:

```
function init() {}
function update(delta) {}
function getName(){ return "NAME"; }
```

The script API exposes some methods to work with:

- **map(x, in_min, in_max, out_min, out_max)** *Maps a number in a certain range to the corresponding number in the output range*
- **round(number)** *Rounds a number to the next whole integer*
- **xrgb(r, g, b)** *Returns the hex representation of the given RGB color*
- **xhsv(h, s, v)** *Returns the hex representation of the given HSV color*
- **setDelay(count)** *If set to a value >0 the script engine will delay the frequency updates. For example with a value of '1', the script engine would delay the frequency updates by one other frequency update.*
- **setLed(index, hexColor)** *Set the LED with the given index to the given hex color*
- **getLed(index)** *Get the hex color of the given LED*
- **setGroup(index, [led1, led2, ...])** *Set a group of an array of leds to the given index - For example setGroup(0, [0,1]): a setLed(0) would now set index 0 and 1 together*
- **clearGroups()** *Clear all previous group definitions*

Look into the *mod* folder for some examples. If you write your own mod, try to avoid objects and arrays as much as possible. They are perfomance killers!