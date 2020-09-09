/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FilterSynthAudioProcessorEditor::FilterSynthAudioProcessorEditor (FilterSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 600);
    
    initSlider(slider_osc1_wave, label_osc1_wave, "Wave");
    attach_osc1_wave.reset (new SliderAttachment (audioProcessor.interface_params, "osc1_wave", slider_osc1_wave));
    
    initSlider(slider_osc1_shape, label_osc1_shape, "Shape");
    attach_osc1_shape.reset (new SliderAttachment (audioProcessor.interface_params, "osc1_shape", slider_osc1_shape));
    
    initSlider(slider_osc2_wave, label_osc2_wave, "Wave");
    attach_osc2_wave.reset (new SliderAttachment (audioProcessor.interface_params, "osc2_wave", slider_osc2_wave));
    
    initSlider(slider_osc2_shape, label_osc2_shape, "Shape");
    attach_osc2_shape.reset (new SliderAttachment (audioProcessor.interface_params, "osc2_shape", slider_osc2_shape));
    
    initSlider(slider_osc2_freq, label_osc2_freq, "Freq");
    attach_osc2_freq.reset (new SliderAttachment (audioProcessor.interface_params, "osc2_freq", slider_osc2_freq));
    
    initSlider(slider_osc_mix, label_osc_mix, "Mix");
    attach_osc_mix.reset (new SliderAttachment (audioProcessor.interface_params, "osc_mix", slider_osc_mix));
    
    initSlider(slider_filter_cutoff, label_filter_cutoff, "Cutoff");
    attach_filter_cutoff.reset (new SliderAttachment (audioProcessor.interface_params, "filter_cutoff", slider_filter_cutoff));
    
    initSlider(slider_filter_q, label_filter_q, "Q");
    attach_filter_q.reset (new SliderAttachment (audioProcessor.interface_params, "filter_q", slider_filter_q));
    
    initSlider(slider_filter_mode, label_filter_mode, "Mode");
    attach_filter_mode.reset (new SliderAttachment (audioProcessor.interface_params, "filter_mode", slider_filter_mode));
    
    initSlider(slider_filter_drive, label_filter_drive, "Drive");
    attach_filter_drive.reset (new SliderAttachment (audioProcessor.interface_params, "filter_drive", slider_filter_drive));
    
    initSlider(slider_del1_length, label_del1_length, "Length");
    attach_del1_length.reset (new SliderAttachment (audioProcessor.interface_params, "del1_length", slider_del1_length));
    
    initSlider(slider_del1_decay, label_del1_decay, "Decay");
    attach_del1_decay.reset (new SliderAttachment (audioProcessor.interface_params, "del1_decay", slider_del1_decay));
    
    initSlider(slider_del2_length, label_del2_length, "Length");
    attach_del2_length.reset (new SliderAttachment (audioProcessor.interface_params, "del2_length", slider_del2_length));
    
    initSlider(slider_del2_decay, label_del2_decay, "Decay");
    attach_del2_decay.reset (new SliderAttachment (audioProcessor.interface_params, "del2_decay", slider_del2_decay));
    
    initSlider(slider_ampenv_a, label_ampenv_a, "A");
    attach_ampenv_a.reset (new SliderAttachment (audioProcessor.interface_params, "ampenv_a", slider_ampenv_a));
    
    initSlider(slider_ampenv_d, label_ampenv_d, "D");
    attach_ampenv_d.reset (new SliderAttachment (audioProcessor.interface_params, "ampenv_d", slider_ampenv_d));
    
    initSlider(slider_ampenv_s, label_ampenv_s, "S");
    attach_ampenv_s.reset (new SliderAttachment (audioProcessor.interface_params, "ampenv_s", slider_ampenv_s));
    
    initSlider(slider_ampenv_r, label_ampenv_r, "R");
    attach_ampenv_r.reset (new SliderAttachment (audioProcessor.interface_params, "ampenv_r", slider_ampenv_r));
}

FilterSynthAudioProcessorEditor::~FilterSynthAudioProcessorEditor()
{
}

//==============================================================================

void FilterSynthAudioProcessorEditor::initSlider(juce::Slider& s, juce::Label& l, std::string text){
    s.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    s.setRange (0.0, 1.0, 0.05);
    s.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    s.setValue(0.5);
    addAndMakeVisible(s);
    
    l.setText(text, juce::dontSendNotification);
    l.attachToComponent(&s, false);
    addAndMakeVisible(l);
}

void FilterSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    //g.setColour (juce::Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Funky Boi", getLocalBounds(), juce::Justification::centred, 1);
}

void FilterSynthAudioProcessorEditor::resized()
{
    
    slider_osc1_wave.setTopLeftPosition(50, 50);
    slider_osc1_wave.setSize(40, 40);
    
    slider_osc1_shape.setTopLeftPosition(100, 50);
    slider_osc1_shape.setSize(40, 40);
    
    slider_osc2_wave.setTopLeftPosition(150, 50);
    slider_osc2_wave.setSize(40, 40);
    
    slider_osc2_shape.setTopLeftPosition(50, 100);
    slider_osc2_shape.setSize(40, 40);
    
    slider_osc2_freq.setTopLeftPosition(100, 100);
    slider_osc2_freq.setSize(40, 40);
    
    slider_osc_mix.setTopLeftPosition(150, 100);
    slider_osc_mix.setSize(40, 40);
    
    slider_filter_cutoff.setTopLeftPosition(200, 50);
    slider_filter_cutoff.setSize(40, 40);
    
    slider_filter_q.setTopLeftPosition(250, 50);
    slider_filter_q.setSize(40, 40);
    
    slider_filter_mode.setTopLeftPosition(200, 100);
    slider_filter_mode.setSize(40, 40);
    
    slider_filter_drive.setTopLeftPosition(250, 100);
    slider_filter_drive.setSize(40, 40);
    
    slider_del1_length.setTopLeftPosition(50, 150);
    slider_del1_length.setSize(40, 40);
    
    slider_del1_decay.setTopLeftPosition(100, 150);
    slider_del1_decay.setSize(40, 40);
    
    slider_del2_length.setTopLeftPosition(150, 150);
    slider_del2_length.setSize(40, 40);
    
    slider_del2_decay.setTopLeftPosition(200, 150);
    slider_del2_decay.setSize(40, 40);
    
    slider_ampenv_a.setTopLeftPosition(50, 200);
    slider_ampenv_a.setSize(40, 40);
    
    slider_ampenv_d.setTopLeftPosition(100, 200);
    slider_ampenv_d.setSize(40, 40);
    
    slider_ampenv_s.setTopLeftPosition(150, 200);
    slider_ampenv_s.setSize(40, 40);
    
    slider_ampenv_r.setTopLeftPosition(200, 200);
    slider_ampenv_r.setSize(40, 40);
    
}

