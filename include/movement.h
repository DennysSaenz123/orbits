#include <bn_sprite_ptr.h>
#include <bn_fixed.h>

/**
 * Moves sprite up/down/left/right based on the d-pad being held.
 * 
 * Dimensions are moved independently, meaning that diagonal motion is faster than moving in one
 * dimension.
 * 
 * @param sprite the sprite to move
 * @param speed the speed at which to move the sprite in each dimension
 */
void dPadMoveSprite(bn::sprite_ptr& sprite, bn::fixed speed);