/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HackAudioChapter6AudioProcessor::HackAudioChapter6AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

HackAudioChapter6AudioProcessor::~HackAudioChapter6AudioProcessor()
{
}

//==============================================================================
const juce::String HackAudioChapter6AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HackAudioChapter6AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HackAudioChapter6AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HackAudioChapter6AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HackAudioChapter6AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HackAudioChapter6AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HackAudioChapter6AudioProcessor::getCurrentProgram()
{
    return 0;
}

void HackAudioChapter6AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String HackAudioChapter6AudioProcessor::getProgramName (int index)
{
    return {};
}

void HackAudioChapter6AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void HackAudioChapter6AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    sample_rate = 48000;
    frequency = 440.0;
}

void HackAudioChapter6AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HackAudioChapter6AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void HackAudioChapter6AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    x.setSize(totalNumInputChannels, buffer.getNumSamples());
    y.setSize(totalNumInputChannels, buffer.getNumSamples());
    gain = 0.5;
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample<buffer.getNumSamples(); ++sample)
        {
            x.setSample(channel, sample, buffer.getSample(channel, sample));
            y.setSample(channel, sample, x.getSample(channel, sample) * gain);
        }
    }
}

//==============================================================================
bool HackAudioChapter6AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HackAudioChapter6AudioProcessor::createEditor()
{
    return new HackAudioChapter6AudioProcessorEditor (*this);
}

//==============================================================================
void HackAudioChapter6AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HackAudioChapter6AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HackAudioChapter6AudioProcessor();
}
