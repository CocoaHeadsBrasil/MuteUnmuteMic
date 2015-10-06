//
//  AudioMixer.h
//  MuteUnmuteMic
//
//  Created by Diogo Tridapalli on 10/5/15.
//  Copyright © 2015 Gustavo Barbosa. All rights reserved.
//

#ifndef AudioMixer_h
#define AudioMixer_h

#import <CoreAudio/CoreAudio.h>
/**
 *  Get device id for the default input device
 *
 *  @param defaultInputDeviceID AudioDeviceID pointer
 *
 *  @return @a true if success @a false otherwise
 */
extern Boolean GetDefaultInputAudioDevice(AudioDeviceID *defaultInputDeviceID);

/**
 *  Get the name of an audio device
 *
 *  @param deviceID               AudioDeviceID
 *  @param deviceName CFStringRef reference, must be release after use
 *
 *  @return @a true if success @a false otherwise
 */
extern Boolean GetAudioDeviceName(const AudioDeviceID deviceID,
                                  CFStringRef *deviceName);

/**
 *  Get mute state on input device
 *
 *  @param inputDeviceID AudioDeviceID
 *  @param isMuted       @a true if device is muted @a false otherwise
 *
 *  @return @a true if success @a false otherwise
 */
extern Boolean GetMuteOnInputAudioDevice(const AudioDeviceID inputDeviceID,
                                         Boolean *isMuted);

/**
 *  Mute or unmute an inpute device
 *
 *  @param inputDeviceID AudioDeviceID
 *  @param mute          @a true if device should be muted @a false otherwise
 *
 *  @return @a true if success @a false otherwise
 */
extern Boolean SetMuteOnInputAudioDevice(const AudioDeviceID inputDeviceID,
                                         const Boolean mute);

/**
 *  Get the mute state of default input via HAL
 *
 *  @return @a true if is muted @a false otherwise
 */
extern Boolean IsHardwareMuted();

/**
 *  Set the mute state of default input via HAL
 *
 *  @param theMute @a true if should be muted @a false otherwise
 */
extern void SetHardwareMute(Boolean theMute);

#endif /* AudioMixer_h */
