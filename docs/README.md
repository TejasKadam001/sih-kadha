# iKwath documentation

Engineering notes derived from what is in this repository. Start with the top-level [README](../README.md); use these pages for detail.

| Document | Read it for |
|---|---|
| [pod-and-extraction.md](pod-and-extraction.md) | Pharmacopoeial ratios, particle size, vacuum boiling physics, pod and NFC design |
| [control-sequence.md](control-sequence.md) | The eight-step cycle, endpoint by mass, what firmware still has to settle |
| [pinout-and-buses.md](pinout-and-buses.md) | ESP32 pins, I2C addresses, relay assignment |
| [safety-system.md](safety-system.md) | Three hardware layers plus specified firmware supervision |
| [pcb-status.md](pcb-status.md) | Which board files exist, reported DRC and ERC results, deviations |
| [bom.md](bom.md) | Full parts table generated from `blehpcb/BOM.csv` and how to read the total |
| [app-and-cloud.md](app-and-cloud.md) | What the two web apps do today versus the intended app and cloud design |
| [../ino/README.md](../ino/README.md) | The ESP32 firmware: cycle, safety checks, serial commands |
| [../models/README.md](../models/README.md) | Process model notebook and what it shows |
| [limitations.md](limitations.md) | Twelve gaps and what closes each |
| [judge-qna.md](judge-qna.md) | Likely questions with straight answers |

Project-level documents: [SIH26048_PROJECT_REPORT.md](../SIH26048_PROJECT_REPORT.md), [INSTALL.md](../INSTALL.md), [CHANGELOG.md](../CHANGELOG.md), [PUSH_LOG.md](../PUSH_LOG.md).

Longer research notes (`blehread/`) and PCB generation work (`blehpcb/`) are gitignored and live only on the author's machine.
