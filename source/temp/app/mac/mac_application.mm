#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>

#include <future>
#include <thread>
#include <utility>

#include "temp/app/mac/mac_application.h"
#include "temp/core/logger.h"

@interface ApplicationDelegate : NSObject < NSApplicationDelegate > {
}
@end

@implementation ApplicationDelegate
- (void)applicationWillFinishLaunching:(NSNotification *)notification {
    [[NSApp keyWindow] makeKeyAndOrderFront:NSApp];
    [[NSApp keyWindow] orderFrontRegardless];
}
@end

@interface WindowDelegate : NSObject<NSWindowDelegate>
@property (assign) NSWindow* window;
- (void)windowWillClose:(NSNotification*)notification;
- (void)windowDidResize:(NSNotification*)notification;
@end

@implementation WindowDelegate

temp::app::OnResizeWindow* on_resize_window_;

- (id)init {
    if (self = [super init]) {
        on_resize_window_ = nullptr;
    }
    return self;
}

- (void)setOnResizeWindow: (temp::app::OnResizeWindow*)on_resize_window {
    on_resize_window_ = on_resize_window;
}

- (void)windowWillClose:(NSNotification*)notification {
    [NSApp stop:(NSApp)];
}

- (void)windowDidResize:(NSNotification *)notification {
    if(on_resize_window_ != nullptr) {
        auto rect = _window.contentView.frame;
        (*on_resize_window_)((std::uint32_t)rect.size.width, (std::uint32_t)rect.size.height);
    }
}

@end

namespace temp {
namespace app {
namespace mac {
    
struct MacApplication::Properties {
    NSApplication *app = nil;
    ApplicationDelegate *delegate = nil;

    std::atomic_char exit_flag;

    NSWindow *window = nil;
    NSView *view = nil;
};

MacApplication::MacApplication()
    : properties_(new Properties()) {
    properties_->app = [NSApplication sharedApplication];
    properties_->delegate = [[ApplicationDelegate alloc] init];
    [properties_->app setDelegate:properties_->delegate];
    [properties_->app setActivationPolicy:NSApplicationActivationPolicyRegular];

    properties_->exit_flag = 0;

    properties_->window = [[NSWindow alloc]
        initWithContentRect:NSMakeRect(0, 0, 1080, 720)
                    styleMask:NSWindowStyleMaskTitled
                            | NSWindowStyleMaskMiniaturizable
                            | NSWindowStyleMaskClosable
                            | NSWindowStyleMaskResizable
                    backing:NSBackingStoreBuffered
                        defer:NO];
    auto delegate = [[WindowDelegate alloc] init];
    delegate.window = properties_->window;
    [delegate setOnResizeWindow:&on_resize_window()];
    [properties_->window setTitle:@"てんぷら"];
    [properties_->window center];
    [properties_->window setDelegate:delegate];
    [properties_->window makeKeyWindow];
    NSWindowCollectionBehavior behavior = [properties_->window collectionBehavior];
    behavior |= NSWindowCollectionBehaviorManaged
                | NSWindowCollectionBehaviorParticipatesInCycle
                | NSWindowCollectionBehaviorFullScreenPrimary;
    [properties_->window setCollectionBehavior:behavior];
    [properties_->window orderFrontRegardless];

    properties_->view = [properties_->window contentView];
        
    CAMetalLayer* metal_layer = [CAMetalLayer layer];
    [properties_->view setLayer:metal_layer];

    native_window_handle_ = properties_->view;

    TEMP_LOG_TRACE("Constructed MacApplication.");
}

MacApplication::~MacApplication() { [properties_->app terminate:properties_->app];

    TEMP_LOG_TRACE("Destructed MacApplication.");
 }

std::int32_t MacApplication::Run() {
    TEMP_LOG_TRACE("MacApplication begin running");
    on_initialize()();

    std::packaged_task<void()> task([this](){MainLoop();});
    auto future = task.get_future();
    std::thread main_thread(std::move(task));

    [properties_->app run];
    properties_->exit_flag = 1;
    future.wait();
    main_thread.join();

    on_terminate()();

    TEMP_LOG_TRACE("MacApplication end running");
    return 0;
}

void MacApplication::Exit() {
    properties_->exit_flag = 1;
    [properties_->app stop:properties_->app];
}


void MacApplication::MainLoop() {
    while (!properties_->exit_flag) {
        on_update()();
    }
}
    
}
}
}
