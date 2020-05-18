//
//  ofSynth2.hpp
//  ivaApp
//
//  ofSynth2 tries to unify the different ofSynth Classes implemented by A. Brennecke, M. Dittmann and S. Stimberg
//
//  Created by Markus Traber on 06.05.20.
//

#ifndef ofSynth2_hpp
#define ofSynth2_hpp

#include "ofDCO.h"
#include "ofDCOBaseTypes.h"
#include "ofSoundBuffer.h"
#include "ofMath.h"
#include "ofMathConstants.h"

//class ofSoundBuffer;

class ofSynth2 {
    
private:
    static constexpr float FREQ_INCREASE_FACTOR = 10.0;
    static constexpr float FREQ_DECREASE_FACTOR = -10.0;
    std::vector<ofDCO> oscillators;
    
    void changeFrequencyBy(const int& id, const float& factor);
public:
    static constexpr float FUNDAMENTAL_FREQ = 172.5;
    static constexpr float SAMPLE_RATE = 44100.0;
    static constexpr float AMPLITUDE = 0.5;
    
    /// Initializes Synth without any oscillators
    ofSynth2();
    ~ofSynth2(){};
    
    /// Fill the sound buffer with values of this synth
    /// @param buffer Sound buffer object to be filled
    void fillSoundBuffer(ofSoundBuffer& buffer);
    
    /// Set the frequency of an oscillator
    /// @param id The id of the oscillator
    /// @param freq The frequency you want to set
    void setFrequency(const int& id, const float& freq);
    
    /// Get the frequency of an oscillator
    /// @param id The id of the oscillator
    float getFrequency(const int& id);
    
    /// Increase the frequency of an oscillator
    /// @param id The id of the oscillator
    void increaseFrequency(const int& id);
    
    /// Decrease the frequency of an oscillator
    /// @param id The id of the oscillator
    void decreaseFrequency(const int& id);
    
    /// Set the sample rate of an oscillator
    /// @param id The id of the oscillator
    /// @param smplRate Sample rate to be set
    void setSampleRate(const int& id, const float& smplRate);
    
    /// Get the sample rate of an oscillator
    /// @param id The id of the oscillator
    float getSampleRate(const int& id);
    
    /// Set the amplitude of an oscillator
    /// @param id The id of the oscillator
    /// @param freq The frequency you want to set
    void setAmplitude(const int& id, const float& rate);
    
    /// Get the amplitude of an oscillator
    /// @param id The id of the oscillator
    float getAmplitude(const int& id);
    
    /// Add an oscillator to the synth with default parameters, returns index of oscillator
    /// @param oscillatorType Type of oscillator you want to create
    int addOscillator(const ofDCO::OscillatorType oscillatorType);
    
    /// Add an oscillator to the synth, returns index of oscillator
    /// @param oscillatorType Type of oscillator you want to create
    /// @param sampleRate Sample Rate
    /// @param fequency Frequency
    /// @param amplitude Amplitude
    int addOscillator(const ofDCO::OscillatorType oscillatorType, const float sampleRate, const float fequency, const float amplitude);

};

#endif /* ofSynth2_hpp */
