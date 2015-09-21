#import "AppDelegate.h"

#define MAX_VOLUME 75

@interface AppDelegate ()
{
    NSStatusItem *menuItem;
    BOOL muted;
}

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    muted = NO;
    
    menuItem = [[NSStatusBar systemStatusBar] statusItemWithLength:NSVariableStatusItemLength];
    menuItem.highlightMode = YES;
    menuItem.image = [NSImage imageNamed:@"mic_on"];
    [menuItem setTarget:self];
    [menuItem setAction:@selector(toggleMute)];
}

- (void)toggleMute {
    muted = !muted;
    
    int volume = muted ? 0 : MAX_VOLUME;
    NSString *source = [NSString stringWithFormat:@"set volume input volume %d", volume];
    NSAppleScript *script = [[NSAppleScript alloc] initWithSource:source];
    
    [script executeAndReturnError:nil];
    
    NSString *imageName = muted ? @"mic_off" : @"mic_on";
    menuItem.image = [NSImage imageNamed:imageName];
}

@end
