/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FilterSynthAudioProcessor::FilterSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), fft( ORDER_FFT ),
interface_params(*this, nullptr, juce::Identifier("Filter Synth"), {
    std::make_unique<juce::AudioParameterFloat> ("osc1_wave",
                                                 "Osc 1: Wave",
                                                 0.0f,
                                                 1.0f,
                                                 0.5f),
    std::make_unique<juce::AudioParameterFloat> ("osc1_shape",
                                                 "Osc 1: Shape",
                                                 0.0f,
                                                 1.0f,
                                                 0.5f),
    std::make_unique<juce::AudioParameterFloat> ("osc2_wave",
                                                 "Osc 2: Wave",
                                                 0.0f,
                                                 1.0f,
                                                 0.5f),
    std::make_unique<juce::AudioParameterFloat> ("osc2_shape",
                                                 "Osc 2: Shape",
                                                 0.0f,
                                                 1.0f,
                                                 0.5f),
    std::make_unique<juce::AudioParameterFloat> ("osc2_freq",
                                                 "Osc 2: Freq",
                                                 0.0f,
                                                 1.0f,
                                                 0.5f),
    std::make_unique<juce::AudioParameterFloat> ("osc_mix",
                                                 "Osc: Mix",
                                                 0.0f,
                                                 1.0f,
                                                 0.5f),
    std::make_unique<juce::AudioParameterBool> ("osc_sync",
                                                 "Osc: Sync",
                                                 false),
    std::make_unique<juce::AudioParameterFloat> ("noise",
                                                 "Noise",
                                                 0.0f,
                                                 1.0f,
                                                 0.5f),
    std::make_unique<juce::AudioParameterFloat> ("filter_cutoff",
                                                 "Filter: Cutoff",
                                                 0.0f,
                                                 1.0f,
                                                 0.5f),
    std::make_unique<juce::AudioParameterFloat> ("filter_mode",
                                                 "Filter: Mode",
                                                 0.0f,
                                                 1.0f,
                                                 0.5f),
    std::make_unique<juce::AudioParameterFloat> ("filter_q",
                                                 "Filter: Q",
                                                 0.0f,
                                                 1.0f,
                                                 0.5f),
    std::make_unique<juce::AudioParameterFloat> ("filter_drive",
                                                 "Filter: Drive",
                                                 0.0f,
                                                 1.0f,
                                                 0.5f),
    std::make_unique<juce::AudioParameterFloat> ("del1_length",
                                                 "Delay 1: Length",
                                                 0.0f,
                                                 1.0f,
                                                 0.5f),
    std::make_unique<juce::AudioParameterFloat> ("del1_decay",
                                                 "Delay 1: Decay",
                                                 0.0f,
                                                 1.0f,
                                                 0.5f),
    std::make_unique<juce::AudioParameterFloat> ("del2_length",
                                                 "Delay 2: Length",
                                                 0.0f,
                                                 1.0f,
                                                 0.5f),
    std::make_unique<juce::AudioParameterFloat> ("del2_decay",
                                                 "Delay 2: Decay",
                                                 0.0f,
                                                 1.0f,
                                                 0.5f),
    std::make_unique<juce::AudioParameterFloat> ("ampenv_a",
                                                 "AmpEnv: Attack",
                                                 0.0f,
                                                 1.0f,
                                                 0.5f),
    std::make_unique<juce::AudioParameterFloat> ("ampenv_d",
                                                 "AmpEnv: Decay",
                                                 0.0f,
                                                 1.0f,
                                                 0.5f),
    std::make_unique<juce::AudioParameterFloat> ("ampenv_s",
                                                 "AmpEnv: Sustain",
                                                 0.0f,
                                                 1.0f,
                                                 0.5f),
    std::make_unique<juce::AudioParameterFloat> ("ampenv_r",
                                                 "AmpEnv: Release",
                                                 0.0f,
                                                 1.0f,
                                                 0.5f),
    std::make_unique<juce::AudioParameterFloat> ("filtenv_a",
                                                 "FilterEnv: Attack",
                                                 0.0f,
                                                 1.0f,
                                                 0.5f),
    std::make_unique<juce::AudioParameterFloat> ("filtenv_d",
                                                 "FilterEnv: Decay",
                                                 0.0f,
                                                 1.0f,
                                                 0.5f),
    std::make_unique<juce::AudioParameterFloat> ("filtenv_s",
                                                 "FilterEnv: Sustain",
                                                 0.0f,
                                                 1.0f,
                                                 0.5f),
    std::make_unique<juce::AudioParameterFloat> ("filtenv_r",
                                                 "FilterEnv: Release",
                                                 0.0f,
                                                 1.0f,
                                                 0.5f)

})
#endif
{

}

FilterSynthAudioProcessor::~FilterSynthAudioProcessor()
{

}

//==============================================================================
const juce::String FilterSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FilterSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FilterSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FilterSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FilterSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FilterSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FilterSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FilterSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FilterSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void FilterSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FilterSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    params[0] = Parameter(samplesPerBlock, interface_params.getRawParameterValue("osc1_wave"));
    params[1] = Parameter(samplesPerBlock, interface_params.getRawParameterValue("osc1_shape"));
    params[2] = Parameter(samplesPerBlock, interface_params.getRawParameterValue("osc2_wave"));
    params[3] = Parameter(samplesPerBlock, interface_params.getRawParameterValue("osc2_shape"));
    params[4] = Parameter(samplesPerBlock, interface_params.getRawParameterValue("osc2_freq"));
    params[5] = Parameter(samplesPerBlock, interface_params.getRawParameterValue("osc_mix"));
    params[6] = Parameter(samplesPerBlock, interface_params.getRawParameterValue("filter_cutoff"));
    params[7] = Parameter(samplesPerBlock, interface_params.getRawParameterValue("filter_q"));
    params[8] = Parameter(samplesPerBlock, interface_params.getRawParameterValue("filter_mode"));
    params[9] = Parameter(samplesPerBlock, interface_params.getRawParameterValue("filter_drive"));
    params[10] = Parameter(samplesPerBlock, interface_params.getRawParameterValue("del1_length"));
    params[11] = Parameter(samplesPerBlock, interface_params.getRawParameterValue("del1_decay"));
    params[12] = Parameter(samplesPerBlock, interface_params.getRawParameterValue("del2_length"));
    params[13] = Parameter(samplesPerBlock, interface_params.getRawParameterValue("del2_decay"));
    params[14] = Parameter(samplesPerBlock, interface_params.getRawParameterValue("ampenv_a"));
    params[15] = Parameter(samplesPerBlock, interface_params.getRawParameterValue("ampenv_d"));
    params[16] = Parameter(samplesPerBlock, interface_params.getRawParameterValue("ampenv_s"));
    params[17] = Parameter(samplesPerBlock, interface_params.getRawParameterValue("ampenv_r"));
}

void FilterSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FilterSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FilterSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    int length = buffer.getNumSamples();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    //Create and stop voices from midi messages
    int time;
    juce::MidiMessage msg;
    for (juce::MidiBuffer::Iterator i (midiMessages); i.getNextEvent (msg, time);){
        int note = msg.getNoteNumber();
        bool found = false;
        for (int v = 0; v < voices.size(); v++){
            if(voices[v].note == note){
                found = true;
                if(msg.isNoteOn()){
                    voices[v].play();
                }
                if(msg.isNoteOff())
                    voices[v].stop();
            }
        }
        if(!found && msg.isNoteOn()){
            voices.push_back(SynthVoice(note, &fft, params));
        }
    }
    
    //Delete unused voices or update them
    for (int v = 0; v < voices.size(); v++){
        if(voices[v].isFinished())
            voices.erase(voices.begin() + v);
        else
            voices[v].updateBlock(params);
    }
    
    //Set param values each block
    for(auto& p : params){
        p.setValue();
    }
    
    float* LData = buffer.getWritePointer (0);
    float* RData = buffer.getWritePointer (1);
    
    //get audio
    for (int i = 0; i < length; i++){//Each sample...
        LData[i] = 0;
        for (int v = 0; v < voices.size(); v++){//...get value from each voice
            LData[i] += voices[v].getValue(params);
            RData[i] = LData[i];
        }
        
        for(auto& p : params)//...update parameters
            p.update();
    }
    
}

//==============================================================================
bool FilterSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FilterSynthAudioProcessor::createEditor()
{
    return new FilterSynthAudioProcessorEditor (*this);
}

//==============================================================================
void FilterSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FilterSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FilterSynthAudioProcessor();
}
