# iKwath · HERB — Smart Pod-Based Ayurvedic Decoction Maker

> **Ministry of Ayush / All India Institute of Ayurveda (AIIA) · SIH 2026 Problem Statement 26048**\
> Frontend web application designed with a quiet luxury cafe aesthetic, tailored for Android mobile viewports and presentations.

***

## 🌿 Overview

**iKwath · HERB** is a smart appliance and mobile control surface that prepares fresh, standardized Ayurvedic decoctions (*Kwatha / Kadha*) in 12 minutes—strictly constrained to the classical **85–90°C Manda Agni (gentle heat)** temperature band using reduced-pressure vacuum boiling.

***

## ✨ Features

* **Pure Logo Intro with Blur Transition**: Starts with the centered brand logo on a natural sage stone background (`#BBC0A9`), gently breathing, and smoothly blurs out into the login screen.

* **Minimalist Login Page**: Borderless, breathable inputs with subtle hairline accents; includes Email, Password with eye toggle, and "Enter with Google".

* **Dynamic Time-Based Greeting**:

  * `Good morning` (5 AM – 12 PM) — *"A warm cup of kadha brings gentle balance to start your day."*

  * `Good afternoon` (12 PM – 5 PM) — *"Take a mindful pause with a refreshing, revitalizing cup of kadha."*

  * `Good evening` (5 PM – 9 PM) — *"Unwind your senses with a soothing, warm herbal decoction."*

  * `Good night` (9 PM – 5 AM) — *"Rest deeply as gentle healing herbs restore you overnight."*

* **Split Hero Card**: Clean typography on the left and an aesthetic beverage pod image framed in a white rounded container on the right.

* **In-App Active Pod Dropdown**: Clicking the active Kadha card smoothly expands an interactive accordion to select among certified blends (*Triphala Kadha*, *Ayush Kadha*, *Dashamoola Kadha*).

* **Interactive Extraction Engine**:

  * 5-stage progress: `Soak` → `85°C Boil` → `Reduce 4:1` → `Filter` → `Dispense`

  * Live countdown timer (e.g. `12:00 left`)

  * Real-time temperature dial and volume reduction gauge (400 mL → 100 mL)

  * Interactive Canvas PID temperature curve pinned to the 85–90°C safe zone

* **Notification Bell with Red Dot**: Lights up with an animated red dot when dispensing finishes.

* **Pod Camera Scanner**: Dedicated camera icon opening a viewfinder simulator to scan pod QR codes/NFC tags.

* **User Profile**: Profile section for **Tejas Kadam** (`tejaskadam@gmail.com`, Male) with paired device diagnostics.

* **Android Mockup Chassis**: Built-in toggle to view inside an authentic Android frame for capturing pitch deck screenshots.

***

## 🚀 How to Run the Project

This project is built using pure **HTML5, CSS3, and modern JavaScript** with **zero external dependencies** or build steps required.

### Method 1: Using Python (Recommended)

Run the built-in HTTP server from the `website` directory:

```bash
# 1. Navigate to the website directory
cd /Users/tanmaykadam/Desktop/dex/CS/Hackathon/SIH/website

# 2. Start the local server
python3 -m http.server 8080
```

Open your browser and navigate to:

```
http://localhost:8080
```

***

### Method 2: Using Node.js / npx serve

If you have Node.js installed:

```bash
cd /Users/tanmaykadam/Desktop/dex/CS/Hackathon/SIH/website
npx serve .
```

***

### Method 3: Direct File Opening (macOS)

You can open `index.html` directly in your default browser:

```bash
open /Users/tanmaykadam/Desktop/dex/CS/Hackathon/SIH/website/index.html
```

***

## 📂 Project Structure

```
website/
├── index.html         # Main application markup (all screens, tabs, modals)
├── styles.css         # Minimalist cafe design system & fluid transitions
├── app.js             # State machine, kinetics simulator, dynamic greetings
├── README.md          # Project documentation & run guide
├── logo.png           # Original HERB brand identity
├── research.md        # Technical research dossier for SIH PS 26048
├── app_and_website.md # Technical specification & system requirements
└── assets/
    ├── hero_drink.jpg         # High-res iced matcha/herbal beverage
    ├── pods_tray.jpg          # Travertine tray with herbal Kadha powders
    ├── warm_cup.jpg           # Steaming golden Ayurvedic decoction in ceramic cup
    ├── logo_transparent.png  # Transparent brand logo for light theme
    └── logo_white.png        # Transparent white brand logo
```

***

## 📱 Presentation Tips for Evaluators

1. **Android Screenshot Mode**: Use the **"Android Frame"** button on the top evaluator dock to switch between the framed phone chassis and full-screen view.
2. **Replay Intro**: Click **"Replay Intro"** on the floating dock to showcase the smooth logo blur entrance.
3. **Simulate Live Brew**: Click **"Begin Cycle"** to demonstrate real-time temperature regulation locked within the 85–90°C pharmacopoeial ceiling.
4. **Website Showcase**: Click **"Website"** in the top dock to toggle the public landing page explaining the 7-stage extraction kinetics.
