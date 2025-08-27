#include "pch.h"
#include "OscillatorDetector.hpp"


#include <cmath>
#include <algorithm>

#define DEG2RAD(x) ((x) * 3.14159265358979323846 / 180.0)


TEST(OscillatorDetectorTest, DecreasingSin) {
	OscillatorDetector detector;

	const int samples = 7200;  //   / 360 = Y cycles
	double amplitude = 1000.0;


	int64_t prev = 0;

	bool detected = false;
	for (int i = 0; i <= samples; ++i) {
		amplitude -= 10 * i;
		amplitude = std::clamp(amplitude, 0.1, 1000.0);
		double value = amplitude * std::sin(DEG2RAD(i));

		int64_t position = static_cast<int64_t>(value);
		int direction = static_cast<int>(std::clamp(position - prev, int64_t{ -1 }, int64_t{ 1 }));

		detected |= detector.detect(position, direction);
		prev = position;
	}

	EXPECT_FALSE(detected);
}

TEST(OscillatorDetectorTest, SimpleSin) {
	OscillatorDetector detector;

	const int samples = 7200;  //   / 360 = Y cycles
	const double amplitude = 1000.0;


	int64_t prev = 0;

	bool detected = false;
	for (int i = 0; i <= samples; ++i) {
		double value = amplitude * std::sin(DEG2RAD(i));

		int64_t position = static_cast<int64_t>(value);
		int direction = static_cast<int>(std::clamp(position - prev, int64_t{ -1 }, int64_t{ 1 }));

		detected |= detector.detect(position, direction);
		prev = position;
	}

    EXPECT_TRUE(detected);
}

TEST(OscillatorDetectorTest, IncreasingSin) {
	OscillatorDetector detector;

	const int samples = 7200;  //   / 360 = Y cycles
	double amplitude = 1000.0;


	int64_t prev = 0;

	bool detected = false;
	for (int i = 0; i <= samples; ++i) {
		amplitude += 10 * i;
		double value = amplitude * std::sin(DEG2RAD(i));

		int64_t position = static_cast<int64_t>(value);
		int direction = static_cast<int>(std::clamp(position - prev, int64_t{ -1 }, int64_t{ 1 }));

		detected |= detector.detect(position, direction);
		prev = position;
	}

	EXPECT_TRUE(detected);
}


TEST(OscillatorDetectorTest, IncreasingSinSlow) {
    OscillatorDetector detector;
    const int samples = 7200;
    double amplitude = 1000.0;

    int64_t prev = 0;
    bool detected = false;

    for (int i = 0; i <= samples; ++i) {
        amplitude += 1.0;
        double value = amplitude * std::sin(DEG2RAD(i));
        int64_t position = static_cast<int64_t>(value);
        int direction = static_cast<int>(std::clamp(position - prev, int64_t{ -1 }, int64_t{ 1 }));
        detected |= detector.detect(position, direction);
        prev = position;
    }

    EXPECT_TRUE(detected);
}

TEST(OscillatorDetectorTest, IncreasingSinFast) {
    OscillatorDetector detector;
    const int samples = 7200;
    double amplitude = 500.0;

    int64_t prev = 0;
    bool detected = false;

    for (int i = 0; i <= samples; ++i) {
        amplitude += 5.0;
        double value = amplitude * std::sin(DEG2RAD(i * 3));
        int64_t position = static_cast<int64_t>(value);
        int direction = static_cast<int>(std::clamp(position - prev, int64_t{ -1 }, int64_t{ 1 }));
        detected |= detector.detect(position, direction);
        prev = position;
    }

    EXPECT_TRUE(detected);
}

TEST(OscillatorDetectorTest, SmallAmplitudeSlow) {
    OscillatorDetector detector;
    const int samples = 7200;
    double amplitude = 50.0;

    int64_t prev = 0;
    bool detected = false;

    for (int i = 0; i <= samples; ++i) {
        amplitude += 0.1;
        double value = amplitude * std::sin(DEG2RAD(i));
        int64_t position = static_cast<int64_t>(value);
        int direction = static_cast<int>(std::clamp(position - prev, int64_t{ -1 }, int64_t{ 1 }));
        detected |= detector.detect(position, direction);
        prev = position;
    }

    EXPECT_TRUE(detected);
}

TEST(OscillatorDetectorTest, VeryFastFrequency) {
    OscillatorDetector detector;
    const int samples = 7200;
    double amplitude = 1000.0;

    int64_t prev = 0;
    bool detected = false;

    for (int i = 0; i <= samples; ++i) {
        amplitude += 2.0;
        double value = amplitude * std::sin(DEG2RAD(i * 10));
        int64_t position = static_cast<int64_t>(value);
        int direction = static_cast<int>(std::clamp(position - prev, int64_t{ -1 }, int64_t{ 1 }));
        detected |= detector.detect(position, direction);
        prev = position;
    }

    EXPECT_TRUE(detected);
}

TEST(OscillatorDetectorTest, VerySmallAmplitude) {
    OscillatorDetector detector;
    const int samples = 7200;
    double amplitude = 5.0;

    int64_t prev = 0;
    bool detected = false;

    for (int i = 0; i <= samples; ++i) {
        amplitude += 0.05;
        double value = amplitude * std::sin(DEG2RAD(i));
        int64_t position = static_cast<int64_t>(value);
        int direction = static_cast<int>(std::clamp(position - prev, int64_t{ -1 }, int64_t{ 1 }));
        detected |= detector.detect(position, direction);
        prev = position;
    }

    EXPECT_TRUE(detected);
}

TEST(OscillatorDetectorTest, LinearIncrease) {
    OscillatorDetector detector;
    const int samples = 7200;

    int64_t prev = 0;
    bool detected = false;

    for (int i = 0; i <= samples; ++i) {
        double value = i;
        int64_t position = static_cast<int64_t>(value);
        int direction = static_cast<int>(std::clamp(position - prev, int64_t{ -1 }, int64_t{ 1 }));
        detected |= detector.detect(position, direction);
        prev = position;
    }

    EXPECT_FALSE(detected);
}

TEST(OscillatorDetectorTest, OscillatingSmallRise) {
    OscillatorDetector detector;
    const int samples = 7200;
    double amplitude = 100.0;

    int64_t prev = 0;
    bool detected = false;

    for (int i = 0; i <= samples; ++i) {
        amplitude += 0.5;
        double value = amplitude * std::sin(DEG2RAD(i));
        int64_t position = static_cast<int64_t>(value);
        int direction = static_cast<int>(std::clamp(position - prev, int64_t{ -1 }, int64_t{ 1 }));
        detected |= detector.detect(position, direction);
        prev = position;
    }

    EXPECT_TRUE(detected);
}

TEST(OscillatorDetectorTest, HighFrequencySmallRise) {
    OscillatorDetector detector;
    const int samples = 7200;
    double amplitude = 50.0;

    int64_t prev = 0;
    bool detected = false;

    for (int i = 0; i <= samples; ++i) {
        amplitude += 0.2;
        double value = amplitude * std::sin(DEG2RAD(i * 5));
        int64_t position = static_cast<int64_t>(value);
        int direction = static_cast<int>(std::clamp(position - prev, int64_t{ -1 }, int64_t{ 1 }));
        detected |= detector.detect(position, direction);
        prev = position;
    }

    EXPECT_TRUE(detected);
}

TEST(OscillatorDetectorTest, VeryHighFrequency) {
    OscillatorDetector detector;
    const int samples = 7200;
    double amplitude = 200.0;

    int64_t prev = 0;
    bool detected = false;

    for (int i = 0; i <= samples; ++i) {
        amplitude += 1.0;
        double value = amplitude * std::sin(DEG2RAD(i * 20));
        int64_t position = static_cast<int64_t>(value);
        int direction = static_cast<int>(std::clamp(position - prev, int64_t{ -1 }, int64_t{ 1 }));
        detected |= detector.detect(position, direction);
        prev = position;
    }

    EXPECT_TRUE(detected);
}

TEST(OscillatorDetectorTest, AlternatingIncreaseDecrease) {
    OscillatorDetector detector;
    const int samples = 7200;
    double amplitude = 100.0;

    int64_t prev = 0;
    bool detected = false;

    for (int i = 0; i <= samples; ++i) {
        amplitude += (i % 2 == 0) ? 1.0 : -0.5;
        double value = amplitude * std::sin(DEG2RAD(i * 2));
        int64_t position = static_cast<int64_t>(value);
        int direction = static_cast<int>(std::clamp(position - prev, int64_t{ -1 }, int64_t{ 1 }));
        detected |= detector.detect(position, direction);
        prev = position;
    }

    EXPECT_TRUE(detected);
}

TEST(OscillatorDetectorTest, LinearDecrease) {
    OscillatorDetector detector;
    const int samples = 7200;

    int64_t prev = samples;
    bool detected = false;

    for (int i = 0; i <= samples; ++i) {
        double value = samples - i;
        int64_t position = static_cast<int64_t>(value);
        int direction = static_cast<int>(std::clamp(position - prev, int64_t{ -1 }, int64_t{ 1 }));
        detected |= detector.detect(position, direction);
        prev = position;
    }

    EXPECT_FALSE(detected);
}

TEST(OscillatorDetectorTest, DecreasingSinSlow) {
    OscillatorDetector detector;
    const int samples = 7200;
    double amplitude = 1000.0;

    int64_t prev = 0;
    bool detected = false;

    for (int i = 0; i <= samples; ++i) {
        amplitude -= 0.5;
        amplitude = std::clamp(amplitude, 0.1, 1000.0);
        double value = amplitude * std::sin(DEG2RAD(i));
        int64_t position = static_cast<int64_t>(value);
        int direction = static_cast<int>(std::clamp(position - prev, int64_t{ -1 }, int64_t{ 1 }));
        detected |= detector.detect(position, direction);
        prev = position;
    }

    EXPECT_FALSE(detected);
}

TEST(OscillatorDetectorTest, DecreasingSinFast) {
    OscillatorDetector detector;
    const int samples = 7200;
    double amplitude = 2000.0;

    int64_t prev = 0;
    bool detected = false;

    for (int i = 0; i <= samples; ++i) {
        amplitude -= 5.0;
        amplitude = std::clamp(amplitude, 0.1, 2000.0);
        double value = amplitude * std::sin(DEG2RAD(i * 5));
        int64_t position = static_cast<int64_t>(value);
        int direction = static_cast<int>(std::clamp(position - prev, int64_t{ -1 }, int64_t{ 1 }));
        detected |= detector.detect(position, direction);
        prev = position;
    }

    EXPECT_FALSE(detected);
}

TEST(OscillatorDetectorTest, DecreasingVerySmallAmplitude) {
    OscillatorDetector detector;
    const int samples = 7200;
    double amplitude = 20.0;

    int64_t prev = 0;
    bool detected = false;

    for (int i = 0; i <= samples; ++i) {
        amplitude -= 0.05;
        amplitude = std::clamp(amplitude, 0.1, 20.0);
        double value = amplitude * std::sin(DEG2RAD(i));
        int64_t position = static_cast<int64_t>(value);
        int direction = static_cast<int>(std::clamp(position - prev, int64_t{ -1 }, int64_t{ 1 }));
        detected |= detector.detect(position, direction);
        prev = position;
    }

    EXPECT_FALSE(detected);
}