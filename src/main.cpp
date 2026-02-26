#include <bn_core.h>
#include <bn_backdrop.h>
#include <bn_color.h>
#include <bn_keypad.h>
#include <bn_fixed_point.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

#include "bn_sprite_items_dot.h"
#include "bn_sprite_items_square.h"

// A scaling factor by which to reduce the force applied when orbiting
// Important for numerical stability
static constexpr bn::fixed FORCE_SCALE = 10;

static constexpr int MAX_ORBITERS = 30;

static constexpr bn::fixed_point ORBITER_START_POSIITON = {0, 0};
static constexpr bn::fixed_point ORBITER_START_VELOCITY = {0, 5};


class Center {
    public:
        Center(bn::fixed_point starting_position, bn::fixed mass, bn::fixed speed) : 
            _sprite(bn::sprite_items::square.create_sprite(starting_position)),
            _mass(mass),
            _speed(speed) {
        }

        void update() {
            bn::fixed dx = 0;
            bn::fixed dy = 0;
            if(bn::keypad::left_held()) {
                dx -= _speed;
            }
            if(bn::keypad::right_held()) {
                dx += _speed;
            }
            if(bn::keypad::up_held()) {
                dy -= _speed;
            }
            if(bn::keypad::down_held()) {
                dy += _speed;
            }

            _sprite.set_position(_sprite.position() + bn::fixed_point(dx, dy));
        }

        bn::fixed_point position() {
            return _sprite.position();
        }

        bn::fixed mass() {
            return _mass;
        }

    private:
        bn::sprite_ptr _sprite;
        bn::fixed _mass;
        bn::fixed _speed;
};

class Orbiter {

    public:
        Orbiter(bn::fixed_point starting_location, bn::fixed_point starting_velocity) :
        _sprite(bn::sprite_items::dot.create_sprite(starting_location)),
        _velocity(starting_velocity) {
        }

        void update(Center& center) {
            bn::fixed_point delta = center.position() - _sprite.position();
            bn::fixed_point force = delta * center.mass();
            _velocity += force / FORCE_SCALE;
            _sprite.set_position(_sprite.position() + _velocity);
        }

    private:
        bn::sprite_ptr _sprite;
        bn::fixed_point _velocity;
};

int main() {
    bn::core::init();
    Center center = Center({30, 40}, .05, 2);

    bn::vector<Orbiter, MAX_ORBITERS> orbiters = {};
    

    while(true) {
        if(bn::keypad::a_pressed()) {
            if(orbiters.size() < MAX_ORBITERS) {
                orbiters.push_back(Orbiter(ORBITER_START_POSIITON, ORBITER_START_VELOCITY));
            }
        }

        center.update();

        for(Orbiter& orbiter : orbiters) {
            orbiter.update(center);
        }
        bn::core::update();
    }
}