# iKwath: Push Log

A record of what each push to the repository did and why, newest first. Versioned notes are in [CHANGELOG.md](CHANGELOG.md).

Branch: `main`

| # | Commit | Date | Version | Summary |
|---|---|---|---|---|
| 4 | `2bfe716` | 2026-09-30 | 0.4.0 | Documentation set, README restructure, status corrections |
| 3 | `7036a7b` | 2026-09-21 23:26 | 0.3.0 | Final KiCad PCB design files and schematics |
| 2 | `80da5cf` | 2026-09-21 22:59 | 0.2.0 | Web applications, assets and project architecture |
| 1 | `238240f` | 2026-09-21 22:57 | 0.1.0 | First commit: README specification |

---

## Push 4: Documentation and status corrections (`2bfe716`, v0.4.0)

**Status:** committed to `main`. Message:

```
v0.4.0: add ESP32 firmware draft, docs set, INSTALL, CHANGELOG, PUSH_LOG, project report; correct README status claims
```

**Files:** README rewritten; new `ino/` firmware folder and `models/` notebook; new `INSTALL.md`, `CHANGELOG.md`, `PUSH_LOG.md`, `SIH26048_PROJECT_REPORT.md` and ten files under `docs/`. `ps_shortlist.md` is also untracked and is included only if you want it on the remote.

| Area | What was done | Why |
|---|---|---|
| README | Restructured; layout now lists real folders; added a Built-versus-Designed table | Reviewers should see exactly what exists |
| Claims | Removed the unverified "zero-error" rule-check claim (detail lives in `docs/pcb-status.md`); stated that the app simulates extraction and the firmware is untested | Statements must match the files |
| Firmware | Added `ino/ikwath_firmware` (state machine, PID heater, mass endpoint, NFC profile, safety checks, serial and HTTP interface) | The control sequence now has an implementation to bench-test |
| Models | Added `models/ikwath_process_model.ipynb`; its energy balance shows the heater is undersized for the cycle-time target | Check design targets against each other before building |
| BOM | Total quoted with its assumption | The CSV column is ambiguous between line total and unit price |
| Docs | Engineering set derived from the research notes and README | Each subsystem has one place to read about it |

**Verification:** figures were read from the repository files (rule-check reports, `BOM.csv`, git history, folder listings). The firmware was written but not compiled or run.

**Follow-ups:** decide whether to un-ignore and push `blehpcb/` and `blehread/`; fix `.claude/launch.json`; run DRC on the committed `pcb/` project and record the result; compile and bench-test the firmware; record a measured brew log.

---

## Push 3: Final KiCad PCB design (`7036a7b`, v0.3.0)

2026-09-21 23:26 IST. 5 files, 40,568 insertions, all under `pcb/` plus a `.gitignore` change.

**What went in:** `ikwath.kicad_pro`, `ikwath.kicad_sch`, `ikwath.kicad_pcb`, `ikwath.kicad_prl`, `fp-info-cache`, and a KiCad local-history folder.

**Why:** put the hardware design under version control so the schematic and board could be shared and reviewed.

**State:** the committed board contains routed copper (hundreds of track segments) but its rule-check results were not recorded in the repository.

---

## Push 2: Web applications and assets (`80da5cf`, v0.2.0)

2026-09-21 22:59 IST. 67 files, 7,034 insertions.

**What went in:** `app/` (50 files: landing page, stylesheet, imagery, hero video, asset backup with restore script) and `website/` (14 files: interactive app with `app.js`, manifest, assets, README), plus `.gitignore` and a launch configuration.

**Why:** give the hardware idea a visible product and app story for the presentation, including a live-looking extraction dashboard.

**State:** both front ends are static and run without dependencies. The extraction dashboard is a simulator.

---

## Push 1: First commit (`238240f`, v0.1.0)

2026-09-21 22:57 IST. 1 file, 422 insertions.

**What went in:** `README.md` with the full specification: pharmacopoeial background, extraction physics, pod and NFC design, fluidics, dual-PCB architecture, pin map, safety system, control state machine, application design, BOM and citations.

**Why:** fix the design in writing before building.

---

## Template for the next entry

```markdown
## Push N: <title> (<hash>, vX.Y.Z)

Date and size: `git show --stat --format='%h %ad' HEAD`

What went in:
Why:
Verification:
Follow-ups:
```
