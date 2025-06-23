# Fans-vst

* A VST3 plugin for showing your Bilibili’s name and fans.  
* Made with JUCE.  
* Provide VST3 plugin, Audio unit and standalone application.  
* Support Windows(x64) and MacOS(Universe).  

## Build
Open Fans.jucer with Projucer
External libraries:

​								[json](https://github.com/nlohmann/json.git)

​								[libcurl](https://github.com/commontk/libcurl.git)

## Installation

Download the released package.  
And copy file to  
Windows:  
--VST3:C:\Program Files\Common Files\VST3  
MacOS:  
--VST3:/Library/Audio/Plug-Ins/VST3  
--AU:/Library/Audio/Plug-Ins/Components  

## Screenshots

VST3:  
<img src="img/vst3.png" alt="vst3" width="300">  
AU:  
<img src="img/au.png" alt="au" width="300">  
Standalone:  
<img src="img/standalone.png" alt="standalone" width="300">  

## Usage

Load the plugin on your DAW or run the standalone app.  
Setting the Uid by clicking the button in the top right corner or pressing Return.  
<img src="img/setting.png" alt="setting" width="300">  

## License

[GPL](https://choosealicense.com/licenses/gpl-3.0/)
