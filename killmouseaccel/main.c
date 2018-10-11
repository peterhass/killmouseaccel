//
//  main.c
//  killmouseaccel
//
//  Created by Christian Höltje on 9/20/11.
//  This is public domain. The code was originally taken from
//  http://forums3.armagetronad.net/viewtopic.php?t=3364
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <IOKit/hidsystem/IOHIDLib.h>
#include <IOKit/hidsystem/IOHIDParameter.h>
#include <IOKit/hidsystem/event_status_driver.h>

void usage(char* app_name) {
  fprintf(stdout, "USAGE: %s [accel] [mouse/trackpad] [trackpad/mouse]\nacc: Use DISABLE to disable accel\n", app_name);
}

void print_current(int argc, char** argv) 
{
  io_connect_t handle = NXOpenEventStatus();

  for(int i = 2; i < argc; i++) {
    CFStringRef type = 0;
    
    if(strcmp(argv[i], "mouse") == 0)
        type = CFSTR(kIOHIDMouseAccelerationType);
    else if(strcmp(argv[i], "trackpad") == 0)
        type = CFSTR(kIOHIDTrackpadAccelerationType);
    
    int32_t accel = 0;
    IOByteCount accel_size;
    if(type) {
      if(IOHIDGetParameter(handle, type, sizeof accel, &accel, &accel_size) != KERN_SUCCESS) {
        fprintf(stderr, "Failed to get %s accel\n", argv[i]);
      } else {
        fprintf(stdout, "type %s was set to acc %d\n", argv[i], accel);
      }

    }
  }
}

int main(int argc, char **argv)
{
    if(argc < 3) {
        usage(argv[0]);
        return 1;
    }

    int32_t accel;
    puts(argv[1]);
    if(strcmp(argv[1], "disable") == 0)
      accel = -0x10000;
    else
      accel = atoi(argv[1]);
    printf("accel = %d\n", accel);
    print_current(argc, argv);

    io_connect_t handle = NXOpenEventStatus();
    if(handle) {
        int i;
        for(i=2; i<argc; i++) {
            CFStringRef type = 0;
            
            if(strcmp(argv[i], "mouse") == 0)
                type = CFSTR(kIOHIDMouseAccelerationType);
            else if(strcmp(argv[i], "trackpad") == 0)
                type = CFSTR(kIOHIDTrackpadAccelerationType);
            if(type && IOHIDSetParameter(handle, type, &accel, sizeof accel) != KERN_SUCCESS)
                fprintf(stderr, "Failed to kill %s accel\n", argv[i]);
        }
        NXCloseEventStatus(handle);
    } else
        fprintf(stderr, "No handle\n");
    
    return 0;
}
