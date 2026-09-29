# Pod design and extraction physics

## Kwatha in one paragraph

Kwatha (Kashaya, Kadha) is the classical aqueous decoction: coarse powder (yavakuta churna) boiled in a multiple of water and reduced, then strained. It is not shelf-stable, so the value of iKwath is making a fresh, correctly reduced dose as easy as a capsule machine.

## Pharmacopoeial parameters

| Herb class | Botanical parts | Water ratio | Reduction | Concentration |
|---|---|---|---|---|
| Soft (Mridu) | leaves, flowers, tender herbs | 16 parts | to 1/4 | about 4x |
| Medium (Madhyama) | bark, stems, soft wood | 8 parts | to 1/4 | about 2x |
| Hard (Kathina) | roots, dense heartwood | 4 parts | to 1/4 | about 1x |

Outpatient working profile: 400 mL of water reduced to 100 mL (4:1 by volume).

## Particle size (PCIM&H AFI/2020/KC/1.0)

* 100 percent through IS sieve 22 (710 micron).
* No more than 10 percent through IS sieve 44 (355 micron).
* Reason: coarse granules let water penetrate without forming an impermeable bed, and avoid fine colloidal solids that would clog the mesh and cloud the drink.

## Temperature versus speed

Classical texts call for gentle heat (Manda Agni). Above 100 C volatile terpenes and thermolabile glycosides degrade. The design goal is to cut a 45 to 60 minute simmer to about 9 to 12 minutes while liquid stays at or below 85 to 90 C. The levers:

1. **Reduced pressure.** At 0.50 to 0.60 atm absolute (50 to 60 kPa) water boils at roughly 81 to 86 C. It boils vigorously, with convection, inside the temperature ceiling.
2. **Surface area.** A wide shallow pan (150 to 200 cm2) instead of a tall pot (about 50 cm2) roughly triples the evaporating surface.
3. **Boundary-layer stripping.** A small blower removes humid air from above the liquid.
4. **Recirculation.** A pump sprays liquid over the pod bed, removing diffusion boundary layers and hot spots.

These are design targets from the physics; no measured cycle time exists yet (see limitations).

## Pod

| Item | Specification |
|---|---|
| Charge | 25 to 35 g dry coarse herb |
| Filter | integrated mesh, 150 to 250 micron, keeps particles above 355 micron out of the drink |
| Shell | food-grade polypropylene or pressed cellulose, BPA-free, rated to 100 C in water |
| Seals | hermetic foil top and base seals |
| Identity | NTAG213 NFC tag (13.56 MHz) on the rim, read by an MFRC522 |

### NFC payload (design)

Formulation ID, herb-class water ratio (16:1, 8:1 or 4:1), pre-soak time (180 to 300 s), target evaporated mass, temperature setpoint (85 to 88 C) and target extract density (TDS or Brix threshold).

## Quality proxy

Total dissolved solids are read inline with an analog TDS sensor. This is a consistency proxy only. Marker fingerprinting (HPTLC) and quantitative extractive value (percent w/w) require a certified laboratory.
