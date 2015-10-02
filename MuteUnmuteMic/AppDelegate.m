#import "AppDelegate.h"

#define MAX_VOLUME 70

@interface AppDelegate ()
{
    NSStatusItem *menuItem;
    BOOL muted;
    int inputVolumeToUnmute;
}

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    [self initDefaults];
    [self configureStatusBar];
}

- (void)initDefaults {
    muted = NO;
    inputVolumeToUnmute = MAX_VOLUME;
}

- (void)configureStatusBar {
    NSStatusBar *statusBar = [NSStatusBar systemStatusBar];
    
    menuItem = [statusBar statusItemWithLength:NSVariableStatusItemLength];
    [menuItem setToolTip:@"MuteUnmuteMic by CocoaHeads Brazil"];
    [menuItem setImage:[NSImage imageNamed:@"mic_on"]];
    [menuItem setHighlightMode:YES];

    [menuItem setTarget:self];
    [menuItem setAction:@selector(menuItemClicked:)];
    [menuItem.button sendActionOn:NSLeftMouseUpMask|NSRightMouseUpMask];
}

- (void)menuItemClicked:(id)sender {
    NSEvent *event = [[NSApplication sharedApplication] currentEvent];
    
    if ((event.modifierFlags & NSControlKeyMask) || (event.type == NSRightMouseUp)) {
        [self showMenu];
    } else {
        [self toggleMute];
    }

}

- (void)toggleMute {
    muted = !muted;
    [self updateInputVolume];
}

- (void)updateInputVolume {
    int volume = muted ? 0 : inputVolumeToUnmute;
    NSString *source = [NSString stringWithFormat:@"set volume input volume %d", volume];
    NSAppleScript *script = [[NSAppleScript alloc] initWithSource:source];
    
    [script executeAndReturnError:nil];
    
    NSString *imageName = muted ? @"mic_off" : @"mic_on";
    menuItem.image = [NSImage imageNamed:imageName];

}

- (void)showMenu {
    [menuItem popUpStatusItemMenu:self.menu];
}


- (IBAction)didSetVolumeInput:(NSMenuItem *)sender {
    
    for (NSMenuItem *item in sender.menu.itemArray) {
        item.state = 0;
    }
    sender.state = 1;
    
    inputVolumeToUnmute = [sender.title intValue];
    [self updateInputVolume];
}

@end
