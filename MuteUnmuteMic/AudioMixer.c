//
//  AudioMixer.c
//  MuteUnmuteMic
//
//  Created by Diogo Tridapalli on 10/5/15.
//  Copyright © 2015 CocoaHeads Brasil. All rights reserved.
//

#include "AudioMixer.h"

Boolean GetDefaultInputAudioDevice(AudioDeviceID *defaultInputDeviceID)
{
    UInt32 thePropSize = sizeof(AudioDeviceID);
    
    AudioObjectPropertyAddress thePropertyAddress =
    { kAudioHardwarePropertyDefaultInputDevice,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMaster };
    
    OSStatus errorCode =
    AudioObjectGetPropertyData(kAudioObjectSystemObject,
                               &thePropertyAddress,
                               0,
                               NULL,
                               &thePropSize,
                               defaultInputDeviceID);
    if (errorCode) {
        printf("Error in GetDefaultInputAudioDevice: %d\n", errorCode);
    }
    
    return errorCode == 0;
}

Boolean GetAudioDeviceName(const AudioDeviceID deviceID,
                           CFStringRef *deviceName)
{
    UInt32 thePropSize = sizeof(CFStringRef);
    AudioObjectPropertyAddress thePropertyAddress =
    { kAudioObjectPropertyName,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMaster };
    
    OSStatus errorCode =
    AudioObjectGetPropertyData(deviceID,
                               &thePropertyAddress,
                               0,
                               NULL,
                               &thePropSize,
                               deviceName);
    
    if (errorCode) {
        printf("Error in GetAudioDeviceName: %d\n", errorCode);
    }
    
    return errorCode == 0;
}

Boolean GetMuteOnInputAudioDevice(const AudioDeviceID inputDeviceID,
                                  Boolean *isMuted)
{
    AudioObjectPropertyAddress thePropertyAddress =
    { kAudioDevicePropertyMute,
        kAudioDevicePropertyScopeInput,
        kAudioObjectPropertyElementMaster };
    
    UInt32 mute = 0;
    UInt32 thePropSize = sizeof(mute);
    
    OSStatus errorCode;
    if (AudioObjectHasProperty(inputDeviceID, &thePropertyAddress)) {
        errorCode = AudioObjectGetPropertyData(inputDeviceID,
                                               &thePropertyAddress,
                                               0,
                                               NULL,
                                               &thePropSize,
                                               &mute);
        if (errorCode) {
            printf("Error in GetMuteOnInputAudioDevice: %d\n", errorCode);
        }
        
        *isMuted = mute > 0;
        
        return errorCode == 0;
    } else {
        printf("Error in GetMuteOnInputAudioDevice: mute not supported\n");
        
        return false;
    }
}

Boolean SetMuteOnInputAudioDevice(const AudioDeviceID inputDeviceID,
                                  const Boolean mute)
{
    AudioObjectPropertyAddress thePropertyAddress =
    { kAudioDevicePropertyMute,
        kAudioDevicePropertyScopeInput,
        kAudioObjectPropertyElementMaster };
    
    UInt32 theMute = mute;
    UInt32 thePropSize = sizeof(theMute);
    
    
    const char *inputDeviceString;
    CFStringRef inputDeviceName;
    if (GetAudioDeviceName(inputDeviceID, &inputDeviceName)) {
        inputDeviceString =
        CFStringGetCStringPtr(inputDeviceName, CFStringGetSystemEncoding());
        CFRelease(inputDeviceName);
    } else {
        inputDeviceString = "<Unamed device>";
    }
    
    Boolean setMute = true;
    
    if (AudioObjectHasProperty(inputDeviceID, &thePropertyAddress)) {
        printf("\tSetting %s mute %s\n", inputDeviceString, (theMute) ? "on" : "off");
        OSStatus errorCode = AudioObjectSetPropertyData(inputDeviceID,
                                                        &thePropertyAddress,
                                                        0,
                                                        NULL,
                                                        thePropSize,
                                                        &theMute);
        if (errorCode) {
            printf("Error in SetMuteOnInputAudioDevice: %d\n", errorCode);
            setMute = false;
        }
    } else {
        printf("Error in SetMuteOnInputAudioDevice: mute not supported\n");
        setMute = false;
    }
    
    return setMute;
}

Boolean IsHardwareMuted()
{
    AudioDeviceID theDefaultInputDeviceID;
    
    Boolean isMuted = false;
    
    if (GetDefaultInputAudioDevice(&theDefaultInputDeviceID)) {
        GetMuteOnInputAudioDevice(theDefaultInputDeviceID, &isMuted);
    }
    
    return isMuted;
}

void SetHardwareMute(Boolean theMute)
{
    AudioDeviceID theDefaultInputDeviceID;
    if (GetDefaultInputAudioDevice(&theDefaultInputDeviceID)) {
        SetMuteOnInputAudioDevice(theDefaultInputDeviceID, theMute);
    }
}