/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2025, Szymon Bandel
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <limits>
#include <cstdint>


/**
 * @brief Lightweight oscillator detector for a 1D signal.
 *
 * The OscillatorDetector class tracks local maxima and minima of a signal
 * based on the direction of its change. It detects extrema using debouncing
 * and thresholding to avoid false oscillations. Detection parameters, such as
 * sensitivity and smoothing, can be configured via set/get methods.
 *
 * This class is lightweight in terms of computation, making it suitable for
 * real-time use or high-frequency loops.
 *
 * Usage:
 *  - Call detect(position, direction) with the current signal value and its
 *    direction of change (-1 = falling, 0 = stopped, 1 = rising).
 *  - Detection parameters can be read or modified using the getter/setter methods.
 */
class OscillatorDetector {
public:
    OscillatorDetector() = default;
    ~OscillatorDetector() = default;

    /**
     * @brief Detects oscillatory behavior by tracking local maxima and minima.
     *
     * This function analyzes the current signal `position` and its `direction`
     * to identify extrema (peaks and troughs). It uses debounce counters and
     * a smoothing threshold to avoid false detections caused by small fluctuations.
     *
     * @param position The current value of the signal.
     * @param direction The direction of change:
     *                  -1 = falling
     *                   0 = stopped
     *                   1 = rising
     * @return true if the number of detected extrema exceeds the sensitivity threshold.
     */
    bool detect(int64_t position, int direction) {
        bool reset{ false };
        bool maximumFound = (m_internals.lastDirection > 0 && direction <= 0);
        bool minimumFound = (m_internals.lastDirection < 0 && direction >= 0);

        if (maximumFound) {
            if (m_internals.maxFoundPos <= position && m_internals.maximumDebounceCounter++ == 0) {
                ++m_internals.extremaCounter;
                m_internals.maxFoundPos = position;
                m_internals.minimumDebounceCounter = 0;
            }
            else if (m_internals.maxFoundPos <= position) {
                m_internals.maxFoundPos = position;
            }
            else if (m_internals.maximumDebounceCounter > m_params.smootherThreshold) {
                reset = true;
            }
        }

        if (minimumFound) {
            if (m_internals.minFoundPos >= position && m_internals.minimumDebounceCounter++ == 0) {
                ++m_internals.extremaCounter;
                m_internals.minFoundPos = position;
                m_internals.maximumDebounceCounter = 0;
            }
            else if (m_internals.minFoundPos >= position) {
                m_internals.minFoundPos = position;
            }
            else if (m_internals.minimumDebounceCounter > m_params.smootherThreshold) {
                reset = true;
            }
        }

        if (reset) {
            m_internals = {}; // reset all internals to their default-initialized values
        }

        m_internals.lastDirection = direction;
        return m_internals.extremaCounter > m_params.sensitivity;
    }

    /**
     * @brief Set the smoothing threshold used to debounce extrema detection.
     * @param threshold Number of updates required to confirm an extremum.
     */
    void setSmootherThreshold(uint8_t threshold) {
        m_params.smootherThreshold = threshold;
    }

    /**
     * @brief Set the sensitivity threshold for the number of extrema needed to trigger detection.
     * @param sensitivity Number of extrema required to return true in detect().
     */
    void setSensitivity(uint8_t sensitivity) {
        m_params.sensitivity = sensitivity;
    }

    /**
     * @brief Get the current smoothing threshold.
     * @return The smoothing threshold value.
     */
    uint8_t getSmootherThreshold() const {
        return m_params.smootherThreshold;
    }

    /**
     * @brief Get the current sensitivity threshold.
     * @return The sensitivity threshold value.
     */
    uint8_t getSensitivity() const {
        return m_params.sensitivity;
    }

private:
    struct {
        uint8_t smootherThreshold{ 5 };
        uint8_t sensitivity{ 5 };
    } m_params;

    struct {
        uint8_t extremaCounter{ 0 };
        int lastDirection{ 0 };
        int64_t minFoundPos{ std::numeric_limits<int64_t>::max() };
        int64_t maxFoundPos{ std::numeric_limits<int64_t>::min() };
        uint8_t minimumDebounceCounter{ 0 };
        uint8_t maximumDebounceCounter{ 0 };
    } m_internals;
};
