#include "LeakDetector.h"
#include "LeakDetectorBridge.h"

extern "C" {

void LeakDetector_Allocated(size_t bytes) {
    LeakDetectors::HeapRTOS().allocated(bytes);
}

void LeakDetector_Freed(size_t bytes) {
    LeakDetectors::HeapRTOS().freed(bytes);
}

}
