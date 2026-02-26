#include "Center.h"

bn::fixed_point Center::position() {
    return _sprite.position();
}
bn::fixed Center::mass() {
    return _mass;
}
void Center::update() {
    dPadMoveSprite(_sprite, _speed);
}

Center::Center(bn::fixed_point starting_position, bn::fixed mass, bn::fixed speed) : 
    _sprite(bn::sprite_items::square.create_sprite(starting_position)),
    _mass(mass),
    _speed(speed) {
    }
