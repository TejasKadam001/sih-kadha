# Limitations and what is needed to close them

| # | Limitation | Consequence | What closes it |
|---|---|---|---|
| 1 | Firmware is a first draft (`ino/`) | The control sequence and safety supervision are written but have not run on the appliance | Compile, bench-test each subsystem, then calibrate the values marked CALIBRATE in `config.h` |
| 2 | No physical prototype tested | The 9 to 12 minute cycle, 85 to 90 C envelope and 300 g endpoint are design targets, not measurements | Build the rig, log temperature, pressure and mass for full cycles, publish the curves |
| 3 | No extract quality data | Nothing shows the decoction matches classical hand preparation | Have samples analysed (extractive value, TDS versus reference, HPTLC fingerprint) at a certified lab |
| 4 | Web apps simulate the brew | The dashboard is illustrative and connects to nothing | The sketch offers an optional HTTP status and control interface; point the app at it, then add BLE and MQTT |
| 5 | Cloud, mobile client and catalogue are design only | No backend, no real formulation database | Build a small backend and source certified profiles |
| 6 | PCB not finalised | Several iterations exist; rule-check clean-up and routing are still open on some | Pick a variant, fix footprints, finish routing, store the final DRC and ERC reports |
| 7 | Bill of materials is prototype grade | Not food-grade or certified; enclosure, chamber and tooling not costed | Re-source food-contact parts and add mechanical costs |
| 8 | Vacuum plus load cell interaction untested | Chamber pressure and hose forces may bias the mass reading | Characterise with a static test under vacuum; add tare and compensation |
| 9 | Mains-side design unreviewed | Creepage, clearance and grounding not checked by a qualified person | Independent electrical review before any powered test |
| 10 | Heater power budget does not match the cycle-time target | Evaporating 300 g needs about 690 kJ of latent heat, and warming the water adds about 100 kJ. The 50 W heater in the bill of materials would need hours; a 12 minute cycle needs on the order of 1.4 kW into the water (`models/ikwath_process_model.ipynb`) | Choose a mains-rated heating element and rate the relay, wiring, cutoff and fuse for it, or reduce the evaporation target, or relax the cycle-time claim |
| 11 | Water volume versus pod charge | A 25 g pod at the herb-class ratios (16, 8, 4) needs 400, 200 or 100 mL of water, while the profile fixes 400 mL | Decide whether volume follows the class ratio or the pod charge changes with class, then align the profile, firmware and docs |
| 12 | Repository hygiene | `blehpcb/` and `blehread/` are gitignored; `.claude/launch.json` points to a missing folder; `pcb/.history` has a nested repository | Decide what to publish, fix the launch file, remove or ignore the nested repository |

## Scope statement

iKwath is a prototype concept for a standardised decoction appliance. It is not a certified medical or food-contact device, and nothing here is a health claim.
