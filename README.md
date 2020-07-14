[![GPL](https://img.shields.io/github/license/glowfly/glowfly_station)](https://github.com/glowfly/glowfly_station/blob/master/LICENSE)   
[![ko-fi](https://www.ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/A0A01MQZP)

# GlowFly
**GlowFly** is a WiFi connected, programmable and music reactive light system. It supports custom *MOD* cartridges which contain executable scripts. These scripts are able to influence the light patterns produced by **GlowFly**.

Furthermore **GlowFly** creates a naive mesh including a *websocket*. **GlowFly Nodes** are able to connect to this mesh and will create an additional access point and websocket for other nodes to connect to. This way nodes not in range of the **GlowFly Station** are also able to join the mesh as long as at least one node is in range. The nodes are able to receive *commands* created by the *Station*.

*Everything combined, it is possible to span a mesh of WiFi connected devices which are synchronously reacting to music.*

> *Click the image for a video of it in action!*
[![GlowFly](https://raw.githubusercontent.com/glowfly/glowfly_station/master/img/cover2.jpg)](https://www.youtube.com/watch?v=Lw6lD8utsBI)

# Table of Contents
1. [3D Model](#3d-model)
2. [GlowFly](#GlowFly)  
    a) [BOM](#bom)  
    b) [Firmware](#firmware)  
    c) [Build](#build)

## 3D Model
The models were designed in Fusion 360. The exported STLs are included in the Github repository (*stl* folder). Furthermore there are Fusion 360 archive files ready to download (*cad* folder).

## GlowFly

### BOM

- 3D Printed Parts
- 1x Wemos D1 Mini
- 1x 1K Resistor
- 1x Capacitor 680uF
- 1x MAX4466
- 1x 1x5 DuPont Male Pin Header
- [9x WS2812B LEDs (I used a strip and cut it)](https://www.amazon.de/dp/B01CDTED80)
- 4x M2x12mm screws
- 4x M3x4mm screws
- [Stripboard](https://www.amazon.com/dp/B00C9NXP94)
- Wire
- Hotglue & Superglue

### Firmware

Before building **GlowFly** you should flash the firmware onto the Wemos D1 Mini. This way you are able to make sure that it is functional. Open the project in [Visual Studio Code](https://code.visualstudio.com/). The project uses [PlatformIO](https://platformio.org/platformio-ide) to develop the firmware. Install the PlatformIO extension into Visual Studio Code, if not already done. 

To flash the firmware onto the Wemos connect it to your PC and press the *Upload* button in the status bar.

![GlowFly - Flash](https://raw.githubusercontent.com/glowfly/glowfly_station/master/img/flash.png)

Afterwards start the *serial monitor* and you should see some outputs like in the screenshot below. If you are able to see the output, the device is working and you are ready to build the **GlowFly Station**.

![GlowFly - Serial](https://raw.githubusercontent.com/glowfly/glowfly_station/master/img/serial.png)

### Build

First of all, make yourself comfortable with the following schematics. It shows you all parts of the *Station* which are necessary to connect to each other. We will use a piece of stripboard for each wire which is not directly connected to the Wemos D1 Mini. Mostly to provide power to each part.

![GlowFly - Schematics](https://raw.githubusercontent.com/glowfly/glowfly_station/master/img/GlowFly-circuit.png)

Now that you made yourself comfortable with the wiring, take the *Top* part and the *Frame* and glue them together with the superglue. I recommend to place something heavy on top to make sure that they bond well.

![GlowFly - Frame Glued](https://raw.githubusercontent.com/glowfly/glowfly_station/master/img/frame-glued.jpg)

Solder wires to your display and mount it to the *Top* part.

![GlowFly - Display](https://raw.githubusercontent.com/glowfly/glowfly_station/master/img/display.jpg)

To diffuse the LEDs a bit, the *middle* part has a small slot for a paper insert. Take a bit of paper and cut it into a narrow stripe and insert it into the slot.

![GlowFly - LED Paper](https://raw.githubusercontent.com/glowfly/glowfly_station/master/img/led-paper.jpg)

![GlowFly - LED Inserted](https://raw.githubusercontent.com/glowfly/glowfly_station/master/img/led-paper-insert.jpg)

Solder three wires to your nine LED stripe and insert it into the *middle* part, too - **pay attention to the data line direction**. I bent the cables away from the back of the shell to avoid any possible interference between the microphone cables and the LEDs.

![GlowFly - LED Inserted](https://raw.githubusercontent.com/glowfly/glowfly_station/master/img/led-insert.jpg)

Solder three wires to your microphone and attach it with hotglue to the *Bottom* part of the shell.

![GlowFly - Mic](https://raw.githubusercontent.com/glowfly/glowfly_station/master/img/mic.jpg)

At this point I advise you to build your first [MOD](https://github.com/glowfly/glowfly_mods). A *MOD* will help to align the MOD connector correctly in the *bottom* shell.   

**Playing jingle ...**

Now that your first **MOD** is done, take the stripboard and create one 4x7 and one 2x5 sized piece. Picture below as a reference - pay attention to the copper line directions.

![GlowFly - Stripboard](https://raw.githubusercontent.com/glowfly/glowfly_station/master/img/stripboard.jpg)

Solder the DuPont male pin header to one row of the 2x5 stripboard piece and five wires to the second row. The wires should be directed to the opposite side of the pin header and should sit flush. Otherwise the connector will not sit correctly in the cartridge slot. 

Now insert the *MOD* connector into the slot of the *Bottom* part of the shell and try to insert your *MOD*. If they fit, hotglue the connector to the shell. This way the connector and the *MOD*s will align perfectly.

![GlowFly - MOD Connector](https://raw.githubusercontent.com/glowfly/glowfly_station/master/img/mod-connector.jpg)

Take the 4x7 stripboard you prepared before and solder all necessary components and wires as shown in the image below. The ground wires are only indicated (read the red comment) to keep the image tidy. Also solder every other wire like indicated in the above schematics.

![GlowFly - Veroboard](https://raw.githubusercontent.com/glowfly/glowfly_station/master/img/GlowFly-Veroboard.png)

Mount the Wemos D1 Mini to the *Bottom* part of the shell.

![GlowFly - Mounted](https://raw.githubusercontent.com/glowfly/glowfly_station/master/img/mounted.jpg)

Power the Wemos and insert a *MOD*. You should see the *decibel* in the top right of the display. Set the gain of you microphone with a small screwdriver. It should float between -40 and -43dB in a silent room. If done, place the stripboard into the shell, place one of the *divider* parts on top of the *bottom* part, then place the *middle* part on top of it. Followed by an additional *divider* and the *top* part.
Insert the *bolts* into the four holes and use the M2x12mm screws to close the shell. Your **GlowFly Station** is done!