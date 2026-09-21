/* ==========================================================================
   iKwath · HERB — Clean Modern Application Controller
   Fluid Animations, In-App Dropdowns, Dynamic Greetings & Camera Scanner
   ========================================================================== */

document.addEventListener('DOMContentLoaded', () => {

  // ==========================================
  // 1. DATA: CERTIFIED KADHA FORMULATIONS
  // ==========================================
  const CERTIFIED_PODS = [
    {
      id: 'triphala',
      name: 'Triphala Kadha',
      ingredients: 'Amla, Haritaki, Bibhitaki',
      ratio: '8:1 Herbal Ratio',
      reduction: '4:1 Reduction',
      waterMl: 400,
      targetMl: 100,
      targetTemp: 88.0,
      targetBrix: 3.2,
      thumb: 'assets/warm_cup.jpg',
      indication: 'Gentle daily cleanse, digestion balance and eye health.'
    },
    {
      id: 'ayush_kwatha',
      name: 'Ayush Kadha',
      ingredients: 'Tulsi, Cinnamon, Dry Ginger, Black Pepper',
      ratio: '16:1 Herbal Ratio',
      reduction: '4:1 Reduction',
      waterMl: 400,
      targetMl: 100,
      targetTemp: 88.4,
      targetBrix: 3.4,
      thumb: 'assets/hero_drink.jpg',
      indication: 'Immunity defense and respiratory vitality.'
    },
    {
      id: 'dashamoola',
      name: 'Dashamoola Kadha',
      ingredients: 'Ten Sacred Roots Blend',
      ratio: '8:1 Root Ratio',
      reduction: '4:1 Reduction',
      waterMl: 400,
      targetMl: 100,
      targetTemp: 89.1,
      targetBrix: 3.8,
      thumb: 'assets/pods_tray.jpg',
      indication: 'Deep restorative blend for soothing muscles and stress.'
    }
  ];

  let activePod = CERTIFIED_PODS[0];

  // Brew State
  // 0: Standby, 1: Soak, 2: Boil, 3: Reduce, 4: Filter, 5: Dispense, 6: Complete
  let brewState = 0;
  let brewTimer = null;
  let brewSeconds = 0;
  const TOTAL_CYCLE_SECONDS = 72; // 72 ticks = 12 mins simulated

  let sensorData = {
    temp: 24.5,
    pressure: 1.00,
    volume: 400,
    brix: 0.8
  };

  const chartHistory = Array(25).fill(24.5);

  // ==========================================
  // 2. DOM REFERENCES
  // ==========================================
  const splashScreen = document.getElementById('splashScreen');
  const authScreen = document.getElementById('authScreen');
  const mainAppScreen = document.getElementById('mainAppScreen');
  const websiteScreen = document.getElementById('websiteShowcaseScreen');

  const replaySplashBtn = document.getElementById('replaySplashBtn');
  const toggleFrameBtn = document.getElementById('toggleFrameBtn');
  const viewportWrapper = document.getElementById('viewportWrapper');
  const frameModeText = document.getElementById('frameModeText');
  const switchViewWebsiteBtn = document.getElementById('switchViewWebsiteBtn');
  const returnToAppBtn = document.getElementById('returnToAppBtn');
  const profileShowcaseTrigger = document.getElementById('profileShowcaseTrigger');

  // Dynamic Greeting elements
  const dynamicGreetingHeading = document.getElementById('dynamicGreetingHeading');
  const dynamicGreetingThought = document.getElementById('dynamicGreetingThought');

  // Top Bar Actions (Camera & Bell with Red Dot)
  const cameraScanBtn = document.getElementById('cameraScanBtn');
  const notifBellBtn = document.getElementById('notifBellBtn');
  const notifRedDot = document.getElementById('notifRedDot');

  // Auth & Profile User
  const authForm = document.getElementById('authForm');
  const authName = document.getElementById('authName');
  const authEmail = document.getElementById('authEmail');
  const authContinueBtn = document.getElementById('authContinueBtn');
  const googleSignInBtn = document.getElementById('googleSignInBtn');
  const togglePasswordBtn = document.getElementById('togglePasswordBtn');
  const authPassword = document.getElementById('authPassword');
  const profileUserName = document.getElementById('profileUserName');
  const profileUserEmail = document.getElementById('profileUserEmail');
  const profileUserInitials = document.getElementById('profileUserInitials');

  // Navigation (4 Tabs: Brew, Kadhas, Journal, Profile)
  const dockButtons = document.querySelectorAll('.dock-btn-minimal');
  const tabPages = document.querySelectorAll('.tab-page-content');

  // Active Pod Dropdown
  const dropdownPodSection = document.getElementById('dropdownPodSection');
  const activePodTrigger = document.getElementById('activePodTrigger');
  const activePodName = document.getElementById('activePodName');
  const activePodIngredients = document.getElementById('activePodIngredients');
  const activePodThumb = document.getElementById('activePodThumb');
  const podDropdownList = document.getElementById('podDropdownList');

  // Extraction Engine
  const cycleActionBtn = document.getElementById('cycleActionBtn');
  const cycleBtnLabel = document.getElementById('cycleBtnLabel');
  const cycleStateText = document.getElementById('cycleStateText');
  const countdownTimer = document.getElementById('countdownTimer');
  const stepperProgressBar = document.getElementById('stepperProgressBar');
  const liveTempDisplay = document.getElementById('liveTempDisplay');
  const liveVolumeDisplay = document.getElementById('liveVolumeDisplay');
  const tempBandBadge = document.getElementById('tempBandBadge');
  const rinseCycleBtn = document.getElementById('rinseCycleBtn');
  const abortCycleBtn = document.getElementById('abortCycleBtn');
  const heroStartCycleBtn = document.getElementById('heroStartCycleBtn');
  const tempChartCanvas = document.getElementById('tempChartCanvas');

  // Camera Modal
  const cameraScanModal = document.getElementById('cameraScanModal');
  const closeCameraModalBtn = document.getElementById('closeCameraModalBtn');
  const simulateScanSuccessBtn = document.getElementById('simulateScanSuccessBtn');
  const profileCameraTrigger = document.getElementById('profileCameraTrigger');

  // Additional sections
  const podsCatalogueList = document.getElementById('podsCatalogueList');
  const historyCardsGroup = document.getElementById('historyCardsGroup');
  const toastNotification = document.getElementById('toastNotification');
  const toastMessage = document.getElementById('toastMessage');

  // ==========================================
  // 3. DYNAMIC GREETING BASED ON TIME OF DAY
  // ==========================================
  function updateDynamicGreeting() {
    if (!dynamicGreetingHeading || !dynamicGreetingThought) return;
    const now = new Date();
    const hour = now.getHours();

    if (hour >= 5 && hour < 12) {
      dynamicGreetingHeading.textContent = 'Good morning';
      dynamicGreetingThought.textContent = '"A warm cup of kadha brings gentle balance to start your day."';
    } else if (hour >= 12 && hour < 17) {
      dynamicGreetingHeading.textContent = 'Good afternoon';
      dynamicGreetingThought.textContent = '"Take a mindful pause with a refreshing, revitalizing cup of kadha."';
    } else if (hour >= 17 && hour < 21) {
      dynamicGreetingHeading.textContent = 'Good evening';
      dynamicGreetingThought.textContent = '"Unwind your senses with a soothing, warm herbal decoction."';
    } else {
      dynamicGreetingHeading.textContent = 'Good night';
      dynamicGreetingThought.textContent = '"Rest deeply as gentle healing herbs restore you overnight."';
    }
  }
  updateDynamicGreeting();

  // ==========================================
  // 4. PURE INTRO — SMOOTH CINEMATIC BLUR
  // ==========================================
  let introTimeout = null;

  function runIntro() {
    splashScreen.classList.remove('blur-out');
    showScreen(splashScreen);

    if (introTimeout) clearTimeout(introTimeout);
    // Serene hold for 2.0s, then smooth non-snappy blur transition
    introTimeout = setTimeout(blurOutToLogin, 2000);
  }

  function blurOutToLogin() {
    if (introTimeout) clearTimeout(introTimeout);
    splashScreen.classList.add('blur-out');
    setTimeout(() => {
      showScreen(authScreen);
    }, 800);
  }

  if (splashScreen) splashScreen.addEventListener('click', blurOutToLogin);
  if (replaySplashBtn) replaySplashBtn.addEventListener('click', runIntro);

  runIntro();

  function showScreen(target) {
    [splashScreen, authScreen, mainAppScreen, websiteScreen].forEach(s => {
      if (s) s.classList.remove('active');
    });
    if (target) target.classList.add('active');
  }

  // ==========================================
  // 5. MINIMAL LOGIN FLOW
  // ==========================================
  if (togglePasswordBtn && authPassword) {
    togglePasswordBtn.addEventListener('click', () => {
      if (authPassword.type === 'password') {
        authPassword.type = 'text';
        togglePasswordBtn.style.opacity = '1';
      } else {
        authPassword.type = 'password';
        togglePasswordBtn.style.opacity = '0.5';
      }
    });
  }

  function enterApp() {
    const rawName = authName ? authName.value.trim() : '';
    const enteredName = rawName || 'Tejas Kadam';
    const enteredEmail = (authEmail && authEmail.value.trim()) || 'tejaskadam@gmail.com';

    // 1. Update Profile User Name & Email
    if (profileUserName) {
      profileUserName.textContent = enteredName;
    }
    if (profileUserEmail) {
      profileUserEmail.textContent = enteredEmail;
    }

    // 2. Update Profile Avatar Initials (e.g. "Tanmay Kadam" -> "TK", "Tanmay" -> "T")
    if (profileUserInitials) {
      const nameParts = enteredName.split(/\s+/).filter(Boolean);
      let initials = 'TK';
      if (nameParts.length >= 2) {
        initials = (nameParts[0][0] + nameParts[nameParts.length - 1][0]).toUpperCase();
      } else if (nameParts.length === 1) {
        initials = nameParts[0].slice(0, 2).toUpperCase();
      }
      profileUserInitials.textContent = initials;
    }

    // 3. Popup changes to "Welcome, <Name>"
    showToast(`Welcome, ${enteredName}`);

    // 4. Transition to main screen
    showScreen(mainAppScreen);
    updateDynamicGreeting();
    setTimeout(drawMinimalChart, 100);
  }

  if (authForm) {
    authForm.addEventListener('submit', (e) => {
      e.preventDefault();
      enterApp();
    });
  }

  if (authContinueBtn) {
    authContinueBtn.addEventListener('click', (e) => {
      e.preventDefault();
      enterApp();
    });
  }

  if (googleSignInBtn) googleSignInBtn.addEventListener('click', enterApp);

  // ==========================================
  // 6. TOP BAR ACTIONS: CAMERA SCANNER & NOTIFICATION BELL
  // ==========================================
  // Open camera scanner modal with Android WebView safe fallback
  function openCameraScanner() {
    if (!cameraScanModal) return;
    try {
      if (typeof cameraScanModal.showModal === 'function') {
        cameraScanModal.showModal();
      } else {
        cameraScanModal.setAttribute('open', '');
      }
      history.pushState({ modal: 'camera' }, '');
    } catch (e) {
      cameraScanModal.setAttribute('open', '');
    }
  }

  function closeCameraScanner() {
    if (!cameraScanModal) return;
    try {
      if (typeof cameraScanModal.close === 'function') {
        cameraScanModal.close();
      } else {
        cameraScanModal.removeAttribute('open');
      }
    } catch (e) {
      cameraScanModal.removeAttribute('open');
    }
  }

  if (cameraScanBtn) cameraScanBtn.addEventListener('click', openCameraScanner);
  if (profileCameraTrigger) profileCameraTrigger.addEventListener('click', openCameraScanner);

  if (closeCameraModalBtn) closeCameraModalBtn.addEventListener('click', closeCameraScanner);

  if (simulateScanSuccessBtn) {
    simulateScanSuccessBtn.addEventListener('click', () => {
      closeCameraScanner();
      selectActivePod(CERTIFIED_PODS[0]); // Triphala Kadha
      showToast('✓ Camera recognized: Triphala Kadha Pod');
      if (dockButtons && dockButtons[0]) dockButtons[0].click(); // Switch to Brew tab
    });
  }

  // Notification Bell Click (Clears red dot)
  if (notifBellBtn) {
    notifBellBtn.addEventListener('click', () => {
      if (notifRedDot && notifRedDot.classList.contains('visible')) {
        notifRedDot.classList.remove('visible');
        showToast('✓ Fresh Kadha ready · 100 mL dispensed at 56°C');
      } else {
        showToast('No new alerts · All systems normal');
      }
    });
  }

  // ==========================================
  // 7. NAVIGATION & SHOWCASE CONTROLS
  // ==========================================
  if (profileShowcaseTrigger) {
    profileShowcaseTrigger.addEventListener('click', () => {
      showScreen(websiteScreen);
      history.pushState({ screen: 'showcase' }, '');
    });
  }

  if (returnToAppBtn) {
    returnToAppBtn.addEventListener('click', () => {
      showScreen(mainAppScreen);
    });
  }

  // Android Native Hardware / Gesture Back Navigation Support
  window.addEventListener('popstate', (e) => {
    if (cameraScanModal && (cameraScanModal.open || cameraScanModal.hasAttribute('open'))) {
      closeCameraScanner();
    } else if (websiteScreen && websiteScreen.classList.contains('active')) {
      showScreen(mainAppScreen);
    }
  });

  // ==========================================
  // 8. BOTTOM NAVIGATION (4 TABS)
  // ==========================================
  dockButtons.forEach(btn => {
    btn.addEventListener('click', () => {
      dockButtons.forEach(b => b.classList.remove('active'));
      btn.classList.add('active');

      const targetTab = btn.dataset.tab;
      tabPages.forEach(p => {
        if (p.id === targetTab) {
          p.classList.add('active');
        } else {
          p.classList.remove('active');
        }
      });

      if (targetTab === 'tabBrew') {
        setTimeout(drawMinimalChart, 100);
      }
    });
  });

  // ==========================================
  // 9. IN-APP POD DROPDOWN ACCORDION
  // ==========================================
  function renderDropdownOptions() {
    if (!podDropdownList) return;
    podDropdownList.innerHTML = '';

    CERTIFIED_PODS.forEach(pod => {
      const option = document.createElement('div');
      const isSelected = pod.id === activePod.id;
      option.className = `dropdown-pod-option ${isSelected ? 'selected' : ''}`;
      option.innerHTML = `
        <div>
          <span class="option-name">${pod.name}</span>
          <span class="option-sub">${pod.ingredients}</span>
        </div>
        <span class="option-badge">${isSelected ? 'Active' : 'Choose'}</span>
      `;

      option.addEventListener('click', (e) => {
        e.stopPropagation();
        selectActivePod(pod);
        closeDropdown();
      });

      podDropdownList.appendChild(option);
    });
  }

  function toggleDropdown() {
    const isOpen = dropdownPodSection.classList.toggle('open');
    activePodTrigger.setAttribute('aria-expanded', isOpen);
  }

  function closeDropdown() {
    dropdownPodSection.classList.remove('open');
    activePodTrigger.setAttribute('aria-expanded', 'false');
  }

  activePodTrigger.addEventListener('click', toggleDropdown);

  function selectActivePod(pod) {
    activePod = pod;
    activePodName.textContent = pod.name;
    activePodIngredients.textContent = pod.ingredients;
    if (activePodThumb) activePodThumb.src = pod.thumb;
    renderDropdownOptions();
    showToast(`Pod set to ${pod.name}`);
  }

  renderDropdownOptions();

  document.addEventListener('click', (e) => {
    if (!dropdownPodSection.contains(e.target)) {
      closeDropdown();
    }
  });

  // ==========================================
  // 10. SMOOTH EXTRACTION KINETICS & COUNTDOWN
  // ==========================================
  function formatCountdown(remainingSeconds) {
    const totalSecs = Math.max(0, (TOTAL_CYCLE_SECONDS - remainingSeconds) * 10);
    const mins = Math.floor(totalSecs / 60);
    const secs = totalSecs % 60;
    return `${mins.toString().padStart(2, '0')}:${secs.toString().padStart(2, '0')} left`;
  }

  function updateUIState() {
    for (let i = 1; i <= 5; i++) {
      const node = document.getElementById(`stepNode${i}`);
      if (node) {
        node.className = (i <= brewState) ? 'min-step active' : 'min-step';
      }
    }

    let progressPct = 0;
    if (brewState > 0 && brewState < 6) {
      progressPct = Math.min(100, Math.round((brewSeconds / TOTAL_CYCLE_SECONDS) * 100));
    } else if (brewState === 6) {
      progressPct = 100;
    }
    stepperProgressBar.style.width = `${progressPct}%`;

    if (brewState === 0) {
      cycleStateText.textContent = 'Ready';
      cycleBtnLabel.textContent = 'Begin Cycle';
      countdownTimer.textContent = '12:00 left';
    } else if (brewState === 1) {
      cycleStateText.textContent = 'Soaking Herb';
      cycleBtnLabel.textContent = 'Pause';
      countdownTimer.textContent = formatCountdown(brewSeconds);
    } else if (brewState === 2) {
      cycleStateText.textContent = 'Gentle Boil (85°C)';
      cycleBtnLabel.textContent = 'Pause';
      countdownTimer.textContent = formatCountdown(brewSeconds);
    } else if (brewState === 3) {
      cycleStateText.textContent = 'Reducing 4:1';
      cycleBtnLabel.textContent = 'Pause';
      countdownTimer.textContent = formatCountdown(brewSeconds);
    } else if (brewState === 4) {
      cycleStateText.textContent = 'Mesh Filtration';
      cycleBtnLabel.textContent = 'Filtering...';
      countdownTimer.textContent = formatCountdown(brewSeconds);
    } else if (brewState === 5) {
      cycleStateText.textContent = 'Dispensing Cup';
      cycleBtnLabel.textContent = 'Dispensing...';
      countdownTimer.textContent = formatCountdown(brewSeconds);
    } else {
      cycleStateText.textContent = 'Complete';
      cycleBtnLabel.textContent = 'New Brew';
      countdownTimer.textContent = '00:00 left';
    }

    liveTempDisplay.textContent = sensorData.temp.toFixed(1);
    liveVolumeDisplay.textContent = Math.round(sensorData.volume);

    if (sensorData.temp >= 85.0 && sensorData.temp <= 90.0) {
      tempBandBadge.textContent = 'Locked in 85–90°C';
    } else {
      tempBandBadge.textContent = '85–90°C safe range';
    }

    const diagRtd = document.getElementById('diagRtd');
    const diagPress = document.getElementById('diagPress');
    const diagLoad = document.getElementById('diagLoad');
    const diagBrix = document.getElementById('diagBrix');
    if (diagRtd) diagRtd.textContent = `${sensorData.temp.toFixed(1)}°C`;
    if (diagPress) diagPress.textContent = `${(sensorData.pressure * 101.3).toFixed(1)} kPa`;
    if (diagLoad) diagLoad.textContent = `${Math.round(sensorData.volume)} g`;
    if (diagBrix) diagBrix.textContent = `${sensorData.brix.toFixed(1)}° Brix`;
  }

  function startBrewCycle() {
    if (brewState === 0 || brewState === 6) {
      brewState = 1;
      brewSeconds = 0;
      sensorData = { temp: 24.5, pressure: 1.00, volume: 400, brix: 0.8 };
      showToast(`Brewing ${activePod.name}`);
    } else {
      if (brewTimer) {
        clearInterval(brewTimer);
        brewTimer = null;
        cycleBtnLabel.textContent = 'Resume';
        showToast('Paused');
        return;
      }
    }

    if (brewTimer) clearInterval(brewTimer);
    brewTimer = setInterval(tickBrewCycle, 500);
    updateUIState();
  }

  function tickBrewCycle() {
    brewSeconds++;

    if (brewSeconds < 8) {
      brewState = 1;
      sensorData.temp = Math.min(38.0, sensorData.temp + 1.8);
      sensorData.pressure = 1.00;
      sensorData.volume = 400;
      sensorData.brix = 0.9;
    } else if (brewSeconds < 24) {
      brewState = 2;
      sensorData.pressure = Math.max(0.55, sensorData.pressure - 0.04);
      if (sensorData.temp < 88.4) {
        sensorData.temp += 3.2;
      } else {
        sensorData.temp = 88.4 + (Math.sin(brewSeconds) * 0.5);
      }
      sensorData.volume = Math.max(340, sensorData.volume - 3);
      sensorData.brix = 1.4;
    } else if (brewSeconds < 54) {
      brewState = 3;
      sensorData.pressure = 0.55;
      sensorData.temp = 88.2 + (Math.sin(brewSeconds * 1.5) * 0.6);
      const reduceProgress = (brewSeconds - 24) / 30;
      sensorData.volume = 340 - (reduceProgress * 240);
      sensorData.brix = 1.4 + (reduceProgress * 2.0);
    } else if (brewSeconds < 64) {
      brewState = 4;
      sensorData.temp = Math.max(70.0, sensorData.temp - 1.2);
      sensorData.volume = 100;
      sensorData.brix = 3.4;
    } else if (brewSeconds < 72) {
      brewState = 5;
      sensorData.temp = Math.max(56.0, sensorData.temp - 1.0);
      sensorData.volume = 100;
      sensorData.brix = 3.42;
    } else {
      // Completed!
      brewState = 6;
      clearInterval(brewTimer);
      brewTimer = null;
      showToast(`100 mL of fresh ${activePod.name} ready`);
      addHistory(activePod.name, '100 mL');

      // Light up the Red Dot on the Notification Bell!
      if (notifRedDot) {
        notifRedDot.classList.add('visible');
      }
    }

    chartHistory.shift();
    chartHistory.push(sensorData.temp);
    drawMinimalChart();

    updateUIState();
  }

  function abortBrew() {
    if (brewTimer) clearInterval(brewTimer);
    brewTimer = null;
    brewState = 0;
    brewSeconds = 0;
    sensorData = { temp: 24.5, pressure: 1.00, volume: 400, brix: 0.8 };
    showToast('Brew stopped');
    updateUIState();
    drawMinimalChart();
  }

  function rinseCycle() {
    if (brewTimer) clearInterval(brewTimer);
    showToast('Rinsing chamber...');
    let r = 0;
    const t = setInterval(() => {
      r++;
      if (r > 4) {
        clearInterval(t);
        showToast('Chamber clean');
        brewState = 0;
        updateUIState();
      }
    }, 400);
  }

  cycleActionBtn.addEventListener('click', startBrewCycle);
  heroStartCycleBtn.addEventListener('click', () => {
    dockButtons[0].click();
    startBrewCycle();
  });
  abortCycleBtn.addEventListener('click', abortBrew);
  rinseCycleBtn.addEventListener('click', rinseCycle);

  // ==========================================
  // 11. MINIMAL CANVAS TEMPERATURE CHART
  // ==========================================
  function drawMinimalChart() {
    if (!tempChartCanvas) return;
    const ctx = tempChartCanvas.getContext('2d');
    const w = tempChartCanvas.width;
    const h = tempChartCanvas.height;

    ctx.clearRect(0, 0, w, h);

    const y85 = h - 6 - (85 / 100) * (h - 14);
    const y90 = h - 6 - (90 / 100) * (h - 14);

    ctx.fillStyle = 'rgba(187, 192, 169, 0.22)';
    ctx.fillRect(0, y90, w, y85 - y90);

    ctx.strokeStyle = 'rgba(30, 43, 29, 0.15)';
    ctx.lineWidth = 1;
    ctx.setLineDash([3, 3]);
    ctx.beginPath();
    ctx.moveTo(0, y85);
    ctx.lineTo(w, y85);
    ctx.moveTo(0, y90);
    ctx.lineTo(w, y90);
    ctx.stroke();
    ctx.setLineDash([]);

    ctx.beginPath();
    const stepX = w / (chartHistory.length - 1);

    for (let i = 0; i < chartHistory.length; i++) {
      const val = chartHistory[i];
      const posX = i * stepX;
      const posY = h - 6 - (val / 100) * (h - 14);

      if (i === 0) {
        ctx.moveTo(posX, posY);
      } else {
        const prevX = (i - 1) * stepX;
        const prevY = h - 6 - (chartHistory[i - 1] / 100) * (h - 14);
        const cX = (prevX + posX) / 2;
        ctx.bezierCurveTo(cX, prevY, cX, posY, posX, posY);
      }
    }

    ctx.strokeStyle = '#1E2B1D';
    ctx.lineWidth = 1.8;
    ctx.stroke();

    ctx.lineTo(w, h);
    ctx.lineTo(0, h);
    ctx.closePath();
    const grad = ctx.createLinearGradient(0, 0, 0, h);
    grad.addColorStop(0, 'rgba(30, 43, 29, 0.08)');
    grad.addColorStop(1, 'rgba(30, 43, 29, 0.0)');
    ctx.fillStyle = grad;
    ctx.fill();
  }

  // ==========================================
  // 12. KADHAS CATALOGUE
  // ==========================================
  function renderPodsCatalogue() {
    if (!podsCatalogueList) return;
    podsCatalogueList.innerHTML = '';

    CERTIFIED_PODS.forEach(pod => {
      const card = document.createElement('article');
      card.className = 'pod-catalogue-card';
      card.innerHTML = `
        <div class="pod-card-hero">
          <div class="pod-card-thumb">
            <img src="${pod.thumb}" alt="${pod.name}">
          </div>
          <div class="pod-card-meta">
            <h3 class="pod-cat-title">${pod.name}</h3>
            <p class="pod-cat-indication">${pod.indication}</p>
          </div>
        </div>
        <div class="pod-card-bottom">
          <button class="btn-insert-pod" data-podid="${pod.id}">
            <span>Select Blend</span>
          </button>
        </div>
      `;

      card.querySelector('.btn-insert-pod').addEventListener('click', () => {
        selectActivePod(pod);
        dockButtons[0].click();
      });

      podsCatalogueList.appendChild(card);
    });
  }

  renderPodsCatalogue();

  // ==========================================
  // 13. JOURNAL
  // ==========================================
  const mockHistory = [
    { name: 'Triphala Kadha', volume: '100 mL', time: 'Today 07:15' },
    { name: 'Ayush Kadha', volume: '100 mL', time: 'Yesterday 20:30' }
  ];

  function renderHistory() {
    if (!historyCardsGroup) return;
    historyCardsGroup.innerHTML = '';
    mockHistory.forEach(item => {
      const card = document.createElement('div');
      card.className = 'history-item-card';
      card.innerHTML = `
        <strong class="hist-name">${item.name}</strong>
        <span class="hist-meta">${item.volume} · ${item.time}</span>
      `;
      historyCardsGroup.appendChild(card);
    });
  }

  function addHistory(name, volume) {
    mockHistory.unshift({ name, volume, time: 'Just now' });
    renderHistory();
  }

  renderHistory();

  // Toast utility
  let toastTimer = null;
  function showToast(msg) {
    toastMessage.textContent = msg;
    toastNotification.classList.add('visible');
    if (toastTimer) clearTimeout(toastTimer);
    toastTimer = setTimeout(() => {
      toastNotification.classList.remove('visible');
    }, 2500);
  }

});
