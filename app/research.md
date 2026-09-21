# iKwath — SIH 2026 Research Dossier (PS 26048)

> Ministry of Ayush / All India Institute of Ayurveda (AIIA) · Hardware · MedTech/BioTech/HealthTech
> Last updated: 2026-09-18

---

## 1. Understand the problem before anything else

### 1.1 What a Kwatha actually is
Kwatha (kashaya/kadha) is the classical Ayurvedic **aqueous decoction**: coarse powder (yavakuṭa cūrṇa) of one or more herbs is boiled in water, reduced down, and strained. It is the most prescribed Ayurvedic dosage form and, critically, **it must be drunk within a few hours of preparation** — it isn't shelf-stable like an arishta or a churna tablet. That single fact is the whole business case for iKwath: people can't/won't make it fresh at home correctly, so they buy concentrated shelf-stable syrups that are weaker and easier to adulterate. Your device exists to make "fresh, correct-dose Kwatha" as easy as a Nespresso shot.

### 1.2 The classical preparation rule (API/AFI general method)
This is the exact mechanism the jury will expect you to know cold — quote it, don't paraphrase it vaguely.

**Standard API (Ayurvedic Pharmacopoeia of India) general rule for Kwatha:**

| Drug hardness (per yavakuṭa cūrṇa) | Water added (parts, by weight of drug) | Boiled & reduced to | Effective concentration factor |
|---|---|---|---|
| Soft / leafy / flower drugs | 16 parts water | 1/4 part (i.e. to ¼ of the *added water*, not of drug) | ~4x |
| Medium-hard drugs (stems, barks) | 8 parts water | 1/4 part | ~2x |
| Hard drugs (roots, heartwood) | 4 parts water | 1/4 part | ~1x |

Your PS statement simplifies this to a single working case: **~400 mL water → 1 litre boiling chamber → reduced to ~100 mL dose (a 4:1 reduction)**, which corresponds to the "soft drug, 16-parts-water" class — the most common category in outpatient formulary use. Design your default profile around this, but your pod's encoded profile (see §3) must be able to override the ratio and reduction target per formulation, because hard-drug decoctions use a different starting ratio.

**Powder grade (yavakuṭa cūrṇa) — PCIM&H AFI/2020/KC/1.0 spec:**

| Parameter | Spec |
|---|---|
| Passes through | IS Sieve No. 22 (710 µm) — **100% must pass** |
| Retained coarse fraction | Not more than 10% passes through IS Sieve No. 44 (355 µm) — i.e., it must stay *coarse*, not powder-fine |
| Purpose of grade | Coarse grind maximizes surface area for extraction while still being retainable by a straining cloth/pod mesh |

This directly informs your pod mesh design (§ below): the pod's internal filter mesh should be sized to *just* retain 355–710 µm particles without choking flow — think ~150–250 µm mesh, similar order to a French-press/tea-pod mesh, not a coffee paper filter.

### 1.3 The real engineering problem (read this twice — it's what separates finalist teams from eliminated ones)
The PS is *not* "build a kettle with a timer." It is an **extraction kinetics problem constrained by a pharmacopoeial identity test.** Read the Expected Solution section again: *"any acceleration has to be validated to reproduce the classically prepared decoction's extract density and constituent profile."* That means:

1. You cannot just crank the heat to boil faster — Ayurvedic decoctions are traditionally prepared at a **gentle/mild boil (~85–90°C, i.e. sub-100°C, "manda agni")**, not a rolling boil, because many actives (glycosides, volatile constituents, certain alkaloids) degrade or volatilize at/above 100°C or with prolonged high heat. The PS explicitly caps you at 85–90°C.
2. The real bottleneck is **time**, not temperature. A classical kwatha takes 20–45+ minutes because reduction (4:1 evaporation) at atmospheric pressure and near-100°C is slow by nature (evaporation rate ∝ surface area × vapor pressure gradient, roughly). To cut cycle time *without* raising temperature, you must attack the **other variables** in the evaporation rate equation:
   - **Surface area** — spread the 400 mL into a thin film/large-surface geometry instead of a deep pool (a shallow wide pan evaporates far faster than a tall narrow cup for the same volume).
   - **Vapor removal** — actively pull humid air away from the liquid surface (a small fan/forced convection over the surface, or a vent with airflow) so the boundary layer doesn't saturate and stall evaporation.
   - **Agitation/recirculation** — continuously stir or recirculate the liquid through the powder bed so extraction isn't diffusion-limited; this also prevents hot-spots and scorching, and speeds mass transfer independent of bulk temperature.
   - **Reduced pressure (vacuum-assisted boiling)** — this is your strongest lever. Water boils at ~85–90°C when pressure is dropped to roughly 0.5–0.6 atm (~55–65 kPa absolute, per steam tables). Pulling a **mild, food-safe vacuum** with a small diaphragm/vacuum pump lets you boil vigorously (fast evaporation, fast reduction) while the liquid temperature never exceeds ~85–90°C — this is literally rotary-evaporator (rotovap) principle, used industrially to concentrate heat-labile herbal extracts without degrading them. **This is almost certainly the "correct" hero mechanism the PS is hinting at** with the line *"reduced-pressure (lower-temperature) evaporation."* Build your differentiator around this.
3. Whatever speed-up method you choose, the PS explicitly wants a **validation story**: constituent profile (qualitative, e.g. TLC/HPTLC fingerprint or marker-compound match) and extractive yield (quantitative, e.g. % w/w extractive value per API test methods, or extract density via refractometer/Brix as an on-device proxy) must match the classical 45–60 min preparation. You will not be able to do real HPTLC in a hackathon, but you MUST show in your PPT/report that you *understand* this is the actual success metric, and propose extract density monitoring (see §5 sensors) as your real-time proxy control variable — this is explicitly invited by the PS text ("the appliance monitors weight and extract density so every dose is consistent").

### 1.4 Why "pod-based" — and what problem it solves besides convenience
- **Dosing accuracy & adulteration**: a sealed, certified pod removes the two biggest failure points of home/clinic kwatha — wrong powder grade and wrong quantity.
- **The pod doubles as the brew-bag/filter**, so there's no loose powder to clean and no risk of fine particulate carrying into the dose (important because fine particulate in a decoction is itself an API-defined defect).
- **Formulation flexibility**: AIIA/OPD Ayurvedic pharmacies stock dozens of kwatha churna formulations (Dashamoola, Trikatu-based, Guduchi, etc.) — a pod system means one appliance serves all of them, the pod just carries the *profile* (ratio, temperature ramp, time, reduction target) as data.
- **Certifiability**: the PS says pods "are certifiable against the API/AFI by AIIA/PCIM&H" — meaning your business model piggybacks on an existing regulatory pathway (like how Nespresso pods are QC'd at the factory, not by the consumer).

---

## 2. SIH-specific strategy (how selection actually works — read this before designing)

### 2.1 The stage that actually eliminates most teams is the **idea/PPT round**, not the grand finale
- SIH runs in two effective gates: (a) internal college shortlisting / SIH portal idea submission (PPT, ~6 slides, PDF only), and (b) if selected, the 36-hour Grand Finale hackathon with a working prototype demo.
- On a popular PS, 400–500+ teams may submit for as few as 5-ish seats at the internal round — **decided purely by a 6-slide PDF, no live pitch, no demo.** This means your PPT has to stand entirely on its own: no verbal narration will save a confusing slide.
- Because PS 26048 is a Ministry of Ayush hardware PS with real pharmacopoeial depth (not a generic "app for X" PS), it is likely to attract **fewer but more technically serious teams** than trendy software PS's. That is an advantage for a team that goes deep on the actual pharmaceutics — most competing teams will treat it as "a kettle with an app" and miss the extraction-kinetics/validation angle entirely. Winning strategy: **be the team that clearly understands API/AFI pharmacognosy, not just electronics.**

### 2.2 PPT/idea submission format rules (follow exactly — deviation risks disqualification)
- **Strict slide limit** (commonly 6 slides in the official AICTE/SIH template) — do not add slides, do not change the provided template's fonts/theme.
- **PDF only.** No PPTX, no Word doc uploads accepted by the portal.
- Content should be **visual and information-dense, not paragraph-dense** — diagrams, flow charts, exploded views, block diagrams, tables (like the ones in this doc) score far better than prose bullets.
- Typical mandated sections (map directly onto slides):
  1. **Title slide** — PS ID/title, team name, team leader, category (Hardware).
  2. **Problem understanding & idea/proposed solution** — restate the AFI/API constraint in your own words + your one-line solution ("pod-based vacuum-assisted rapid decoction maker").
  3. **Technical approach** — block diagram: mechanical + electronics + firmware + app, and your process flow (soak → vacuum-assisted controlled boil → reduce → filter → dispense).
  4. **Feasibility & viability** — risks (heat-labile actives, vacuum seal cost, pod cost/certification) + mitigation.
  5. **Impact & benefits** — clinics/OPDs, elderly/home users, standardization & anti-adulteration, Ayush export/panchakarma centers.
  6. **Research/references** — cite PCIM&H AFI/2020/KC/1.0, API monographs, AIIA — this signals domain rigor to Ayush-affiliated evaluators.

### 2.3 How SIH actually "reads" your PDF/watches your video — practical implications
- Portal submissions are typically reviewed as **rendered PDF pages** by human evaluators (nodal-officer/SPOC panels, plus domain experts from the sponsoring ministry) — assume a **human skims each slide for 15–30 seconds**, not that an AI/OCR engine "scans for keywords." That said:
  - Use **real text, not text-baked-into-images** for headings/body — some portals/evaluator tools do run keyword/plagiarism checks and copy-paste text extraction across submissions; an unselectable PDF (flattened image-only slide) can fail those automated similarity/keyword passes and looks lazy to a human reviewer besides.
  - Keep file size reasonable; portals often cap upload size (~5–10 MB) — compress images/diagrams.
  - Filename convention matters cosmetically — follow whatever the portal specifies (often `PSID_TeamName.pdf`).
- **Video round** (used at Grand Finale / some ministry-specific pre-screening rounds, and always useful to have ready): assume it is **watched at 1x by a person**, often multiple videos back-to-back in a review sprint — so:
  - Keep it **2–4 minutes**, hook in the first 10 seconds (don't open with a team intro; open with the problem — "a fresh kadha degrades in hours, but making one correctly takes 45 minutes and a trained hand").
  - Show the **physical device working** on camera as early as possible — judges are checking "is this real or just a rendering?" A CAD render alone reads as vaporware; even a rough 3D-printed shell with visible internals earns more trust than a polished animation.
  - Overlay **captions/labels** on screen (pod insertion, vacuum stage, temperature readout, dispensed dose) — most judges skim with sound off in a first pass.
  - Never hardcode/fake a demo value — cut a visible sensor reading (temperature, weight) into frame; a judge who suspects a staged number will mark down "feasibility."
  - End with impact numbers (cycle time achieved vs classical 45–60 min, e.g. "9–12 min," dose consistency ±X%) — quantified claims outperform adjectives.

### 2.4 Who evaluates PS 26048
SIH does not publish individual evaluator names publicly before the event, and no name-level information exists yet for SIH 2026 at the time of this research. What is knowable and should shape your prep:
- The **sponsoring organization is Ministry of Ayush, department All India Institute of Ayurveda (AIIA)** — evaluation panels for ministry-sponsored PS's are typically staffed by a mix of: (a) the ministry's own nodal officer/SPOC for that PS, (b) subject-matter experts from the sponsoring institute (here, likely AIIA faculty/Ayurvedic pharmaceutics or Rasa Shastra & Bhaishajya Kalpana department scientists, and possibly PCIM&H standardization scientists since the PS explicitly names PCIM&H certification), and (c) AICTE/SIH-appointed technical mentors (usually academics/industry engineers) for the hardware/electronics side.
- **Practical implication**: your panel is very likely **half pharma-science literate, half engineering literate.** Don't over-index on pure electronics flexing (a beautiful PCB won't matter if you show boiling temperature 100°C and confidently call it "standard") — pair every engineering claim with the correct pharmacopoeial term (yavakuṭa cūrṇa, extractive value, manda/madhyama agni) so both halves of the panel see you did the homework.
- Cross-check the official channel closer to the deadline: `sih.gov.in` publishes the SPOC/mentor list per PS during the mentorship phase — monitor it, don't guess further.

### 2.5 General "hacks" that consistently separate winning SIH teams (cross-referenced across multiple winner write-ups/playbooks)
1. **PS selection asymmetry** — a technically deep PS with a real institutional sponsor (like this one) draws fewer copy-paste "ChatGPT idea" teams than trendy software PS's; lean into the depth rather than trying to look "more techy."
2. **Depth over buzzword density** — judges explicitly call out that cramming in AI/ML/blockchain buzzwords without grounding in the actual problem backfires; anchor every feature to the PS's literal Expected Solution bullets.
3. **A working, ugly prototype beats a beautiful non-working render.** For hardware PS's this is even more true — bring a physical unit that visibly boils/reduces/dispenses, even if the enclosure is 3D-printed/unpainted.
4. **Never let a hardcoded/rigged demo be visible** — panels actively probe for this; feed it a different pod/parameter live if asked.
5. **Time-box every pitch section** — presentations that run over get cut off mid-technical-explanation before the "impact" slide is ever seen; rehearse to finish under the limit with margin.
6. **Always carry an offline backup** (recorded video + PDF on a USB, no dependency on venue wifi) — connectivity failure is a recurring reason strong teams under-deliver live.
7. **Quantify impact** — cycle-time reduction %, cost per dose vs commercial arishta, number of AIIA/Ayush OPDs as addressable market, etc. Numbers read as credibility.

---

## 3. System overview — process table (powder in → dose out)

| Stage | What happens | Target parameter | Sensors/actuators involved |
|---|---|---|---|
| **1. Pod insertion & ID** | User inserts single-dose pod (contains yavakuṭa cūrṇa of the formulation) into the pod bay; lid closes and locks | Pod formulation profile read | RFID/NFC or barcode reader on pod, lid microswitch + solenoid lock |
| **2. Water fill** | Appliance meters ~400 mL potable water (API Jala standard) into the 1 L boiling chamber, wetting the pod | Water volume per profile (varies by formulation ratio) | Water level sensor / flow meter, inlet solenoid valve, load cell (weight-based confirmation) |
| **3. Soak** | Powder is allowed to hydrate for a short pre-soak (improves extraction rate before heat is applied) | 3–5 min, ambient–40°C | Temperature sensor (chamber), timer (firmware) |
| **4. Controlled mild boil** | Heater brings liquid to 85–90°C and holds; vacuum pump (if used) drops chamber pressure so boiling occurs at this temperature without exceeding it | 85–90°C, held within pressure/temp band | PT100/thermistor probe, heating element, vacuum pump + pressure sensor, PID control loop |
| **5. Active reduction** | Liquid is reduced from ~400 mL to ~100 mL (4:1) via continuous mild boil + forced vapor removal (fan/vent) + optional recirculation pump to keep extraction diffusion-limited-free | Reduce to ¼ volume / target extract density (Brix) | Load cell (mass-loss tracking = evaporated volume proxy), optical/refractometric density sensor, small extraction fan/vent, recirculation micro-pump |
| **6. Filter** | Reduced liquid is drawn/pressed through the pod's integrated mesh filter, separating spent powder from clear decoction | Particulate-free dispense | Pod mesh (150–250 µm), outlet pump/solenoid |
| **7. Dispense** | ~100 mL warm, fresh decoction dispensed into user's cup | ~100 mL ± tolerance, drinkable warm temp (~50–60°C) | Load cell (dose volume/weight confirmation), outlet valve |
| **8. Eject & clean** | Spent pod ejected into waste bin; boiling chamber + filter get a rinse cycle (dishwasher-safe parts removable) | Chamber clean for next cycle | Pod ejector mechanism, rinse-water solenoid |
| **Safety, throughout** | Anti-boil-over (foam/level sensing cuts heater), dry-run protection (no-water lockout), over-pressure/vacuum relief | Fail-safe at all times | Level/foam sensor, dry-run thermal cutoff, pressure relief valve |

### 3.1 Managing the "temperature must not exceed 85–90°C" constraint — how you actually do it
This is the single most-scrutinized engineering claim in your project. Present it as **three complementary control layers**, not one:

1. **Closed-loop PID temperature control** (baseline, cheapest, every team will have this): a food-grade thermistor/PT100 in the liquid feeds a PID loop driving the heater (PWM via SSR/MOSFET) to hold 85–90°C. This alone prevents *overshoot* but does not, by itself, speed up reduction — atmospheric boiling point of water is 100°C, so at 85–90°C you're not even boiling, just evaporating slowly (like a warm cup). This is the naive approach and explains why classical kwatha takes 45+ minutes.
2. **Reduced-pressure (vacuum-assisted) boiling — your differentiator**: use a small food-safe diaphragm vacuum pump to drop the sealed chamber to roughly 0.5–0.6 atm absolute. At that pressure, water's *boiling point itself* drops to ~85–90°C (standard steam-table relationship), so the liquid can **actually boil vigorously** (fast bubble-driven evaporation, fast reduction, fast extraction) while never thermally exceeding the 85–90°C ceiling the PS specifies. This is the same working principle as a rotary evaporator (rotovap) used in labs/industry to concentrate heat-sensitive extracts. State this explicitly in your PPT — it shows you solved the "shorten time without altering quality" requirement with a real physical mechanism, not just "better firmware."
3. **Surface-area + forced vapor removal + recirculation** (secondary boosters, cheap to add, compound with #2): a shallow/wide evaporation tray geometry inside the chamber, a small extraction fan pulling humid air off the liquid surface (vented outside, condensate optionally captured/discarded — do not let condensate drip back and dilute the dose), and a small recirculation pump continuously pushing liquid through the pod bed so extraction isn't diffusion-limited. None of these raise temperature — they only speed mass transfer.

**Anti-overheat safety net**: independent of the control loop, add a hardware thermal cutoff (bimetallic or thermal fuse rated ~95–100°C) in series with the heater as a fail-safe — this is standard appliance safety practice and a evaluator will expect to see it even briefly mentioned.

### 3.2 On-device validation proxy (answers "how do you know the dose is still correct?")
You cannot run HPTLC on a countertop appliance. Propose, and be upfront about the limitation:
- **Extract density (°Brix via optical refractometer sensor, or a simple inline conductivity/TDS-style proxy)** as a real-time surrogate for "extractive value" — API defines extractive value (% w/w) as a key QC parameter for kwatha churna; density/Brix correlates with total dissolved extractives and is a widely used industrial proxy in herbal concentrate production.
- **Load-cell-tracked mass loss** during reduction as a cross-check that the ¼-volume reduction endpoint was actually reached (evaporated mass ≈ initial water mass − final dose mass, corrected for absorbed water in spent powder).
- Clearly state in your PPT that **full API-compliance validation (extractive value %, HPTLC marker-compound fingerprint) would be done at the pod-certification stage by AIIA/PCIM&H on the batch/formulation level**, not per-cup — the appliance's job is to *reliably reproduce* a pre-validated process (time/temp/pressure/ratio profile), not to re-certify each cup. This is an important nuance: it matches how the PS frames pod certification ("Pods... are certifiable against the API/AFI by AIIA/PCIM&H") and shows you understand the QC boundary between appliance and pharmacy.

---

## 4. What to build — three deliverables

| Deliverable | Purpose | Detailed doc |
|---|---|---|
| **CAD (mechanical/industrial design)** | Full appliance shape, boiling chamber, pod bay, exploded internal architecture | [structure.md](structure.md) |
| **PCB / electronics** | Sensor suite, control board, power stage, firmware architecture | [pcb.md](pcb.md) |
| **App + backend/website** | Pod profile management, cycle control/monitoring, dose history, clinic/consumer modes | [app_and_website.md](app_and_website.md) |

---

## 5. Reference list (cite these in your PPT's "references" slide)
- PCIM&H, *AFI/2020/KC/1.0 — Formulary Specification of AYUSH Kvātha Cūrṇa* — pcimh.gov.in
- PCIM&H, *API-2/2020/KC/1.0 — Pharmacopoeial Monograph of AYUSH Kvātha Cūrṇa* — ccrum.res.in
- *The Ayurvedic Pharmacopoeia of India*, Part I & Part II (Formulations) — cdn.ayush.gov.in
- All India Institute of Ayurveda (AIIA) — institutional mandate on drug standardization, QC, safety evaluation
- General food-industry reference: vacuum/reduced-pressure evaporation & concentration principles (rotary evaporation, low-temperature vacuum concentration preserving heat-labile actives)

---

## 6. Open questions to resolve with your team before locking the PPT
1. Do we build a **single reference formulation profile** (e.g., a common Dashamoola-type kwatha) for the working prototype, and only *simulate* multi-formulation pod-switching in the app? (Recommended for a hackathon timeline — don't try to physically validate multiple herbs.)
2. Vacuum pump sourcing/cost vs a simpler "surface area + fan" only approach for the first prototype — decide the MVP tier vs the "full vision" tier and present both (MVP now, vacuum-assisted as the funded/production roadmap) if the vacuum BOM is too expensive/complex to get working reliably in the hackathon window.
3. Pod mechanical design: reusable brew-basket + disposable powder sachet (cheaper, easier to prototype) vs fully sealed single-use pod with integrated mesh (closer to the PS's literal wording, harder to prototype fast) — see [structure.md](structure.md) §pod design for the tradeoff table.
