/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment ;
//==============================================================================
/**
*/
class FilterSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FilterSynthAudioProcessorEditor (FilterSynthAudioProcessor&);
    ~FilterSynthAudioProcessorEditor() override;

    //==============================================================================
    void initSlider(juce::Slider& s, juce::Label& l, std::string text);
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FilterSynthAudioProcessor& audioProcessor;
    
    juce::Slider slider_osc1_wave;
    juce::Label label_osc1_wave;
    std::unique_ptr<SliderAttachment> attach_osc1_wave;
    
    juce::Slider slider_osc1_shape;
    juce::Label label_osc1_shape;
    std::unique_ptr<SliderAttachment> attach_osc1_shape;
    
    juce::Slider slider_osc2_wave;
    juce::Label label_osc2_wave;
    std::unique_ptr<SliderAttachment> attach_osc2_wave;
    
    juce::Slider slider_osc2_shape;
    juce::Label label_osc2_shape;
    std::unique_ptr<SliderAttachment> attach_osc2_shape;
    
    juce::Slider slider_osc2_freq;
    juce::Label label_osc2_freq;
    std::unique_ptr<SliderAttachment> attach_osc2_freq;
    
    juce::Slider slider_osc_mix;
    juce::Label label_osc_mix;
    std::unique_ptr<SliderAttachment> attach_osc_mix;
    
    juce::Slider slider_filter_cutoff;
    juce::Label label_filter_cutoff;
    std::unique_ptr<SliderAttachment> attach_filter_cutoff;
    
    juce::Slider slider_filter_q;
    juce::Label label_filter_q;
    std::unique_ptr<SliderAttachment> attach_filter_q;
    
    juce::Slider slider_filter_mode;
    juce::Label label_filter_mode;
    std::unique_ptr<SliderAttachment> attach_filter_mode;
    
    juce::Slider slider_filter_drive;
    juce::Label label_filter_drive;
    std::unique_ptr<SliderAttachment> attach_filter_drive;
    
    juce::Slider slider_del1_length;
    juce::Label label_del1_length;
    std::unique_ptr<SliderAttachment> attach_del1_length;
    
    juce::Slider slider_del1_decay;
    juce::Label label_del1_decay;
    std::unique_ptr<SliderAttachment> attach_del1_decay;
    
    juce::Slider slider_del2_length;
    juce::Label label_del2_length;
    std::unique_ptr<SliderAttachment> attach_del2_length;
    
    juce::Slider slider_del2_decay;
    juce::Label label_del2_decay;
    std::unique_ptr<SliderAttachment> attach_del2_decay;
    
    juce::Slider slider_ampenv_a;
    juce::Label label_ampenv_a;
    std::unique_ptr<SliderAttachment> attach_ampenv_a;
    
    juce::Slider slider_ampenv_d;
    juce::Label label_ampenv_d;
    std::unique_ptr<SliderAttachment> attach_ampenv_d;
    
    juce::Slider slider_ampenv_s;
    juce::Label label_ampenv_s;
    std::unique_ptr<SliderAttachment> attach_ampenv_s;
    
    juce::Slider slider_ampenv_r;
    juce::Label label_ampenv_r;
    std::unique_ptr<SliderAttachment> attach_ampenv_r;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterSynthAudioProcessorEditor)
};
