# OscillatorDetector

**Lightweight, header-only oscillator detector for 1D signals**

`OscillatorDetector` is a small, lightweight C++ **header-only** class designed to detect oscillatory behavior in a 1D signal by tracking local maxima and minima. It uses debouncing and configurable thresholds to avoid false detections and is suitable for real-time or high-frequency loops.

---

## Features

- Header-only: no compilation or linking needed, just include the `.hpp`.
- Detects local maxima and minima in a signal.
- Debounce logic to ignore spurious fluctuations.
- Configurable **smoothing threshold** and **sensitivity**.
- Lightweight and suitable for real-time usage.
- Fully encapsulated internal state for a clean API.

---

## License

This project is released under the **BSD 3-Clause License**.  
See the license header in `OscillatorDetector.hpp` for details.

---

## Usage

```cpp
#include "OscillatorDetector.hpp"

int main() {
    OscillatorDetector detector;

    // Optional: configure thresholds
    detector.setSmootherThreshold(5);
    detector.setSensitivity(5);

    int64_t signal = 0;
    int direction = 1; // 1 = rising, 0 = stopped, -1 = falling

    // In your loop or signal processing:
    if (detector.detect(signal, direction)) {
        // Oscillation detected
    }

    return 0;
}
```

---

## API

### `bool detect(int64_t position, int direction)`

Analyzes the current signal value and direction to detect extrema.

**Parameters:**

- `position` – the current value of the signal.  
- `direction` – direction of change:  
  - `-1` = falling  
  - `0` = stopped  
  - `1` = rising  

**Returns:** `true` if the number of detected extrema exceeds the sensitivity threshold.

---

### Setters

- `void setSmootherThreshold(uint8_t threshold)` – sets the number of consecutive updates required to confirm an extremum.  
- `void setSensitivity(uint8_t sensitivity)` – sets the number of extrema required to trigger detection.  

---

### Getters

- `uint8_t getSmootherThreshold() const` – returns the current smoothing threshold.  
- `uint8_t getSensitivity() const` – returns the current sensitivity threshold.  

