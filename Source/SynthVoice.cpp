//
//  SynthVoice.cpp
//  FilterSynth
//
//  Created by Joseph Cleveland on 8/20/20.
//  Copyright © 2020 JoeCleveland. All rights reserved.
//

#include "SynthVoice.h"

#define SRATE 44100.0

juce::File logfile("~/Plugin.log");
juce::FileLogger fl(logfile, "Welcome to the Dojo, Brother", 1000);

void logarray(float* a, int len){
    std::string s;
    for (int i = 0; i < len; i++)
        s += std::to_string(a[i]) + ", ";
}

SynthVoice::SynthVoice(int notenum, juce::dsp::FFT* fft, std::array<Parameter, NPARAMS>& params) {
    angle_1 = 0;
    angle_2 = 0;
    note = notenum;
    
    //std::random_device rd;
    //std::mt19937 e2(rd());
    //std::normal_distribution<float> dist(-1, 1);
   
    //init filter
    for(int i = 0; i < KERN-1; i++){
        filterBuffer[i] = 0;
    }
    
    //init delay line
    lastSample1 = 0;
    lastSample2 = 0;
    
    //utility
    this->fft = fft;
    
    //envelopes
    amplitude = Envelope(params[14], params[15], params[16], params[17]);
    amplitude.start();
}

void SynthVoice::updateBlock(std::array<Parameter, NPARAMS>& params){
    makeKern(kernel, params[6], params[7]);
}

double SynthVoice::osc(double angle, double freq, double phase, double wave, double shape){
    return wave * harmonicWave(angle, freq, phase, shape) + (1-wave)*sawtooth(angle, freq, phase, shape);
}

double SynthVoice::sawtooth(double angle, double freq, double phase, double shape){
    //float ratio = 1000 - fmax(freq - 1000, 0)/10;
    return tanh(sin(angle + phase)*shape*200)*cos(angle);
}

double SynthVoice::harmonicWave(double angle, double freq, double phase, double shape){
    return cos(tanh(sin(angle + phase)) * (3 + 40*shape));
}

float SynthVoice::getValue(std::array<Parameter, NPARAMS>& params){
    //update envlopes
    amplitude.update(params[14], params[15], params[16], params[17]);
    
    double freq = pow(2.0, (note - 69.0)/12.0) * 220.0;
    //oscillator
    //double phase = 0;
    double phase = sin(1 * M_PI / SRATE);
    
    angle_1 += freq * 2 * M_PI / SRATE;
    angle_2 += (freq + (double)params[4]*3) * 2 * M_PI / SRATE;
    
    if(angle_1 > 2 * M_PI)
        angle_1 -= 2*M_PI;
    if(angle_2 > 2 * M_PI)
        angle_2 -= 2*M_PI;
    
    double value = osc(angle_1, freq, phase, params[0], params[1]) * (double)params[5];
    value      += osc(angle_2, freq + (double)params[4]*3, phase, params[2], params[3]) * (1 - (double)params[5]);
    value *= amplitude.getValue();
    
    //filter
    value = tanh(filter(
                   (value + lastSampleFilter*(float)params[7]*0.1)*(1 + 10*(float)params[9])
                   ));
    lastSampleFilter = value;
    //delay line1

    float feedback = lastSample1*0.999*(float)params[11];
    float mix = tanh(value + feedback);
    float dlOut1 = dl1.process(mix, (float)params[10]);
    lastSample1 = dlOut1;
    
    feedback = lastSample2*0.999*(float)params[13];
    mix = tanh(value + feedback);
    float dlOut2 = dl2.process(mix, 1.0/note * 0.9);
    lastSample2 = dlOut2;
    
    return tanh(dlOut1*(float)params[12] + dlOut2*(1-(float)params[12])) * amplitude.getReleaseValue();
}

void SynthVoice::makeKern(float* kernel, float cutoff, float res){
    
    float response[RESP * 2];
    //Generate freq response
    cutoff = pow(cutoff, M_E);
    for(int i = 0; i < RESP; i++){
        float x = i/(float)RESP;
        float res_component = -tanh(x*1000 - 100*(cutoff-0.1))*tanh(x*1000 - 100*(cutoff-0.05))*8*res+8*res;
        response[i]  = tanh(-x * 10 + (cutoff-0.2)*12)/2 + 0.5 + res_component;
    }
    //Convert to impulse response
    fft->performRealOnlyInverseTransform(response);
    //Write shifted kernel
    kernel[KERN/2] = response[0];
    for(int i = 1; i < KERN/2; i++){
        kernel[KERN/2 + i] = response[i];
        kernel[KERN/2 - i] = response[i];
    }
    //Hamming window
    for(int i = 0; i < KERN; i++)
        kernel[i] *= 0.54 - 0.46*cos(2*M_PI/(KERN-1));
}

float SynthVoice::filter(float input){
    //Convolve
    float newvalue = 0;
    for(int i = 0; i < KERN-1; i++)
        newvalue += kernel[i] * filterBuffer[i];
    newvalue += kernel[KERN-1] * input;

    //shift buffer
    for(int i = 0; i < KERN-2; i++)
        filterBuffer[i] = filterBuffer[i + 1];
    filterBuffer[KERN-2] = input;
    
    return newvalue;
}

void SynthVoice::play(){
    amplitude.start();
}

void SynthVoice::stop(){
    amplitude.stop();
}

bool SynthVoice::isFinished(){
    return amplitude.finished();
    //return false;
}


//~~~~~~~~~~~~~~~~ DELAY LINE ~~~~~~~~~~~~~~~

DelayLine::DelayLine(){
    for(int i = 0; i < DELAY; i++){
        delayBuffer[i] = 0;
    }
    delayPtr = 0;
    lastBuffer = 0;
}

float DelayLine::process(float input, float length){
    float L = length * 1024 + 10;
    
    float r = L - floor(L);
    
    float output = delayBuffer[delayPtr];
    delayBuffer[delayPtr] = input;
    
    float delOutput = (1 - r) * output + (r) * lastBuffer;
    lastBuffer = output;
    
    //advance the delay line
    delayPtr++;
    delayPtr %= (int)(L);
    
    return delOutput;
}
