# Anticipated questions

**Q1. Why not just simmer it on a stove?**
A classical Kwatha needs 45 to 60 minutes of attended simmering and is only fresh for a few hours, so people buy preserved syrups instead. iKwath aims to make a fresh, correctly reduced single dose on demand.

**Q2. Boiling faster usually means hotter. How do you stay within classical gentle heat?**
By lowering pressure rather than raising temperature. At 0.5 to 0.6 atm water boils at about 82 to 86 C, so the liquid boils vigorously inside the 85 to 90 C envelope. A wide shallow pan, recirculation and a vapor exhaust add speed without heat.

**Q3. How does the machine know when the decoction is done?**
By mass. A load cell tracks the chamber; when 300 g has evaporated from 400 g of water, the reduction is 4:1 and the boil stops. That follows the real evaporation rate instead of a fixed timer.

**Q4. Is it safe?**
Three protections act with the firmware off: a lid interlock and a 95 to 100 C bimetallic cutoff, both in series with the heater, plus a 5 A fuse on the 12 V rail. Firmware adds boil-over, level and over-temperature supervision on top. No mains-side safety review has been done yet.

**Q5. How do you know the drink matches classical Kwatha?**
We do not claim that yet. The design follows pharmacopoeial ratios, particle size and reduction, and it monitors dissolved solids as a consistency proxy. Extract quality has not been tested in a lab, and that is the main next step.

**Q6. Have you built it?**
The hardware is specified in detail, the PCBs exist in several KiCad iterations, a first-draft ESP32 firmware implements the full cycle, and two web front ends are runnable. The firmware has not yet run on the appliance, and a physical prototype is not built. The dashboard's brew is a simulation. `docs/limitations.md` lists the gaps and how to close each.

**Q7. What does a pod add?**
Dose accuracy (a pre-measured, particle-graded charge), a built-in filter, and an NFC tag carrying the brew profile so the machine picks the right ratio, soak, temperature and reduction. It also gives a route to traceability.

**Q8. What does the appliance cost to build?**
The prototype electronics come to about INR 4,588 by the bill of materials, excluding enclosure, chamber, food-grade parts and PCB fabrication. The figure needs a spot-check against supplier prices.
