//
//  AppDelegate.m
//  MuteUnmuteMic
//
//  Copyright © 2015 CocoaHeads Brasil. All rights reserved.
//

#import "AppDelegate.h"
#import "AudioMixer.h"

static NSInteger const kDefaultVolume = 70;

@interface AppDelegate ()

@property (nonatomic) NSStatusItem *menuItem;
@property (nonatomic) BOOL muted;
@property (nonatomic) NSInteger inputVolumeToUnmute;

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    [self initDefaults];
    [self configureStatusBar];
    [self updateInputVolume];
}

- (void)initDefaults
{
    _muted = IsHardwareMuted();
    _inputVolumeToUnmute = kDefaultVolume;
}

- (void)configureStatusBar
{
    NSStatusBar *statusBar = [NSStatusBar systemStatusBar];
    
    NSStatusItem *menuItem =
    [statusBar statusItemWithLength:NSVariableStatusItemLength];
    [menuItem setToolTip:@"[Un]MuteMic by CocoaHeads Brazil"];
    [menuItem setImage:[NSImage imageNamed:@"mic_on"]];
    [menuItem setHighlightMode:YES];

    [menuItem setTarget:self];
    [menuItem setAction:@selector(menuItemClicked:)];
    [menuItem.button sendActionOn:NSLeftMouseUpMask|NSRightMouseUpMask];
    
    self.menuItem = menuItem;
}

- (void)menuItemClicked:(id)sender
{
    NSEvent *event = [[NSApplication sharedApplication] currentEvent];
    
    if ((event.modifierFlags & NSControlKeyMask) || (event.type == NSRightMouseUp)) {
        [self showMenu];
    } else {
        [self toggleMute];
    }
}

- (void)toggleMute
{
    self.muted = !self.muted;
    [self updateInputVolume];
}

- (void)updateInputVolume
{
    BOOL muted = self.muted;
    
    NSInteger volume;
    NSString *imageName;
    if (muted) {
        volume = 0;
        imageName = @"mic_off";
    } else {
        volume = self.inputVolumeToUnmute;
        imageName = @"mic_on";
    }
    
    // set volume
    NSString *source =
    [NSString stringWithFormat:@"set volume input volume %ld", (long)volume];
    NSAppleScript *script = [[NSAppleScript alloc] initWithSource:source];
    NSDictionary *errorInfo = nil;
    [script executeAndReturnError:&errorInfo];
    
    if (errorInfo) {
        NSLog(@"Error on script %@", errorInfo);
    }
    
    // set hardware mute
    SetHardwareMute(muted);
    
    // set image
    self.menuItem.image = [NSImage imageNamed:imageName];
}

- (void)showMenu
{
    [self.menuItem popUpStatusItemMenu:self.menu];
}

- (IBAction)didSetVolumeInput:(NSMenuItem *)sender
{
    for (NSMenuItem *item in sender.menu.itemArray) {
        item.state = 0;
    }
    sender.state = 1;
    
    self.inputVolumeToUnmute = [sender.title integerValue];
    [self updateInputVolume];
}

@end
