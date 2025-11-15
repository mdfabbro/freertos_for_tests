#ifndef __TASKS_FILTER_H__
#define __TASKS_FILTER_H__

#include "TaskCpp.h"
#include <stdio.h>
#include "Queue/Vector.h"

template<size_t Size>
class Filter : public FreeRTOScpp::TaskClass {
private:
    size_t pushed {0};
    Vector<int16_t,Size> &buffer;
    size_t blocks {0};

    int FilterLength; // 

public:
    Filter(Vector<int16_t,Size> &buffer, int FilterLength = 24)
    : TaskClass("Filter", TaskPrio_Low, 128) , buffer(buffer) , FilterLength(FilterLength) {
    }

    ~Filter() = default;

    void task() override {
        for (;;) {
            spin();
        }
    }

    void spin() {
        uint32_t block ;
        if( wait( 0 , 0, &block ) ) {
            const auto startIndex = block * (Size/4);
            printf("[%8lu] ", blocks);
            for( size_t i = 0 ; i < (Size/4); ++i ) {
                printf("%8d ", movingAverageFilter(i + startIndex));
            }
            printf("\n");
            ++blocks;
        } else {
            printf("[ADCDeferred] [ERROR] Timeout!\n");
        }
    }

    int16_t movingAverageFilter( size_t index ) {
        int32_t accum {0} ; // With N = 8 and 12 bits it will not overflow...
        const auto circularIndex = [] ( int i ) -> size_t {
            if(i < 0) {
                return Size - static_cast<size_t>(-i);
            }
            return static_cast<size_t>(i);
        };
        for( int i = -(FilterLength-1); i <= 0; ++i ) {
            accum += static_cast<int32_t>(*buffer[circularIndex(i + static_cast<int>(index))]) ;
        }
        return static_cast<int16_t>(accum / FilterLength);
    }
};

#endif // __TASKS_DEFERRED_H__
