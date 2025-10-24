# 🌊 SISTEM DETECȚIE INUNDAȚII - STM32F429I-DISCO

---

## 📋 Descriere Scurtă

Sistem de monitorizare și alertare pentru detectarea inundațiilor, care măsoară:
- 🌡️ **Temperatură** (senzor DHT22)
- 💧 **Umiditate** (senzor DHT22)
- 🌊 **Nivel apă** (senzor inundație YL-83)

**Caracteristici:**
- ✅ Interfață grafică TouchGFX pe LCD 320x240
- ✅ Sistem de operare FreeRTOS (4 task-uri)
- ✅ Control automat releu + buzzer la detectare inundație
- ✅ Actualizare date în timp real (2s temp/umid, 500ms inundație)

---

## 🛠️ Hardware Necesar

- **STM32F429I-DISCO** (REV D sau E)
- **DHT22 (AM2302)** - Senzor temperatură/umiditate
- **YL-83** - Senzor detecție apă
- **Releu 5V** - Comandă echipamente
- **Buzzer** - Alertă sonoră
- Fire conexiuni Dupont

**Conexiuni:**
```
PG9  → DHT22 (Data)
PG12 → Water Sensor (Digital Out)
PG13 → Buzzer (+)
PG14 → Releu (IN)
```

---

## 💻 Software Necesar

- **STM32CubeIDE** 1.16+ (IDE principal - **recomandat!**)
- **TouchGFX Designer** 4.26+ (design UI)
- **STM32CubeProgrammer** 2.17+ (flash firmware)

**📥 Download:** [www.st.com/en/development-tools](https://www.st.com/en/development-tools)

---

## 🚀 Compilare și Programare

### Metodă 1: STM32CubeIDE (Recomandat)

```bash
1. File → Open Projects from File System
   → Selectați: ProiectInundatie/STM32CubeIDE

2. Build: Ctrl+B (sau iconiță 🔨)

3. Flash: Ctrl+F11 (sau iconiță ▶️)
```

### Metodă 2: TouchGFX Designer

```bash
1. Deschideți: ProiectInundatie.touchgfx
2. Generate Code
3. Flash to Target (necesită STM32CubeProgrammer)
```

### Metodă 3: Make (Terminal)

```powershell
cd STM32CubeIDE\Debug
make clean
make -j12 all
# Flash manual cu STM32CubeProgrammer
```

---

## 📖 Documentație Completă

Vezi **[DOCUMENTATIE_PROIECT.md](DOCUMENTATIE_PROIECT.md)** pentru:
- 📐 Arhitectură sistem detaliat
- 🔧 Explicații cod sursă
- 📊 Diagrame și scheme conexiuni
- 🐛 Probleme rezolvate (debugging)
- 📚 Bibliografie și resurse

---

## 📁 Structură Proiect

```
ProiectInundatie/
├── Core/
│   ├── Src/main.c              ← Codul principal (DHT22, tasks FreeRTOS)
│   └── Inc/FreeRTOSConfig.h    ← Configurare RTOS
├── TouchGFX/
│   ├── gui/src/                ← Logică interfață grafică
│   └── target/TouchGFXStubs.cpp ← Fix pentru bibliotecă
├── STM32CubeIDE/
│   └── Debug/STM32F429I_DISCO_REV_D01.elf
├── STM32F429I_DISCO_REV_D01.ioc ← Configurare STM32CubeMX
├── DOCUMENTATIE_PROIECT.md      ← **Documentație completă (40+ pagini)**
└── readme.md                    ← Acest fișier
```

---

## ⚙️ Configurare STM32CubeMX

- **MCU:** STM32F429ZIT6 @ 180 MHz
- **RTOS:** FreeRTOS v10.0.1 (heap_4, 256 KB SDRAM)
- **Display:** LTDC + DMA2D (320x240, 16bpp)
- **Periferice:** I2C3 (touch), SPI5 (LCD), GPIO (senzori)

**🚨 Important:** Dacă regenerați cod cu CubeMX:
- ✅ Păstrați `configUSE_NEWLIB_REENTRANT = 0` (FreeRTOSConfig.h)
- ✅ Nu ștergeți codul între `/* USER CODE BEGIN */` și `/* USER CODE END */`

---

## 🐛 Probleme Cunoscute

### ❌ Eroare: `'reent.h' file not found`

**Soluție:** În `Core/Inc/FreeRTOSConfig.h`, linia 97:
```c
#define configUSE_NEWLIB_REENTRANT  0  // Trebuie 0, nu 1!
```

### ❌ Keil: "Code size exceeds 32KB"

**Soluție:** Folosiți **STM32CubeIDE** (GCC, fără limitări)  
Proiectul are ~294 KB cod → Keil MDK Free (32 KB) nu funcționează!

---

## 📊 Performanță

| Metric | Valoare |
|--------|---------|
| **Cod (Flash)** | 293 KB |
| **RAM Intern** | 256 KB (stack + variabile) |
| **RAM Extern (SDRAM)** | 8 MB (framebuffers + heap) |
| **Framerate** | ~30 FPS (GUI TouchGFX) |
| **Timp citire DHT22** | ~25 ms |
| **Reacție inundație** | <500 ms |

---
## 🔗 Resurse Utile

- [STM32F429I-DISCO Documentation](https://www.st.com/en/evaluation-tools/32f429idiscovery.html)
- [FreeRTOS Official Docs](https://www.freertos.org/Documentation/RTOS_book.html)
- [TouchGFX Documentation](https://support.touchgfx.com/docs/)
- [DHT22 Datasheet](https://www.sparkfun.com/datasheets/Sensors/Temperature/DHT22.pdf)

---

**⭐ Proiect funcțional și testat pe STM32F429I-DISCO REV D**  
**📅 Ultima actualizare:** Octombrie 2025

Pentru debugging sau întrebări, consultați **DOCUMENTATIE_PROIECT.md**! 🚀

 