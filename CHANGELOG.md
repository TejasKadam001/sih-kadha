# iKwath Changelog

All notable changes are documented here. Format follows [Keep a Changelog](https://keepachangelog.com/en/1.0.0/); versions follow [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

Repository history: three commits on 2026-09-21 (`238240f`, `80da5cf`, `7036a7b`). Version 0.4.0 below is the documentation pass applied after them and is the next push; see [PUSH_LOG.md](PUSH_LOG.md).

---

## [0.4.0] - 2026-09-30

### Added
- `models/ikwath_process_model.ipynb`: vacuum boiling point, energy budget, heat-up and reduction simulation, load-cell endpoint noise study, pod tag encode/decode. Its energy balance flags that the 50 W heater cannot meet the cycle-time target.
- `ino/`: ESP32 firmware sketch (`ikwath_firmware`) implementing the brew cycle, safety checks, PID heater control, load-cell endpoint, NFC pod profiles, serial commands and an optional HTTP interface, plus `ino/README.md`. First draft, not yet run on hardware.
- `INSTALL.md`, `CHANGELOG.md`, `PUSH_LOG.md`, `SIH26048_PROJECT_REPORT.md`.
- `docs/` engineering set: pod and extraction, control sequence, pinout and buses, safety system, PCB status, bill of materials, app and cloud, limitations, judge Q&A.

### Changed
- `README.md` restructured: problem, solution, physics, working principle, dual-PCB architecture, safety, applications, BOM, a Built-versus-Designed status table, documentation map. Repository layout now matches the folders that actually exist.

### Corrected
- Removed the unverified "zero-error" rule-check claim; the actual state of each board iteration is recorded in `docs/pcb-status.md`.
- The README layout listed a `landwebsite/` folder that does not exist and omitted `app/`, `pcb/` and `docs/`; fixed.
- The BOM total of INR 4,588 is now stated with its assumption (cost column read as line totals; INR 8,335 if the figures were unit prices) and the item count corrected to 46.
- States plainly that the firmware is an untested draft and that the web app's extraction is a simulation.

### Noted, not changed
- `.claude/launch.json` still targets a `landwebsite` folder.
- `pcb/.history/` contains a nested git repository and an unrelated KiCad file name (`AGRITHON_2.0.kicad_pcb`).
- `blehpcb/` and `blehread/` are gitignored and therefore not on the remote.

---

## [0.3.0] - 2026-09-21

### Added
- Final KiCad PCB design files and schematics (`pcb/`): project, schematic, PCB, local-history cache. Commit `7036a7b`.

---

## [0.2.0] - 2026-09-21

### Added
- `website/`: interactive brew-simulator web app (HTML, CSS, JavaScript) with manifest and brand assets.
- `app/`: single-page product site with product photography, hero video and an asset backup with restore script.
- `.gitignore` and a Claude launch configuration. Commit `80da5cf`.

---

## [0.1.0] - 2026-09-21

### Added
- Initial README: problem context, Kwatha pharmacopoeial background, extraction physics, pod architecture, fluidics, dual-PCB architecture, safety system, control flow, application and IoT design, BOM, standards. Commit `238240f`.
