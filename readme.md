# 🕹️ Velxio Interactive Arduino Claw Machine

A mini arcade claw machine simulation built for **Velxio** (`velxio.dev`), featuring real microcontroller logic, object-oriented C++ architecture, and interactive push-button inputs.

## 🚀 Features
- **Simulated Hardware Control:** Built for an Arduino Uno board.
- **Interactive Input:** Real-time push-button integration configured with internal pull-up resistors (`INPUT_PULLUP`).
- **Object-Oriented Design:** Uses clean separation of concerns for mock motors (`MotorInterface`) and game mechanics (`ClawController`).
- **Serial Debugging:** Live console logs streaming game states directly to the Velxio Serial Monitor.

---

## 🛠️ Wiring Layout (Velxio Canvas)

To run the interactive controls, wire your push button as follows:

| Component Pin | Arduino Uno Pin | Description |
| :--- | :--- | :--- |
| **Button Left Leg** | **Digital Pin 2** | Signal line (Left/Move trigger) |
| **Button Right Leg** | **GND** | Ground reference |

---

## 📂 Code (`sketch.ino`)

The entire project is packaged into a clean, single-file Arduino sketch for easy loading and saving
💡 How to Run
Open Velxio.
Add an Arduino Uno board to the canvas.
Add a Push Button component and wire it to Pin 2 and GND.
Paste the code into sketch.ino, hit Play, and open the Serial Monitor to play!


    
}
