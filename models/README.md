# models

`ikwath_process_model.ipynb` is a Python notebook that checks the iKwath design targets against basic physics. It needs only NumPy and Matplotlib. All figures are calculated from design parameters, not measured on a machine.

| Section | Question it answers |
|---|---|
| Vacuum boiling point | At the chamber pressures in the design, what temperature does water boil at? |
| Energy budget | How much heater power does a given cycle time need? |
| Heat-up and reduction simulation | How does temperature and evaporated mass evolve for different heater sizes? |
| Endpoint detection | How much does load-cell noise shift the mass-based stopping point? |
| Pod tag payload | Encode, decode and validate the 16-byte tag profile used by the firmware |

## What it currently shows

* Vacuum at 50 to 60 kPa puts boiling at roughly 81 to 86 C, under the 90 C ceiling.
* Latent heat is about 87 percent of the energy needed. The 50 W heater in the bill of materials needs hours for the full reduction; a 12 minute cycle needs on the order of 1.4 kW into the water. This is an open design decision (see `docs/limitations.md`).
* A short moving average keeps the endpoint within the 2 g tolerance for load-cell noise up to a couple of grams rms; measure the real noise floor on the bench.

## Run

```bash
cd models
jupyter notebook ikwath_process_model.ipynb
```

Every assumption sits in the first code cell (heater power, efficiency, temperatures, masses) so it can be replaced with bench data.
