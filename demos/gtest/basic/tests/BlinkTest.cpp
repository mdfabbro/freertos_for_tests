#include <gtest/gtest.h>
#include "tasks/Blink.h"  // si querés probar esta clase

TEST(BlinkTaskTest, Creation) {
    BlinkTask blink((tskIDLE_PRIORITY + 1), "Blink");
    EXPECT_TRUE(blink.isValid());
}
