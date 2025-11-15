#ifndef __EXTRAS_SINE_GENERATION_H__
#define __EXTRAS_SINE_GENERATION_H__

#include <vector>
#include <random>
#include <cmath>
#include <numeric>

struct NoisySineConfig {
    double amplitude = 0.75;    // Sine wave amplitude
    double frequency = 5.0;     // Frequency [Hz]
    double fs = 1000.0;         // Sampling frequency [Hz]
    double duration = 1.0;      // Duration [s]
    double SNR_dB = 35.0;       // Signal-to-noise ratio [dB]
    int16_t ADCBits = 12;       // ADC resolution in bits
};

class NoisySine {
private:
    NoisySineConfig config;

    size_t sample = 0;
    std::vector<double> sine {};
public:

     explicit NoisySine(const NoisySineConfig& cfg = NoisySineConfig())
        : config(cfg) {  }

    uint16_t GetNextSample() {
        if( sample == sine.size() ) {
            const auto pureSine = generateSine();
            sine = addNoise(pureSine);
            sample = 0;
        }
        return toADCValue(sine[sample++]);
    }

private:

    std::vector<double> generateSine() {
        size_t N = static_cast<size_t>(config.fs * config.duration);
        std::vector<double> signal(N);

        const double dt = 1.0 / config.fs;
        const double two_pi_f = 2.0 * M_PI * config.frequency;

        for (size_t n = 0; n < N; ++n) {
            signal[n] = config.amplitude * std::sin(two_pi_f * n * dt);
        }

        return signal;
    }

    std::vector<double> addNoise(const std::vector<double>& signal) {
        size_t N = signal.size();
        std::vector<double> noise(N);

        std::mt19937 gen(std::random_device{}());
        std::uniform_real_distribution<double> dist(-0.5, 0.5);

        for (size_t i = 0; i < N; ++i) {
            noise[i] = dist(gen);
        }

        auto var = [](const std::vector<double>& x) {
            double mean = std::accumulate(x.begin(), x.end(), 0.0) / x.size();
            double accum = 0.0;
            for (double v : x) accum += (v - mean) * (v - mean);
            return accum / x.size();
        };

        const double var_signal = var(signal);
        const double var_noise = var(noise);

        double factor = std::sqrt( var_signal / ( var_noise * std::pow(10.0, config.SNR_dB / 10.0) ) );
        for (size_t i = 0; i < N; ++i) {
            noise[i] *= factor;
        }

        std::vector<double> signal_noise(N);
        for (size_t i = 0; i < N; ++i) {
            signal_noise[i] = signal[i] + noise[i];
        }

        return signal_noise;
    }

    uint16_t toADCValue(double toDigital) {
        int16_t maxVal = (1 << (config.ADCBits-1)); 
        
        auto res = static_cast<int16_t>(toDigital * static_cast<double>(maxVal));
        
        // Saturation
        if (res < -maxVal) res = -maxVal;
        if (res > maxVal - 1) res = maxVal - 1;

        return res + maxVal;
    }

};

#endif 