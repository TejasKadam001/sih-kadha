# iKwath — App & Website

> Companion to [research.md](research.md), [structure.md](structure.md), [pcb.md](pcb.md).

---

## 1. Why the app matters to the judges (don't treat it as an afterthought)
The PS is a hardware PS, but SIH panels consistently reward teams that show a **full system story** — device + control/monitoring layer + a plausible path to real-world deployment (clinics, Ayush pharmacies, homes). The app is also where you demonstrate the two things text alone can't: **live sensor data during your demo video** (temperature/pressure/weight graphs proving the 85–90°C claim in real time) and **the pod-certification/traceability business model** (AIIA/PCIM&H-certified formulation profiles flowing from a backend into the device).

---

## 2. System components

| Component | Purpose | Recommended stack |
|---|---|---|
| **Mobile app** (primary control surface) | Pair with device (BLE for setup, WiFi for ongoing), select/confirm formulation, start cycle, live-monitor stage/temperature/time, dose history, clinic mode | React Native or Flutter (cross-platform, fast to build for a hackathon demo on one phone) |
| **Device firmware ↔ app link** | Telemetry + control commands | BLE for initial WiFi provisioning + local/offline control; WiFi (MQTT or simple REST/WebSocket to a local or cloud broker) for live streaming once connected | 
| **Backend / cloud** | Store pod formulation profiles (synced from AIIA/PCIM&H certified database), user accounts, dose logs, device fleet management (for clinic deployments with multiple units) | Node.js/Express or FastAPI + PostgreSQL; MQTT broker (e.g., Mosquitto/AWS IoT Core) for device telemetry at scale |
| **Website** | Public-facing product site + a light admin/clinic dashboard | Simple marketing site (product story, science/API-AFI credibility section, pods catalogue) + a web dashboard for clinics/pharmacies to manage multiple units and formulation stock |

---

## 3. Mobile app — feature table

| Feature | Description | Priority |
|---|---|---|
| **Device pairing (BLE → WiFi)** | Standard onboarding: scan for device, connect BLE, hand off WiFi credentials, device joins home/clinic network | Must-have |
| **Pod scan confirmation** | When user inserts a pod, app shows formulation name, dose, expected cycle time, reduction target — pulled from the NFC tag ID cross-referenced with the certified formulation database (cached locally, synced from cloud) | Must-have |
| **Live cycle dashboard** | Real-time stage indicator (soak/heat/reduce/filter/dispense), live temperature graph (line chart pinned to the 85–90°C band so users/judges visually see it never crossing the line), live pressure reading (if vacuum-assist implemented), countdown timer, extract-density progress toward reduction target | **Must-have — this is your strongest demo asset** |
| **Manual override / custom profile (advanced/clinic mode)** | For Ayurvedic physicians/pharmacists to adjust ratio/time within safe bounds for a formulation not yet pod-certified, with a clear "unverified profile" warning | Nice-to-have, shows depth |
| **Dose history & compliance log** | Log of every dose made — formulation, timestamp, extract density achieved, dose volume — useful for home users tracking a prescribed course, and for clinics for patient records | Should-have |
| **Reminders/scheduling** | Remind user to take a dose per their prescribed Ayurvedic regimen (e.g., before/after meals, specific times) | Nice-to-have |
| **Formulation library/catalogue** | Browse available certified pod formulations, their indications (from AFI monograph text), contraindications | Should-have |
| **Cleaning/maintenance reminders** | Prompt rinse cycle, filter replacement, descaling based on cycle count | Should-have |
| **Error/fault alerts** | Push notification on any safety fault (boil-over, dry-run, lid not sealed, pressure fault) with plain-language explanation | Must-have (ties directly to your safety story) |
| **Multi-device/clinic mode** | For an Ayush OPD/pharmacy running several units, a dashboard view of all machines' status, pod stock levels, usage analytics | Nice-to-have — strong "impact & scalability" slide material |
| **Offline mode** | Device can run a cycle standalone (physical buttons + onboard display) even with no app/network connection — app is a convenience layer, not a dependency | Must-have (mention explicitly — judges probe "what if wifi fails" during demos) |

---

## 4. Website — sections

| Section | Purpose |
|---|---|
| **Home/hero** | "Fresh, standardized Kwatha in minutes — not hours" — problem/solution framing |
| **How it works** | Visual process flow (pod in → soak → controlled boil → reduce → filter → dispense), reuse the process table from research.md |
| **The science** | Explain the AFI/API standard, the 85–90°C constraint, and the reduced-pressure evaporation mechanism in accessible language — this is your credibility section for Ayush-literate visitors/evaluators who click through |
| **Pods** | Catalogue of certified formulations, each with AIIA/PCIM&H certification badge/reference |
| **For clinics/pharmacies** | B2B pitch: consistency, anti-adulteration, throughput for OPD dispensing, fleet dashboard | 
| **For home use** | B2C pitch: convenience, freshness, dosing accuracy for patients on a prescribed Ayurvedic regimen |
| **Team/about** | SIH team info (for the submission/portal linkage) |
| **Admin/clinic dashboard (web app, gated login)** | Manage multiple devices, view fleet status, manage pod inventory/reorder, view aggregated dose analytics, push firmware/formulation-profile updates to devices |

---

## 5. How the app controls the PCB/device — protocol detail (for your technical-approach slide)

| Layer | Detail |
|---|---|
| **Provisioning** | BLE GATT service on ESP32 exposes WiFi-credential-write characteristic; app scans/connects via BLE, writes SSID/password, device reboots onto WiFi |
| **Ongoing control (local network)** | Device runs a lightweight local WebSocket/MQTT client; app on the same network talks to it directly (low latency, works without internet) — good "offline-first" story |
| **Ongoing control (remote/cloud)** | Device also connects to a cloud MQTT broker for remote monitoring/fleet management (clinic use case) — publishes telemetry topics (`device/{id}/temp`, `/pressure`, `/stage`, `/weight`) and subscribes to a command topic (`device/{id}/cmd`) |
| **Commands app → device** | `start_cycle(pod_id)`, `pause`, `abort`, `manual_override(params)` (gated), `run_rinse_cycle` |
| **Telemetry device → app** | Streamed every 1–2 seconds during an active cycle: stage, temperature, pressure (if vacuum), weight, extract density estimate, time remaining |
| **Formulation profile sync** | Device caches a local table of known pod-formulation profiles (ratio, time, temp, reduction target) keyed by NFC ID; app/backend syncs new/updated certified profiles down to the device whenever connected, so pod recognition still works offline for previously-seen formulations |

---

## 6. Demo-video framing tip (ties back to research.md §2.3)
Show the **live app temperature graph on screen next to the physical device** while it's actively reducing — this single shot does more to prove "we didn't fake the 85–90°C claim" than any slide of text. Also show the **NFC tap → formulation name pop-up on the app** as the very first interaction in your demo — it's a satisfying, legible "smart" moment for a judge watching at 1x speed.
