/* AI component with different types */

#include "transform.hpp"

enum AIType {
    Follower
}


struct AIComponent {
    AIType type;

    /* Data depending on the AIType */
    union {
        struct {
            Vec2 target;
        } followerData;
    }
}
