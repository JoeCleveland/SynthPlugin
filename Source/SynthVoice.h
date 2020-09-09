//
//  SynthVoice.h
//  FilterSynth
//
//  Created by Joseph Cleveland on 8/20/20.
//  Copyright © 2020 JoeCleveland. All rights reserved.
//

#ifndef SynthVoice_h
#define SynthVoice_h

#include <stdio.h>
#include <math.h>
#include <JuceHeader.h>
#include <random>

#define PARAM_DT 0.5
#define NPARAMS 18

//Filter Kernel size, Should be an odd number
#define KERN 129

//Size of the freqeuncy response, order_fft should be log2(RESP)-1
#define RESP 2048
#define ORDER_FFT 9

//Length of the delay line
#define DELAY 3000
#define BUF_CLEAR_VAL 0.0000001

//Envelope consts
#define ENV_SPEED 0.001
#define MIN_ENV_DT 0.00000001

//PARAMATER
class Parameter{
public:
    Parameter() {}
    Parameter(int bs, std::atomic<float>* ref) {
        value = 0.5; newValue = 0.5; dT = 0; blockSize = bs; reference = ref;
    }
    void setValue() {
        newValue = *reference;
        dT = (newValue - value) / blockSize * PARAM_DT;
    }
    void update() {
        value += dT;
    }

    float getValue() { return value; }
    operator float() { return value; }
    operator double() { return double(value); }
    
private:
    float value;
    float newValue;
    float dT;
    std::atomic<float>* reference;
    int blockSize;
};

//ENVELOPE
class Envelope{
public:
    Envelope() {}
    Envelope(float a, float d, float s, float r){
        setParams(a, d, s, r);
        state = 0;
        value = 0;
        releaseValue = 1;
    }
    void setParams(float a, float d, float s, float r){
        dA = pow(1-a, M_E) * ENV_SPEED + MIN_ENV_DT;
        dD = pow(1-d, M_E) * ENV_SPEED + MIN_ENV_DT;
        S = s;
        dR = pow(1-r, M_E) * ENV_SPEED + MIN_ENV_DT;
    }
    void start() { state = 0; value = 0; releaseValue = 1;}
    void stop() { state = 3; }
    void update(float a, float d, float s, float r) {
        setParams(a, d, s, r);
        if(state == 0){
            value += dA;
            if (value >= 1.0)
                state = 1;
        }
        else if(state == 1){
            value -= dD;
            if(value <= S)
                state = 2;
        }
        else if(state == 2)
            value = S;
        else{
            value -= dR;
            releaseValue -= dR;
            value = fmax(0, value);
            releaseValue = fmax(0, releaseValue);
        }
    }
    float getValue() { return value; }
    float getReleaseValue() { return releaseValue; }
    bool finished() {
        return state == 3 && value <= MIN_ENV_DT && releaseValue <= MIN_ENV_DT;
    }
    
private:
    float dA, dD, S, dR;
    int state;
    float value;
    float releaseValue;
};

//DELAY LINES
class DelayLine{
public:
    DelayLine();
    float process(float input, float length);
private:
    float delayBuffer[DELAY];
    int delayPtr;
    float lastBuffer;//For fractional delay interpolation
};

//SYNTH VOICE
class SynthVoice{
public:
    SynthVoice(int notenum, juce::dsp::FFT* fft, std::array<Parameter, NPARAMS>& parameters);
    
    void updateBlock(std::array<Parameter, NPARAMS>& parameters);
    float getValue(std::array<Parameter, NPARAMS>& parameters);
    
    int note;
    void play();
    void stop();
    bool isFinished();
private:
    float angle_1;
    float angle_2;
    
    Envelope amplitude;
    Envelope filterEnv;
    
    //Oscillator
    double osc(double angle, double freq, double phase, double wave, double shape);
    double sawtooth(double angle, double freq, double phase, double shape);
    double harmonicWave(double angle, double freq, double phase, double shape);
    //Filter
    float kernel[KERN];
    float filterBuffer[KERN-1];
    void makeKern(float* kernel, float cutoff, float res);
    float filter(float input);
    float lastSampleFilter;
    
    //Delay line
    DelayLine dl1;
    DelayLine dl2;
    float lastSample1;
    float lastSample2;
    
    //Utility
    juce::dsp::FFT* fft;
};
#endif /* SynthVoice_h */
