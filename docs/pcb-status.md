# PCB status

The repository holds several iterations of the electronics. This page records what each contains and what its own reports say. Figures are copied from the report files; nothing here is a new measurement.

## Where the boards live

| Location | In git? | What it is |
|---|---|---|
| `pcb/` | yes (commit `7036a7b`) | KiCad project: `ikwath.kicad_sch`, `ikwath.kicad_pcb`, `ikwath.kicad_pro`. Contains routed copper (about 430 track segments). No rule-check report is stored with it. |
| `blehpcb/ikwath-control-board/`, `blehpcb/ikwath-power-board/` | no (ignored) | Generated KiCad 8 projects from `blehpcb/generate_final_kicad8.py`, with `*-drc.rpt` and `*-erc.rpt` |
| `blehpcb/pcbreal/` | no (ignored) | Scripted placement, routing and stackup iterations (`place_components.py`, `route_signals.py`, `stackup_and_planes.py`, `finish_board.py`), with Specctra `.dsn` and `.ses` files from an autorouter |

## Reported rule-check results

| Variant | ERC | DRC | Unconnected pads |
|---|---|---|---|
| `blehpcb/ikwath-control-board` | 0 errors, 64 warnings | 371 violations | not itemised |
| `blehpcb/ikwath-power-board` | 0 errors, 52 warnings | 259 violations | not itemised |
| `blehpcb/pcbreal/control-board` (`control-board.drc.rpt`) | | 1 violation | 56 |
| `blehpcb/pcbreal/control-board` (`DRC.rpt`, later run) | | 2 violations | 4 |
| `blehpcb/pcbreal/power-board` | | 5 violations | 62 |

What dominates the larger reports:

* Control board (371): clearance 152, silkscreen over copper 67, solder-mask bridge 52, silkscreen overlap 35, footprint mismatch 32, shorting items 21.
* Power board (259): silkscreen overlap 105, silkscreen over copper 59, clearance 33, library-footprint issues 21, solder-mask bridge 20, shorting items 12.
* The ERC warnings and many DRC items trace to footprints that are not found in the standard libraries (for example the ESP32 module footprint, terminal blocks and the fuse holder).

## Reading these honestly

* Earlier drafts of the README described the reports as zero-error. They are not: only ERC has 0 errors, and the DRC counts above are not zero.
* The later `pcbreal` reports (1 to 5 violations, 4 to 62 unconnected pads) are much cleaner than the `ikwath-*-board` reports, but they still have unrouted nets on the power board, and it is not recorded which variant is intended to be final.
* `shorting_items` entries must be examined before fabrication; they can indicate real overlaps.

## Known deviations from the specification (from the scripted iterations)

* Both PCF8574 expanders sit on the Control Board, so the inter-board link grew from 10 to 16 pins.
* The ESP32 is a socketed 2x19 header footprint rather than a soldered module; verify the pin order against your specific DevKit before soldering.
* Connectors are generic 2.54 mm headers rather than JST-XH.
* Generated board outlines are larger than the 70 x 60 mm and 80 x 70 mm in the specification.

## Before sending to a fabricator

1. Choose one variant and copy it into `pcb/` (or replace `pcb/`).
2. Assign real library footprints to every part reported as missing.
3. Route the remaining nets, then run DRC until only intended items remain.
4. Review clearances and creepage on the mains-side traces with a qualified reviewer.
5. Store the final DRC and ERC reports next to the board in git.
