#include <bn_core.h>
#include <bn_backdrop.h>
#include <bn_color.h>
#include <bn_keypad.h>
#include <bn_fixed_point.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

#include "bn_sprite_items_dot.h"
#include "bn_sprite_items_square.h"
#include "movement.h"
#include "Center.h"

// A scaling factor by which to reduce the force applied when orbiting
// Important for numerical stability
static constexpr bn::fixed FORCE_SCALE = 10;

// Maximum number of orbiters allowed on the screen
static constexpr int MAX_ORBITERS = 30;

// Default starting posiiton and velocity for Orbiter
static constexpr bn::fixed_point ORBITER_START_POSIITON = {0, 0};
static constexpr bn::fixed_point ORBITER_START_VELOCITY = {0, 5};

/**
 * An object that orbits around a center. Orbits using Hooke's law, as if attached by a 2D spring.
 * The stiffness of the spring in each dimension is center.mass() / FORCE_SCALE. 
 */
class Orbiter {
    public:
        /**
         * Creates an orbiter.
         * 
         * @param starting_location the initial location of the Orbiter
         * @param starting_velocity the initial velocity of the Orbiter, {dx, dy}
         * @param center the center to orbit around
         */
        Orbiter(bn::fixed_point starting_location, bn::fixed_point starting_velocity, Center &center) :
        _sprite(bn::sprite_items::dot.create_sprite(starting_location)),
        _velocity(starting_velocity),
        _center(center) {
        }

        /**
         * Take a step orbiting around the center.
         */
        void update() {
            // A vector from the Orbiter to the Center
            bn::fixed_point delta = _center.position() - _sprite.position();

            // Scale the vector by the mass and apply a FORCE_SCALE
            // This is done in two steps instead of having a pre-scaled mass to prevent loss of
            // fixed point precision
            bn::fixed_point force = delta * _center.mass();
            force /= FORCE_SCALE;

            // Update the current velocity with the force
            _velocity += force;

            // Update the position by taking a step by the velocity vector
            _sprite.set_position(_sprite.position() + _velocity);
        }

    private:
        bn::sprite_ptr _sprite;
        bn::fixed_point _velocity;
        Center& _center;
};

int main() {
    bn::core::init();

    Center center = Center({30, 40}, .05, 2);
    bn::vector<Orbiter, MAX_ORBITERS> orbiters = {};
    
    while(true) {
        // Add an orbiter when A is pressed if there's room
        if(bn::keypad::a_pressed()) {
            if(orbiters.size() < MAX_ORBITERS) {
                orbiters.push_back(Orbiter(ORBITER_START_POSIITON, ORBITER_START_VELOCITY, center));
            }
        }

        // Remove an orbiter when B is pressed if there's at least one
        if (bn::keypad::b_pressed()) {
            if(orbiters.size() > 0) {
                orbiters.pop_back();
            }
        }

        center.update();
        for(Orbiter& orbiter : orbiters) {
            orbiter.update();
        }

        bn::core::update();
    }
}