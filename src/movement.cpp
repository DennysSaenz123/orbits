#include "movement.h"
#include <bn_keypad.h>

void dPadMoveSprite(bn::sprite_ptr& sprite, bn::fixed speed) {
    bn::fixed dx = 0;
    bn::fixed dy = 0;

    if(bn::keypad::left_held()) {
        dx -= speed;
    }
    if(bn::keypad::right_held()) {
        dx += speed;
    }
    if(bn::keypad::up_held()) {
        dy -= speed;
    }
    if(bn::keypad::down_held()) {
        dy += speed;
    }

    sprite.set_position(sprite.position() + bn::fixed_point(dx, dy));
}
