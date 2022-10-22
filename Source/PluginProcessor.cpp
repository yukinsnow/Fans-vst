/*
  ==============================================================================

    @yukinsnow
    Sodatune.com

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FansAudioProcessor::FansAudioProcessor()
{
}

FansAudioProcessor::~FansAudioProcessor()
{
}

//==============================================================================
const juce::String FansAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FansAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FansAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FansAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FansAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FansAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FansAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FansAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FansAudioProcessor::getProgramName (int index)
{
    return {};
}

void FansAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FansAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void FansAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FansAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void FansAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool FansAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

void FansAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    // Here is my implement to store the bilibili mid.
    juce::String Tag = "mid";
    auto xmlTemp = new juce::XmlElement(Tag);
    std::unique_ptr<juce::XmlElement> xml(xmlTemp);
    xml->addTextElement(std::to_string(mid));
    copyXmlToBinary(*xml, destData);
}

void FansAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    // Here is my implement to get the uid from DAW's xml.
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));

    if (xml.get() != nullptr)
        if (xml->hasTagName("mid"))
            mid = xml->getAllSubText().getIntValue();
            //mid = xml->getByName("mid")->getText().getIntValue();
}

int FansAudioProcessor::getMid()
{
    return mid;
}

void FansAudioProcessor::setMid(int midTemp)
{
    mid = midTemp;
}

juce::AudioProcessorEditor* FansAudioProcessor::createEditor()
{
    return new FansAudioProcessorEditor(*this);
}

//==============================================================================

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FansAudioProcessor();
}
