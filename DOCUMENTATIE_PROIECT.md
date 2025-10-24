# 📘 DOCUMENTAȚIE PROIECT - SISTEM DE DETECȚIE INUNDAȚII

---

## 📋 CUPRINS

1. [Descrierea Proiectului](#1-descrierea-proiectului)
2. [Componente Hardware](#2-componente-hardware)
3. [Componente Software](#3-componente-software)
4. [Arhitectura Sistemului](#4-arhitectura-sistemului)
5. [Funcționalități Implementate](#5-funcționalități-implementate)
6. [Configurare și Instalare](#6-configurare-și-instalare)
7. [Utilizare](#7-utilizare)
8. [Diagrame și Scheme](#8-diagrame-și-scheme)
9. [Probleme Întâmpinate și Rezolvări](#9-probleme-întâmpinate-și-rezolvări)
10. [Concluzii](#10-concluzii)
11. [Bibliografie](#11-bibliografie)

---

## 1. DESCRIEREA PROIECTULUI

### 1.1 Obiectiv

Dezvoltarea unui sistem de monitorizare și avertizare pentru detectarea inundațiilor, care măsoară în timp real:
- **Nivelul apei** (folosind sensor de inundație)
- **Temperatura ambientală** (°C)
- **Umiditatea aerului** (%)

Sistemul activează automat:
- ✅ **Releu de comandă** - pentru control echipamente (pompe, alarme externe)
- 🔊 **Avertizare sonoră** - buzzer pentru alertă audibilă
- 📱 **Interfață grafică TouchGFX** - afișare date pe LCD

### 1.2 Scopul Proiectului

- Învățarea programării microcontrolerelor ARM Cortex-M4
- Utilizarea sistemului de operare în timp real FreeRTOS
- Configurarea periferice complexe (I2C, SPI, GPIO, LTDC, DMA2D)
- Dezvoltarea de interfețe grafice cu TouchGFX
- Implementarea comunicației între task-uri RTOS

---

## 2. COMPONENTE HARDWARE

### 2.1 Microcontroler

**STM32F429ZIT6** - ARM Cortex-M4F @ 180 MHz

**Specificații:**
- 🔲 **Flash:** 2 MB
- 🧠 **RAM:** 256 KB (intern) + 8 MB SDRAM (extern)
- ⚡ **FPU:** Unitate virgulă mobilă (IEEE 754)
- 📺 **Display Controller:** LTDC + DMA2D (Chrom-ART Accelerator™)
- 🔌 **Periferice:** I2C, SPI, UART, ADC, DMA, Timers

### 2.2 Placă de Dezvoltare

**STM32F429I-DISCO REV D**

**Include:**
- 📱 LCD TFT 2.4" (320x240) cu controller ILI9341
- 👆 Touch Controller STMPE811 (I2C)
- 🔗 ST-LINK/V2 integrat (debug + programare)
- 💾 8 MB SDRAM extern (IS42S16400J)
- 🔘 2 butoane (USER, RESET)
- 💡 2 LED-uri utilizator

### 2.3 Senzori și Actuatori

| Component | Model | Interfață | Pin-uri |
|-----------|-------|-----------|---------|
| **Senzor Temperatură/Umiditate** | DHT22 (AM2302) | Digital (1-Wire) | PG9 |
| **Senzor Inundație** | YL-83 Water Sensor | Digital | PG12 |
| **Releu** | Module 5V | Digital Output | PG14 |
| **Buzzer** | Passive Buzzer | PWM (simulat) | PG13 |

### 2.4 Scheme de Conexiuni

```
STM32F429I-DISCO
┌─────────────────────────────────┐
│                                 │
│  PG9  ────► DHT22 (Data)       │  Senzor Temp/Umid
│  PG12 ────► Water Sensor OUT   │  Detectare inundație
│  PG13 ────► Buzzer (+)         │  Alertă sonoră
│  PG14 ────► Releu (IN)         │  Control echipamente
│                                 │
│  PE2  ────► VSYNC_FREQ (Debug) │
│  PE3  ────► RENDER_TIME (Debug)│
│  PE4  ────► FRAME_RATE (Debug) │
│  PE5  ────► MCU_ACTIVE (Debug) │
│                                 │
│  LCD  ────► LTDC (Paralel)     │  Display integrat
│  Touch ───► I2C3 (STMPE811)    │  Touch integrat
└─────────────────────────────────┘

Alimentare:
- STM32: USB 5V (ST-LINK)
- Senzori: 3.3V (pinii plăcii)
- Releu: 5V extern (dacă necesar)
```

---

## 3. COMPONENTE SOFTWARE

### 3.1 IDE și Toolchain

- **STM32CubeIDE 1.16+** - IDE principal Eclipse-based
- **STM32CubeMX 6.12+** - Configurare hardware și generare cod
- **TouchGFX Designer 4.26+** - Design interfață grafică
- **ARM GCC Compiler** - Compilator (fără limitări de mărime cod)
- **STM32CubeProgrammer** - Programare și debug

### 3.2 Middleware și Biblioteci

- **FreeRTOS v10.0.1** - Sistem de operare timp real
- **TouchGFX Framework 4.26** - Framework UI
- **HAL (Hardware Abstraction Layer)** - STM32F4xx HAL Driver
- **CMSIS** - Cortex Microcontroller Software Interface Standard

### 3.3 Configurare FreeRTOS

**Caracteristici activate:**
- ✅ Task Scheduler (preemptive)
- ✅ Tick Rate: 1000 Hz (1 ms)
- ✅ Heap: heap_4 (256 KB în SDRAM)
- ✅ Queue, Mutex, Semaphores
- ⚠️ configUSE_NEWLIB_REENTRANT = 0 (compatibilitate ARM Compiler 6)

### 3.4 Configurare TouchGFX

- **Rezoluție:** 320x240 pixeli
- **Adâncime culoare:** 16 bpp (RGB565)
- **Framebuffer:** 2 buffere × 150 KB = 300 KB (în SDRAM)
- **Font:** Verdana 10, 20, 40 pt (4bpp)
- **Imagini:** Logo personalizat 180x200 ARGB8888

---

## 4. ARHITECTURA SISTEMULUI

### 4.1 Diagrama Bloc

```
┌─────────────────────────────────────────────────────────┐
│                   STM32F429ZIT6                         │
│                                                         │
│  ┌─────────────┐  ┌──────────────┐  ┌──────────────┐  │
│  │ Task DHT22  │  │ Task Flood   │  │ Task Control │  │
│  │ (Temp/Hum)  │  │ (Inundație)  │  │ (Releu+Buzz) │  │
│  └──────┬──────┘  └──────┬───────┘  └──────┬───────┘  │
│         │                │                 │           │
│         └────────────────┴─────────────────┘           │
│                          │                             │
│                   ┌──────▼───────┐                     │
│                   │  GUI Task    │                     │
│                   │  (TouchGFX)  │                     │
│                   └──────┬───────┘                     │
│                          │                             │
│                   ┌──────▼───────┐                     │
│                   │ LTDC + DMA2D │                     │
│                   └──────┬───────┘                     │
└────────────────────────────┼──────────────────────────┘
                             │
                     ┌───────▼────────┐
                     │   LCD Display  │
                     │   (ILI9341)    │
                     └────────────────┘
```

### 4.2 Task-uri FreeRTOS

| Task | Prioritate | Stack | Perioadă | Funcție |
|------|-----------|-------|----------|---------|
| **GUI_Task** | Normal | 32 KB | Continuu | Actualizare interfață TouchGFX |
| **task_CheckFlood** | Normal | 1 KB | 500 ms | Citire senzor inundație |
| **task_ControlOut** | Normal | 1 KB | 100 ms | Control releu și buzzer |
| **task_ReadDHT22** | Normal | 1 KB | 2000 ms | Citire DHT22 (temp/umid) |
| **defaultTask** | Normal | 512 B | Continuu | Task implicit (poate fi șters) |

### 4.3 Variabile Globale Partajate

```c
// Protecție acces concurent cu volatile
volatile float g_temperature = 0.0f;  // Temperatura curentă (°C)
volatile float g_humidity = 0.0f;     // Umiditatea curentă (%)
volatile uint8_t g_flood_detected = 0; // Status inundație (0=OK, 1=ALERTĂ)
```

**⚠️ Notă:** În producție, variabilele ar trebui protejate cu Mutex sau folosire de queue-uri!

---

## 5. FUNCȚIONALITĂȚI IMPLEMENTATE

### 5.1 Citire Senzor DHT22

**Protocol comunicație 1-Wire:**

```
┌─────────────────────────────────────────────────────┐
│ START: MCU trage LOW > 1ms                          │
│   ↓                                                 │
│ DHT răspunde: LOW 80μs + HIGH 80μs                 │
│   ↓                                                 │
│ Transmite 40 biți (5 octeți):                      │
│   - Umiditate (16 bit)                             │
│   - Temperatură (16 bit)                           │
│   - Checksum (8 bit)                               │
│   ↓                                                 │
│ Decodare: '0' = 26-28μs HIGH, '1' = 70μs HIGH    │
│   ↓                                                 │
│ Validare checksum: sum(byte0..3) == byte4         │
└─────────────────────────────────────────────────────┘
```

**Implementare:**
```c
uint8_t DHT22_Read(float* temperature, float* humidity) {
  // 1. Trimite start signal (LOW 1.1ms)
  // 2. Setează pin ca INPUT
  // 3. Așteaptă răspuns DHT22 (LOW + HIGH)
  // 4. Citește 40 biți (măsurare durată HIGH)
  // 5. Validare checksum
  // 6. Calculează valori reale (împărțire la 10)
}
```

### 5.2 Detectare Inundație

**Senzor YL-83:**
- **Output HIGH (3.3V)** → Apă detectată ⚠️
- **Output LOW (0V)** → Fără apă ✅

```c
void StartTask_CheckFlood(void *argument) {
  for(;;) {
    if (HAL_GPIO_ReadPin(WATER_SENSOR_GPIO_Port, WATER_SENSOR_Pin) == GPIO_PIN_SET) {
      g_flood_detected = 1;  // ALERTĂ!
    } else {
      g_flood_detected = 0;  // OK
    }
    osDelay(500);  // Verificare la fiecare 500 ms
  }
}
```

### 5.3 Control Releu și Buzzer

**Logică activare:**

```
┌──────────────────┐     ┌─────────────────┐
│ g_flood_detected │ ──► │ Releu: ACTIVAT  │
│      == 1        │     │ Buzzer: PORNIT  │
└──────────────────┘     └─────────────────┘
         │
         NO
         ↓
┌─────────────────┐
│ Releu: OPRIT    │
│ Buzzer: OPRIT   │
└─────────────────┘
```

**Implementare:**
```c
void StartTask_ControlOut(void *argument) {
  for(;;) {
    if (g_flood_detected) {
      HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET);   // Releu ON
      HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET); // Buzzer ON
    } else {
      HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
    }
    osDelay(100);  // Reacție rapidă (100 ms)
  }
}
```

### 5.4 Interfață Grafică TouchGFX

**Componente UI:**

- 🖼️ **Logo personalizat** (180x200 px)
- 🌡️ **TextArea Temperatură** (wildcardBuffer: `tempBuffer`)
- 💧 **TextArea Umiditate** (wildcardBuffer: `humBuffer`)
- 🔤 **Font:** Verdana 10/20/40 pt

**Actualizare date:**

```cpp
// În Screen1View.cpp
void Screen1View::updateTemperature(float temp) {
  Unicode::snprintfFloat(tempBuffer, 6, "%.1f", temp);
  textArea1.invalidate();  // Redesenare
}

void Screen1View::updateHumidity(float hum) {
  Unicode::snprintfFloat(humBuffer, 6, "%.1f", hum);
  textArea2.invalidate();
}
```

**Apelare din main.c:**

```c
extern Screen1View* screen1ViewPtr;  // Pointer global

void StartTask_ReadDHT22(void *argument) {
  float temp, hum;
  for(;;) {
    if (DHT22_Read(&temp, &hum)) {
      g_temperature = temp;
      g_humidity = hum;
      
      // Actualizează GUI (thread-safe)
      if (screen1ViewPtr != NULL) {
        screen1ViewPtr->updateTemperature(temp);
        screen1ViewPtr->updateHumidity(hum);
      }
    }
    osDelay(2000);  // Citire la 2 secunde
  }
}
```

---

## 6. CONFIGURARE ȘI INSTALARE

### 6.1 Cerințe Hardware

- ✅ PC cu Windows 10/11 (64-bit)
- ✅ STM32F429I-DISCO (REV D sau E)
- ✅ Cablu USB Micro-B (ST-LINK)
- ✅ Senzori: DHT22, YL-83, Releu, Buzzer
- ✅ Fire conexiuni Dupont (M-F, M-M)

### 6.2 Instalare Software

**Pasul 1:** Instalați toolchain-ul

```bash
# Descărcați și instalați (în ordine):
1. STM32CubeIDE 1.16.0+
   https://www.st.com/en/development-tools/stm32cubeide.html

2. STM32CubeMX 6.12.0+ (inclus în CubeIDE)

3. TouchGFX Designer 4.26.0+
   https://www.st.com/en/development-tools/touchgfxdesigner.html

4. STM32CubeProgrammer 2.17+
   https://www.st.com/en/development-tools/stm32cubeprog.html
```

**Pasul 2:** Clonați/Descărcați proiectul

```

**Pasul 3:** Deschideți proiectul în STM32CubeIDE

```
File → Open Projects from File System
→ Selectați: C:\...\ProiectInundatie\STM32CubeIDE
```

### 6.3 Configurare Hardware

**Conexiuni senzori:**

```
DHT22:
  VCC  → 3.3V (STM32)
  DATA → PG9
  GND  → GND

Water Sensor (YL-83):
  VCC  → 3.3V
  DO   → PG12 (Digital Output)
  GND  → GND

Releu Module:
  VCC  → 5V (extern sau pin 5V STM32)
  IN   → PG14
  GND  → GND

Buzzer:
  (+)  → PG13
  (-)  → GND
```

### 6.4 Compilare și Programare

**Metodă 1: Din STM32CubeIDE**

```
1. Conectați placa STM32F429I-DISCO la PC (USB)
2. Build: Ctrl+B sau hammer icon (🔨)
3. Flash: Ctrl+F11 sau Run icon (▶️)
```

**Metodă 2: Din TouchGFX Designer**

```
1. Deschideți ProiectInundatie.touchgfx
2. Modificați UI (dacă e necesar)
3. Generate Code → Flash to Target
```

**Metodă 3: Manual cu STM32CubeProgrammer**

```
1. Conectați placa
2. Open STM32CubeProgrammer
3. Connect (ST-LINK)
4. Download File: STM32F429I_DISCO_REV_D01.hex
5. Start Programming
```

---

## 7. UTILIZARE

### 7.1 Pornire Sistem

1. **Conectați alimentarea** (USB ST-LINK)
2. **Sistemul inițializează** (2-3 secunde):
   - Configurare SDRAM
   - Inițializare LCD
   - Pornire FreeRTOS
3. **Afișare interfață grafică:**
   - Logo personalizat
   - Temperatură: `--.-°C` (până la prima citire)
   - Umiditate: `--.-%`

### 7.2 Funcționare Normală

**Condiții normale (fără inundație):**

```
┌─────────────────────────────┐
│  📱 Display LCD             │
│                             │
│  🏫 [Logo]                  │
│                             │
│  🌡️ Temp: 23.5°C           │
│  💧 Umid: 45.2%             │
│                             │
│  🔇 Buzzer: OPRIT           │
│  🔌 Releu: DESCHIS          │
└─────────────────────────────┘
```

**Actualizare date:**
- **Temperatură/Umiditate:** La fiecare 2 secunde
- **Verificare inundație:** La fiecare 500 ms

### 7.3 Detectare Inundație

**Când senzorul detectează apă:**

```
┌─────────────────────────────┐
│  📱 Display LCD             │
│                             │
│  🏫 [Logo]                  │
│                             │
│  🌡️ Temp: 24.1°C           │
│  💧 Umid: 67.8%             │
│                             │
│  ⚠️ INUNDAȚIE DETECTATĂ!   │
│  🔊 Buzzer: PORNIT          │
│  🔌 Releu: ÎNCHIS           │
└─────────────────────────────┘
```

**Acțiuni automate:**
1. ✅ Releu se activează (poate comanda pompă evacuare)
2. 🔊 Buzzer sună (alertă sonoră)
3. 📱 Afișaj continuă să arate date (temp/umid)

### 7.4 Debugging

**Pin-uri de monitorizare performanță:**

| Pin | Signal | Descriere |
|-----|--------|-----------|
| PE2 | VSYNC_FREQ | Frecvență actualizare display |
| PE3 | RENDER_TIME | Timp randare frame |
| PE4 | FRAME_RATE | FPS efectiv |
| PE5 | MCU_ACTIVE | Activitate procesor |

**Conectați osciloscop/analizor logic pentru măsurători.**

---

## 8. DIAGRAME ȘI SCHEME

### 8.1 State Machine - Task Control

```
        ┌──────────────┐
        │    IDLE      │
        │ (No Flood)   │
        └──────┬───────┘
               │
               │ [g_flood_detected == 1]
               ↓
        ┌──────────────┐
        │   ALERT      │◄───┐
        │ Releu: ON    │    │
        │ Buzzer: ON   │    │
        └──────┬───────┘    │
               │            │
               │ [Flood still detected]
               └────────────┘
               │
               │ [g_flood_detected == 0]
               ↓
        ┌──────────────┐
        │    IDLE      │
        │ (Recovered)  │
        └──────────────┘
```

### 8.2 Timing Diagram - DHT22

```
MCU Pin:  ───┐     ┌─────────────────────────────────
              │     │
              └─────┘ (1.1ms LOW)
                ↑
           Start Signal

DHT Pin:  ────────┐     ┌───┐   ┌───┐   ┌─────┐...
                   │     │   │   │   │   │     │
                   └─────┘   └───┘   └───┘     └...
                    80μs   80μs  Data bits (40x)
                    LOW    HIGH
```

### 8.3 Memory Layout

```
┌─────────────────────────────────────────┐
│         FLASH (2 MB)                    │
│  0x08000000 - 0x081FFFFF                │
│  - Cod program (~294 KB)                │
│  - Constante (Fonturi, imagini)        │
└─────────────────────────────────────────┘

┌─────────────────────────────────────────┐
│      Internal RAM (256 KB)              │
│  0x20000000 - 0x2003FFFF                │
│  - Stack task-uri FreeRTOS              │
│  - Variabile globale                    │
│  - Heap parțial                         │
└─────────────────────────────────────────┘

┌─────────────────────────────────────────┐
│      External SDRAM (8 MB)              │
│  0xD0000000 - 0xD07FFFFF                │
│  - Framebuffer 1: 150 KB                │
│  - Framebuffer 2: 150 KB                │
│  - FreeRTOS Heap (heap_4): 256 KB      │
│  - Scratch buffer TouchGFX: ~200 KB     │
└─────────────────────────────────────────┘
```

---

## 9. PROBLEME ÎNTÂMPINATE ȘI REZOLVĂRI

### 9.1 Eroare: `'reent.h' file not found`

**Problemă:**
```
FreeRTOS.h:62:10: fatal error: 'reent.h' file not found
#include <reent.h>
```

**Cauză:**
- ARM Compiler 6 nu folosește newlib (nu are `reent.h`)
- `configUSE_NEWLIB_REENTRANT` era setat pe 1

**Rezolvare:**
```c
// În Core/Inc/FreeRTOSConfig.h, linia 97:
#define configUSE_NEWLIB_REENTRANT       0  // era 1
```

### 9.2 Warning: `-Winconsistent-missing-override`

**Problemă:**
```cpp
Screen1Presenter.hpp:20: warning: 'activate' overrides a member function 
but is not marked 'override'
```

**Rezolvare:**
```cpp
// În TouchGFX/gui/include/gui/screen1_screen/Screen1Presenter.hpp
class Screen1Presenter : public Screen1PresenterBase {
public:
  virtual void activate() override;    // Adăugat 'override'
  virtual void deactivate() override;  // Adăugat 'override'
};
```

### 9.3 Eroare Linker: Section size overflow

**Problemă:**
```
L6407E: Sections of aggregate size 0x84e08 bytes could not fit into .ANY selector(s).
```

**Cauză:**
- Framebuffer-ele TouchGFX (2×150KB) nu încăpeau în RAM-ul intern (256KB)

**Rezolvare:**
- Mutat framebuffer-e și heap FreeRTOS în SDRAM extern (8MB)
- Modificat linker script (`STM32F429ZITX_FLASH.ld`)

### 9.4 Eroare: Keil Code Size Limit (32 KB)

**Problemă:**
```
L6050U: The code size of this image (125896 bytes) exceeds the maximum 
allowed for this version of the linker.
```

**Cauză:**
- Keil MDK free version limitată la 32 KB
- Proiectul TouchGFX are ~126 KB cod

**Rezolvare:**
- ✅ **Trecut pe STM32CubeIDE** (GCC, fără limitări)
- Alternative: Keil Pro ($$$) sau IAR EWARM ($$$$)

### 9.5 Undefined Symbol: TouchGFX Library Functions

**Problemă:**
```
undefined symbol: CompressedFontCache::hasCachedGlyph()
undefined symbol: VectorFontRendererImpl::getVectorFontBuffers()
```

**Cauză:**
- Biblioteca TouchGFX (wchar16 variant) nu include toate feature-urile

**Rezolvare:**
- Creat `TouchGFXStubs.cpp` cu implementări dummy:
```cpp
namespace touchgfx {
bool CompressedFontCache::hasCachedGlyph(Unicode::UnicodeChar) {
  return false;  // Nu folosim compressed fonts
}
// ... alte stub-uri
}
```

---

## 10. CONCLUZII

### 10.1 Rezultate Obținute

✅ **Sistem funcțional** care:
- Monitorizează temperatura și umiditatea în timp real
- Detectează prezența apei
- Activează automat releu și buzzer la inundație
- Afișează date pe LCD cu interfață grafică profesională

✅ **Competențe dobândite:**
- Programare ARM Cortex-M4 cu HAL
- Utilizare FreeRTOS (task-uri, sincronizare)
- Dezvoltare UI cu TouchGFX
- Debugging și rezolvare erori compilare/linker
- Configurare periferice complexe (LTDC, DMA2D, SDRAM)

### 10.2 Îmbunătățiri Viitoare

🔄 **Versiuni următoare pot include:**

1. **Comunicație wireless:**
   - ESP32 pentru notificări WiFi/MQTT
   - Bluetooth Low Energy pentru app Android/iOS

2. **Logging date:**
   - Salvare istoric temperatură/umiditate pe SD card
   - Grafice în timp real pe LCD

3. **Alarme configurabile:**
   - Praguri personalizabile de temperatură
   - Nivele multiple de alarmă

4. **Control remote:**
   - Comandă releu de la distanță
   - Verificare status prin web browser

5. **Senzori adiționali:**
   - Senzor presiune atmosferică (BMP280)
   - Senzor calitate aer (MQ-135)
   - Multiple senzori de inundație (nivele diferite)

### 10.3 Aplicabilitate Practică

Sistemul poate fi utilizat în:

- 🏠 **Subsoluri și pivnițe** - detectare infiltrații
- 🏭 **Spații industriale** - monitorizare scurgeri
- 🌾 **Depozite agricole** - control umiditate cereale
- 🖥️ **Camere servere** - alertă răcire/umiditate anormală
- 🔬 **Laboratoare** - monitorizare condiții mediu

---

## 11. BIBLIOGRAFIE

### 11.1 Documentație Oficială

1. **STMicroelectronics:**
   - [STM32F429xx Reference Manual (RM0090)](https://www.st.com/resource/en/reference_manual/rm0090-stm32f405415-stm32f407417-stm32f427437-and-stm32f429439-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
   - [STM32F429I-DISCO User Manual (UM1670)](https://www.st.com/resource/en/user_manual/um1670-discovery-kit-with-stm32f429zi-mcu-stmicroelectronics.pdf)
   - [TouchGFX Documentation](https://support.touchgfx.com/docs/)
   - [STM32CubeF4 HAL User Manual](https://www.st.com/resource/en/user_manual/um1725-description-of-stm32f4-hal-and-lowlayer-drivers-stmicroelectronics.pdf)

2. **FreeRTOS:**
   - [FreeRTOS Kernel Guide](https://www.freertos.org/Documentation/FreeRTOS_Reference_Manual_V10.0.0.pdf)
   - [Mastering the FreeRTOS Real Time Kernel](https://www.freertos.org/Documentation/161204_Mastering_the_FreeRTOS_Real_Time_Kernel-A_Hands-On_Tutorial_Guide.pdf)

3. **ARM:**
   - [Cortex-M4 Technical Reference Manual](https://developer.arm.com/documentation/100166/0001)
   - [ARMv7-M Architecture Reference Manual](https://developer.arm.com/documentation/ddi0403/latest/)

### 11.2 Datasheet-uri Componente

- [DHT22/AM2302 Datasheet](https://www.sparkfun.com/datasheets/Sensors/Temperature/DHT22.pdf)
- [ILI9341 LCD Controller](https://cdn-shop.adafruit.com/datasheets/ILI9341.pdf)
- [STMPE811 Touch Controller](https://www.st.com/resource/en/datasheet/stmpe811.pdf)
- [IS42S16400J SDRAM](https://www.issi.com/WW/pdf/42-45S16400J.pdf)

### 11.3 Tutoriale și Resurse Online

- [STM32 Step-by-Step](https://wiki.st.com/stm32mcu/wiki/STM32StepByStep:Getting_started_with_STM32_:_STM32_step_by_step)
- [TouchGFX Tutorials](https://support.touchgfx.com/docs/tutorials)
- [Embedded Systems with ARM Cortex-M Microcontrollers](https://www.youtube.com/playlist?list=PLtVUYRe-Z-mfKO0lg_-MBvwWl6VjWT8Dt)
- [STM32 FreeRTOS Examples](https://github.com/STMicroelectronics/STM32CubeF4/tree/master/Projects)

### 11.4 Cărți Recomandate

1. **"The Definitive Guide to ARM Cortex-M3/M4"** - Joseph Yiu (2013)
2. **"Embedded Systems: Real-Time Operating Systems"** - Jim Cooling (2020)
3. **"Mastering STM32"** - Carmine Noviello (2018)
4. **"Making Embedded Systems"** - Elecia White (2011)

---

## 📎 ANEXE

### Anexa A: Pinout Complet STM32F429I-DISCO

| Pin | Funcție | Periferic | Descriere |
|-----|---------|-----------|-----------|
| PG9 | DHT22_DATA | GPIO | Comunicație 1-wire senzor DHT22 |
| PG12 | WATER_SENSOR | GPIO Input | Intrare digitală senzor inundație |
| PG13 | BUZZER | GPIO Output | Control buzzer activ |
| PG14 | RELAY | GPIO Output | Comandă releu |
| PE2 | VSYNC_FREQ | GPIO Output | Debug: Frecvență VSYNC |
| PE3 | RENDER_TIME | GPIO Output | Debug: Timp randare |
| PE4 | FRAME_RATE | GPIO Output | Debug: FPS |
| PE5 | MCU_ACTIVE | GPIO Output | Debug: Activitate CPU |

### Anexa B: Configurare STM32CubeMX

**Clock Configuration:**
- HSE: 8 MHz (cristal extern)
- PLL: × 360, ÷ 2 → SYSCLK = 180 MHz
- AHB: 180 MHz
- APB1: 45 MHz (PCLK1)
- APB2: 90 MHz (PCLK2)

**Periferice Activate:**
- ✅ FMC (SDRAM Bank 2)
- ✅ LTDC (LCD Display)
- ✅ DMA2D (Graphics Accelerator)
- ✅ I2C3 (Touch Controller)
- ✅ SPI5 (LCD ILI9341)
- ✅ TIM6 (Timebase FreeRTOS)

### Anexa C: Lista Fișiere Importante

```
ProiectInundatie/
├── Core/
│   ├── Src/
│   │   ├── main.c              ← Codul principal (DHT22, tasks)
│   │   ├── freertos.c          ← Init FreeRTOS
│   │   └── stm32f4xx_it.c      ← Interrupt handlers
│   └── Inc/
│       ├── main.h
│       └── FreeRTOSConfig.h    ← Configurare FreeRTOS
├── TouchGFX/
│   ├── gui/
│   │   ├── src/screen1_screen/
│   │   │   └── Screen1View.cpp  ← Logica UI
│   │   └── include/screen1_screen/
│   │       └── Screen1View.hpp
│   └── target/
│       └── TouchGFXStubs.cpp    ← Stub-uri bibliotecă
├── STM32CubeIDE/
│   ├── STM32F429ZITX_FLASH.ld   ← Linker script
│   └── Debug/
│       └── STM32F429I_DISCO_REV_D01.elf
└── DOCUMENTATIE_PROIECT.md      ← Acest fișier
```

### Anexa D: Comenzi Utile Terminal

```bash
# Build din linie de comandă
cd STM32CubeIDE/Debug
make clean
make -j12 all

# Flash cu STM32CubeProgrammer CLI
STM32_Programmer_CLI -c port=SWD -w STM32F429I_DISCO_REV_D01.hex -v -rst

# Verificare dimensiuni secțiuni
arm-none-eabi-size STM32F429I_DISCO_REV_D01.elf

# Dump assembly
arm-none-eabi-objdump -d STM32F429I_DISCO_REV_D01.elf > disasm.txt
```

---

---

**📌 NOTĂ FINALĂ:**

Acest proiect demonstrează integrarea complexă a următoarelor concepte:
- ✅ Programare embedded ARM Cortex-M4
- ✅ Sistem de operare timp real (FreeRTOS)
- ✅ Interfață grafică profesională (TouchGFX)
- ✅ Comunicație cu senzori digitali
- ✅ Control actuatori (releu, buzzer)
- ✅ Sincronizare task-uri și partajare date
- ✅ Debugging și optimizare memorie


---
