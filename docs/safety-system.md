# Safety system

The appliance boils water above a mains-derived heater, so protection is layered and does not rely on firmware.

## Hardware layers (independent of the microcontroller)

```mermaid
graph LR
    V12[12V rail] --> F1[F1: 5A fuse]
    F1 --> TB10[TB10: lid interlock]
    TB10 --> K1[Relay 1 NO contact]
    K1 --> F2[F2: bimetallic cutoff 95-100 C]
    F2 --> HTR[Heater element]
    HTR --> GND[Ground]
```

| Layer | Part | Protects against | Behaviour |
|---|---|---|---|
| 1 | F2 bimetallic thermal cutoff, 95 to 100 C, on the boiler wall, in series with the heater | dry boil, firmware fault, welded relay contacts | snap-disc opens and breaks the heater circuit |
| 2 | TB10 lid microswitch, in series with the heater feed | steam and burn exposure when the lid opens | heater power cut in hardware |
| 3 | F1 5 A fast-acting cartridge fuse on the 12 V rail | short circuits, pump stall | blows |

Because the layers are in series with the heater, any one of them removes heat even if the ESP32 is hung, unpowered or mis-programmed.

## Firmware supervision (drafted in `ino/`, not yet run on the appliance)

| Signal | Pin | Intended action |
|---|---|---|
| Lid sense | GPIO25 | abort cycle, heater off |
| Optical boil-over | GPIO26 | heater off, vent |
| Reservoir level | GPIO33 | refuse to start or stop the cycle when low |
| Enclosure NTC | GPIO32 | derate or stop on over-temperature |
| Liquid temperature | GPIO4 | hold 85 to 88 C, trip above the envelope |

## Other design points

* Opto-isolated relay drive and a separate Power Board keep switching noise and mains-side wiring away from the analog sensors.
* Heater traces on the Power Board are specified at heavy-copper width (1.5 to 1.8 mm).
* Food-contact parts (chamber, tubing, pump heads, pod shell) must be food-grade; the BOM lists hobby-grade parts for the prototype only.

## Not covered

No electrical-safety or medical-device certification work has been done, and mains wiring should be reviewed by a qualified person before any powered test.
