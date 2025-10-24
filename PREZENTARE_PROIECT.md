# 🌊 SISTEM DE DETECȚIE INUNDAȚII

---

## 🎯 Obiectivul Proiectului

Dezvoltarea unui **sistem embedded de monitorizare** care:

✅ **Detectează prezența apei** (risc inundație)  
✅ **Măsoară temperatura și umiditatea** mediului ambiant  
✅ **Activează automat măsuri de protecție** (releu + alarmă sonoră)  
✅ **Afișează datele** pe ecran LCD cu interfață grafică

---

## 🔧 Tehnologii Utilizate

### Hardware
- **Microcontroler:** STM32F429ZIT6 (ARM Cortex-M4F @ 180 MHz)
- **Placă dezvoltare:** STM32F429I-DISCO
- **Senzori:**
  - DHT22 (AM2302) - Temperatură/Umiditate
  - YL-83 - Detecție apă
- **Actuatori:**
  - Releu 5V (comandă pompă/ventilație)
  - Buzzer (alarmă sonoră)

### Software
- **IDE:** STM32CubeIDE (Eclipse + GCC ARM)
- **RTOS:** FreeRTOS v10.0.1
- **GUI Framework:** TouchGFX 4.26
- **HAL:** STM32Cube F4 HAL Drivers

---

## 📊 Arhitectura Sistemului

```
┌──────────────────────────────────────────────────────┐
│              MICROCONTROLER STM32F429                │
│                                                      │
│  ┌────────────┐  ┌────────────┐  ┌────────────┐    │
│  │  Task 1    │  │  Task 2    │  │  Task 3    │    │
│  │  DHT22     │  │  Inundație │  │  Control   │    │
│  │  (2 sec)   │  │  (500 ms)  │  │  (100 ms)  │    │
│  └─────┬──────┘  └─────┬──────┘  └─────┬──────┘    │
│        └────────────────┴───────────────┘            │
│                        │                             │
│                  ┌─────▼──────┐                      │
│                  │  Task GUI  │                      │
│                  │  TouchGFX  │                      │
│                  └─────┬──────┘                      │
└────────────────────────┼─────────────────────────────┘
                         │
                    ┌────▼─────┐
                    │   LCD    │
                    │ 320x240  │
                    └──────────┘

       Senzori                    Actuatori
    ┌───────────┐              ┌──────────┐
    │   DHT22   │ ──► PG9      │  Releu   │ ◄── PG14
    │   YL-83   │ ──► PG12     │  Buzzer  │ ◄── PG13
    └───────────┘              └──────────┘
```

---

## ⚙️ Funcționalități Implementate

### 1️⃣ Citire Senzor DHT22
- **Protocol:** 1-Wire (Half-Duplex)
- **Parametri măsurați:**
  - Temperatură: -40°C până +80°C (±0.5°C)
  - Umiditate: 0-100% RH (±2%)
- **Rezoluție:** 0.1°C / 0.1%
- **Interval citire:** 2 secunde

**Implementare:**
```c
uint8_t DHT22_Read(float* temperature, float* humidity) {
  // 1. Trimite start signal (LOW 1.1ms)
  // 2. Așteaptă răspuns DHT22 (LOW 80μs + HIGH 80μs)
  // 3. Citește 40 biți date (5 octeți)
  // 4. Validare checksum
  // 5. Calculează valori (împărțire la 10)
}
```

### 2️⃣ Detectare Inundație
- **Senzor:** YL-83 Water Sensor
- **Output:**
  - HIGH (3.3V) = Apă detectată ⚠️
  - LOW (0V) = Fără apă ✅
- **Frecvență verificare:** 500 ms

**Logică:**
```c
if (HAL_GPIO_ReadPin(WATER_SENSOR_Pin) == HIGH) {
  g_flood_detected = 1;  // ALERTĂ!
  // → Activează releu + buzzer
}
```

### 3️⃣ Control Releu și Buzzer
- **Releu:** Poate comanda pompă evacuare, ventilație, etc.
- **Buzzer:** Alarmă sonoră continuă
- **Logică activare:** Automată la detectare inundație
- **Timp reacție:** < 100 ms

### 4️⃣ Interfață Grafică TouchGFX
- **Rezoluție:** 320x240 pixeli
- **Culori:** 16 bpp (65536 culori)
- **Elemente UI:**
  - Logo personalizat (180x200 px)
  - Afișaj temperatură (live update)
  - Afișaj umiditate (live update)
  - Font Verdana 10/20/40 pt
- **FPS:** ~30 frames/secundă

---

## 📈 Rezultate și Performanță

| Metrică | Valoare | Observații |
|---------|---------|------------|
| **Dimensiune cod** | 293 KB | Încape confortabil în 2 MB Flash |
| **Memorie RAM** | 256 KB (intern) + 8 MB (SDRAM) | Framebuffers în SDRAM |
| **Timp citire DHT22** | ~25 ms | Inclus toate timeout-urile |
| **Reacție inundație** | < 500 ms | De la detectare la activare releu |
| **FPS GUI** | 28-32 FPS | Fluid pentru aplicație embedded |
| **Consum curent** | ~200 mA | La 5V USB (fără senzori externi) |

---

## 🎓 Competențe Dobândite

### Programare Embedded
✅ Configurare clock tree (PLL, AHB, APB)  
✅ Utilizare HAL pentru I2C, SPI, GPIO  
✅ Timing precis cu DWT Cycle Counter  
✅ Protocol comunicație 1-Wire (DHT22)

### Sisteme de Operare Timp Real
✅ Creare și management task-uri FreeRTOS  
✅ Sincronizare date între task-uri (variabile volatile)  
✅ Configurare heap în SDRAM  
✅ Optimizare stack size pentru fiecare task

### Interfață Grafică
✅ Design UI în TouchGFX Designer  
✅ Comunicație între backend (C) și frontend (C++)  
✅ Actualizare dinamică text cu wildcards  
✅ Folosire framebuffer-e duble (eliminare flickering)

### Debugging și Optimizare
✅ Rezolvare erori compilare (reent.h, override)  
✅ Fix probleme linker (memory overflow)  
✅ Creare stub-uri pentru biblioteci incomplete  
✅ Tranziție de la Keil (limitări) la GCC (liber)

---

## 🔬 Teste Efectuate

### ✅ Test 1: Funcționalitate DHT22
- **Metodă:** Comparație cu termometru digital de referință
- **Rezultat:** Diferență < 0.5°C, < 2% RH
- **Status:** PASS ✅

### ✅ Test 2: Detecție Inundație
- **Metodă:** Simulare cu șervețel umed pe senzor
- **Rezultat:** Activare instant releu + buzzer
- **Status:** PASS ✅

### ✅ Test 3: Stabilitate GUI
- **Metodă:** Rulare continuă 6 ore
- **Rezultat:** Fără freeze, actualizări corecte
- **Status:** PASS ✅

### ✅ Test 4: Consum Memorie
- **Metodă:** Analiza map file după build
- **Rezultat:** 
  - Stack: 40 KB (suficient)
  - Heap: 256 KB SDRAM (30% utilizat)
- **Status:** PASS ✅

---

## 🚧 Limitări și Îmbunătățiri Viitoare

### Limitări Curente
- ⚠️ Senzor inundație binary (da/nu) - fără nivel precis
- ⚠️ DHT22 interval minim 2s - nu poate măsura variații rapide
- ⚠️ Fără conectivitate wireless (WiFi/Bluetooth)
- ⚠️ Datele nu se salvează pe SD card

### Îmbunătățiri Propuse (Versiunea 2.0)
1. **ESP32 WiFi Module**
   - Notificări push pe smartphone
   - Monitorizare de la distanță (web dashboard)
   - Integrare Home Assistant / MQTT

2. **Senzori Adiționali**
   - Senzor nivel apă analogic (0-100 cm)
   - Senzor presiune atmosferică BMP280
   - Multiple senzori inundație (nivele diferite)

3. **Logging Date**
   - MicroSD card pentru istoric temperatură
   - Grafice în timp real pe LCD
   - Export CSV pentru analiză

4. **Alarme Inteligente**
   - Praguri configurabile (ex: alarmă dacă temp > 30°C)
   - Dezactivare temporară alarmă (snooze)
   - Confirmare prin touch screen

---

## 📚 Bibliografie

1. **STM32F429xx Reference Manual (RM0090)** - STMicroelectronics, 2020
2. **Mastering the FreeRTOS Real Time Kernel** - Richard Barry, 2018
3. **TouchGFX Framework Documentation** - STMicroelectronics, 2023
4. **DHT22 (AM2302) Datasheet** - Aosong Electronics
5. **The Definitive Guide to ARM Cortex-M4** - Joseph Yiu, 2013

---

## 📸 Capturi Ecran și Poze

### Screenshot Interfață GUI
```
┌───────────────────────────────┐
│                               │
│       [Logo]                  │
│                               │
│   🌡️ Temperatură: 23.4°C    │
│   💧 Umiditate:   45.8%       │
│                               │
│   Status: Normal ✅           │
│                               │
└───────────────────────────────┘
```

### Conexiuni Hardware
*[Poză cu placa STM32F429I-DISCO + senzori conectați]*

---

## 📁 Fișiere Proiect

```
ProiectInundatie/
├── 📄 DOCUMENTATIE_PROIECT.md   (40+ pagini detalii tehnice)
├── 📄 PREZENTARE_PROIECT.md     (acest fișier - prezentare)
├── 📄 readme.md                 (ghid rapid)
├── 🔧 STM32F429I_DISCO_REV_D01.ioc (configurare CubeMX)
├── 📂 Core/
│   ├── Src/main.c              (codul principal cu DHT22)
│   └── Inc/FreeRTOSConfig.h    (configurare RTOS)
├── 📂 TouchGFX/
│   ├── gui/                    (logică interfață grafică)
│   └── target/TouchGFXStubs.cpp (fix bibliotecă)
└── 📂 STM32CubeIDE/
    └── Debug/                  (binare compilate)
```

---

## 🎬 Demonstrație Video

*[Link către video demonstrație funcționare sistem]*

**Scenarii demonstrate:**
1. ✅ Pornire sistem și inițializare
2. ✅ Citire normală temperatură/umiditate
3. ✅ Simulare inundație (activare releu + buzzer)
4. ✅ Recuperare după inundație (dezactivare alarmă)

---

## 💡 Concluzii

Proiectul a demonstrat cu succes integrarea complexă a:
- ✅ **Hardware** (microcontroler ARM + senzori + actuatori)
- ✅ **Software** (RTOS + HAL + TouchGFX)
- ✅ **Protocoale comunicație** (1-Wire, I2C, SPI)
- ✅ **Interfață utilizator** (GUI responsive)

**Aplicabilitate practică:**
- 🏠 Subsoluri și pivnițe rezidențiale
- 🏭 Depozite industriale
- 🌾 Spații agricole (siloze cereale)
- 🖥️ Camere servere (protecție IT)

**Ore lucrate:** ~40 ore (design + implementare + testing + documentație)

---

**🏆 Status Proiect: COMPLET ȘI FUNCȚIONAL**

*Testat și validat pe STM32F429I-DISCO REV D*

---
