/*
  ==============================================================================

    @yukinsnow
    Sodatune.com

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Bilifans.h"

//==============================================================================
FansAudioProcessorEditor::FansAudioProcessorEditor (FansAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    // Set global font.
    font.setStyleFlags(1);
    font.setHeight(20.0f);

    // Set name and fans label.
    name.setColour(juce::Label::textColourId, juce::Colours::white);
    name.setFont(font);
    name.setJustificationType(juce::Justification::centred);
    fans.setColour(juce::Label::textColourId, juce::Colours::white);
    fans.setFont(font);
    fans.setJustificationType(juce::Justification::centred);

    // Set input properties.
    input.setColour(juce::TextEditor::textColourId, juce::Colours::white);
    input.setColour(juce::TextEditor::backgroundColourId, juce::Colours::white.withAlpha(0.0f));
    input.setFont(font);
    input.setJustification(juce::Justification::centred);

    // This func will pass the Return key press to the parent component.
    input.setEscapeAndReturnKeysConsumed(false);

    // Restrict the input
    input.setInputRestrictions(0, "1234567890");

    // Get state from DAW, if there is null, it will show my info.
    if(audioProcessor.getMid() == 0)
        setLabel(123372);
    else
        setLabel(audioProcessor.getMid());

    // Manage the layout between two labels
    stretchableManager.setItemLayout(0, -1, -1, -1);
    stretchableManager.setItemLayout(1, -1, -1, -1);

    // Set the button.
    button.setAlpha(0.9f);
    button.onClick = [this] { Setting(true); };

    // Use the light theme.
    look.setColourScheme(juce::LookAndFeel_V4::getLightColourScheme());
    getLookAndFeel().setDefaultLookAndFeel(&look);

    //set default font to avoid problem when building on Windows
    //Microsoft YaHei UI are licensed to use for all Windows softwares
    if(juce::SystemStats::getOperatingSystemName() == "Windows")
        getLookAndFeel().setDefaultSansSerifTypefaceName("Microsoft YaHei UI");

    // Load components and make them visible.
    addAndMakeVisible(name);
    addAndMakeVisible(fans);
    addAndMakeVisible(button);

    // Set the mainwindow size.
    setSize(300, 300);
}

FansAudioProcessorEditor::~FansAudioProcessorEditor()
{
}

//==============================================================================
void FansAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    auto r = getLocalBounds();
    
    // Load image from memory.
    g.drawImageWithin(image, 0, 0, 300, 300, juce::RectanglePlacement::stretchToFit);
    g.setColour(juce::Colours::black);
    g.setOpacity(0.5f);
    g.fillRect(r.getX(), r.getHeight() * 0.8, r.getWidth(), r.getHeight() * 0.099);
    g.fillRect(r.getX(), r.getHeight() * 0.9, r.getWidth(), r.getHeight() * 0.099);
}

void FansAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    // Here is layout of all component.
    auto r = getLocalBounds();
    Component* comps[] = { &name,&fans };
    stretchableManager.layOutComponents(comps, 2,
        r.getX(), r.getHeight() * 0.8, r.getWidth(), r.getHeight() * 0.1,
        true, true);

    juce::Rectangle<int> buttonSize(0, 0, 28, 28);

    juce::Rectangle<int> area((getWidth()) - (buttonSize.getWidth()),
        0,
        buttonSize.getWidth(), buttonSize.getHeight());

    button.setBounds(area.reduced(2));

    juce::Rectangle<int> inputSize(r.getWidth()*0.4, r.getHeight() * 0.1);
    juce::Rectangle<int> areaInput((getWidth() / 2) - (inputSize.getWidth() / 2),
        r.getHeight() * 0.8,
        inputSize.getWidth(), inputSize.getHeight());
    input.setBounds(areaInput.reduced(2));
}
void FansAudioProcessorEditor::closeButtonPressed()
{
    delete this;
}

void FansAudioProcessorEditor::setLabel(int mid)
{
    // Get the bilibili info from my curl function.
    tuple<string, int, string> result = Bilifans(mid);
    string biliname;
    int bilifans;
    string biliface;
    tie(biliname, bilifans, biliface) = result;

    
    // Convert url from http to https.
    if(biliface.substr(0,5) != "https")
        biliface = "https" + biliface.substr(4,-1);
    name.setText(juce::CharPointer_UTF8("\xe7\x94\xa8\xe6\x88\xb7\xe5\x90\x8d\xef\xbc\x9a") + biliname, juce::dontSendNotification);
    fans.setText(juce::CharPointer_UTF8("\xe7\xb2\x89\xe4\xb8\x9d\xe6\x95\xb0\xef\xbc\x9a") + to_string(bilifans), juce::dontSendNotification);

    // Download the bili's head and store to memory.
    juce::URL imageLink(biliface+"@1c.png"); //force .png to avoid problem 
            juce::MemoryBlock memoryBlock;
            if(imageLink.readEntireBinaryStream(memoryBlock))
            {
                juce::Image buttonImage = juce::ImageFileFormat::loadFrom(memoryBlock.getData(), memoryBlock.getSize());
            }

    // Set the image from memory.
    image = juce::ImageCache::getFromMemory(memoryBlock.getData(), memoryBlock.getSize());

    // Repaint the main window.
    this->repaint();
}

// The Setting function when button onclick.
void FansAudioProcessorEditor::Setting(bool native)
{
    this->name.setVisible(false);
    //mainWindow->fans.setVisible(false);
    this->fans.setText("Bili Uid", juce::dontSendNotification);
    this->addAndMakeVisible(input);
    this->input.setTextToShowWhenEmpty("123372", juce::Colours::white.withAlpha(0.5f));

    if (!input.isEmpty())
    {
        int mid = this->input.getTextValue().toString().getIntValue();
        setLabel(mid);
        this->audioProcessor.setMid(mid);
        this->input.setVisible(false);
        this->name.setVisible(true);
        this->fans.setVisible(true);
        input.setText("", false);
    }
}

// Set the return key function.
bool FansAudioProcessorEditor::keyPressed(const juce::KeyPress& key)
{
    if (key == juce::KeyPress::returnKey)
    {
        Setting(true);
        return true;
    }
    return false;
}
