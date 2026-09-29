# iKwath: Installation and Run Guide

How to run or open every part of the repository. Nothing here needs a build step except the optional PCB regeneration.

## 0. What you can run

| Part | Needs | Effort |
|---|---|---|
| `website/` interactive app | any browser, optionally Python 3 or Node | one command |
| `app/` product site | any browser, optionally Python 3 | one command |
| `pcb/` KiCad project | KiCad 8 or newer | open the project |
| `ino/` ESP32 firmware | Arduino IDE or arduino-cli with the ESP32 board package | optional |
| `blehpcb/` generators (local only) | KiCad's bundled Python | optional |

There is no backend to start. The ESP32 firmware is in `ino/` (section 3); it has not yet run on the appliance.

## 1. Interactive app (`website/`)

```bash
cd website
python3 -m http.server 8080
```

Open http://localhost:8080. Alternatives: `npx serve .` inside `website/`, or open `website/index.html` directly (macOS: `open website/index.html`).

What to try: let the logo intro finish, sign in with the demo form, tap the active pod card to switch between Triphala, Ayush and Dashamoola, start the extraction and watch the five stages, the temperature curve pinned to the 85 to 90 C band and the 400 to 100 mL gauge. The camera icon opens a pod-scanner viewfinder simulation. The profile screen shows placeholder account data.

## 2. Product site (`app/`)

```bash
cd app
python3 -m http.server 8123
```

Open http://localhost:8123. The page loads product images and `assets/herb_opt.mp4`; a slow first load is the video.

`app/_assets_backup/` holds unused media. Restore it with `./_assets_backup/RESTORE_ALL.sh` from `app/` if you need the originals.

## 3. ESP32 firmware (`ino/`)

1. Install the ESP32 board package in the Arduino IDE.
2. Install the libraries listed in [ino/README.md](ino/README.md).
3. Open `ino/ikwath_firmware/ikwath_firmware.ino`, choose ESP32 Dev Module, upload, and open the serial monitor at 115200 baud.
4. Tare and calibrate the load cell (`t`, then `cal <grams>`) before any test. Do not connect the heater or mains until the bench checks in the README pass.

The sketch is a first draft and has not been run on hardware.

## 4. PCB project (`pcb/`)

1. Install KiCad 8 or newer.
2. Open `pcb/ikwath.kicad_pro` (the `.kicad_pcb` and `.kicad_sch` open from the project window).
3. Run Inspect, Design Rules Checker and Electrical Rules Checker to see the current state; expected results are described in [docs/pcb-status.md](docs/pcb-status.md).

`pcb/.history/` is a KiCad local-history folder and is not part of the design.

## 5. Local PCB work (`blehpcb/`, gitignored)

These files exist only on the author's machine. To regenerate the scripted boards:

```bash
KPY="/Applications/KiCad/KiCad.app/Contents/Frameworks/Python.framework/Versions/Current/bin/python3"
$KPY blehpcb/pcbreal/control-board/build.py
$KPY blehpcb/pcbreal/power-board/build.py
```

Then re-run rule checks with `kicad-cli` (paths vary by KiCad version):

```bash
kicad-cli pcb drc --output drc.rpt blehpcb/pcbreal/control-board/control-board.kicad_pcb
```

The scripts place and connect components; some boards still have unrouted nets afterwards (see [docs/pcb-status.md](docs/pcb-status.md)).

## 6. Troubleshooting

| Symptom | Fix |
|---|---|
| Page loads but images are missing | Serve over HTTP from inside `website/` or `app/`, not from another folder |
| Port already in use | Pick another port, for example `python3 -m http.server 8090` |
| KiCad reports missing footprints | Some footprints are custom or renamed; assign library footprints in the Footprint Assignment tool |
| `.claude/launch.json` points to `landwebsite` | That folder does not exist; use `app` or `website` (see CHANGELOG) |
