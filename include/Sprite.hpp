/**
 * @file Sprite.hpp
 * @author Julie Fiadino
 * @copyright Copyright 2021 (C) Julie Fiadino
 * 
 * This file defines the Sprite class and it's inherited one
 * Those are objects drawn on screen by the Renderer class
 */

#pragma once
#include <vector>
#include "Vector.hpp"
#include "FileReader.hpp"
#include "Animation.hpp"

class Renderer;

//==============================================================================
// Defines an collection of sprites
struct SpriteSheet
{
    Bitmap sheet;                       // source image
    int sWidth, sHeight;    // dimensions of each frame
    std::vector<int> animationLength;  // length of each animations
};

// Rectangle
struct Rect
{
    Vector2i pos;                       // rectangle position
    Vector2i hSize;                     // half size
    unsigned int color;                 // color
};

//==============================================================================
// Sprite class
class Sprite
{
protected:
    Vector2i _position;                 // sprite position
    Vector2i _size;                     // image dimensions
    float _scale;                       // scale factor
    Bitmap _img;                        // source image
    bool _reversed = false;             // tells if image needs to be reversed when rendered
    int _centerOffset = 0;              // y offset added when calculating drawing order

public:
    /**
     * Constructor
     * Takes a position, a scale factor and a source image
     * Can also take y center offset and if the image needs to be reversed as parameters
     */
    Sprite(Vector2i pos, float scale, Bitmap image, bool reversed=false, int offset=0);
    ~Sprite();

    /* Draws the sprite on screen */
    virtual void Draw(Renderer *r);
    /**
     * Used by the AnimatedSprite child
     * Empty function
     */
    virtual void IncrementFrameTime(float dt);
    
    /* Returns sprite position */
    Vector2i GetPosition() const;
    /* Returns scale factor */
    float GetScale() const;
    /* Returns copy of source image */
    Bitmap GetImage() const;
    /* Returns image dimensions */
    Vector2i GetImageSize() const;
    /* Returns if image needs to be reversed */
    bool IsReversed() const;
    /* Get sprite's starting y position. Used to calculate drawing order */
    int GetPosOnGround() const;

    /* Sets sprite position */
    void SetPosition(Vector2i pos);
    /* Sets a new source image */
    void SetImage(Bitmap img);
    /* Tells if images needs to be reversed */
    void SetReverse(bool reversed);
    /* Sets image scale factor */
    void SetScale(float scale);
    /* Sets image offset in source */
    void SetImageIndex(int index);
    
    bool lastLayer = false;            // tells if the sprite is on the last layer. 
                                       // used when calculating drawing order
};

//==============================================================================
// AnimatedSprite class
class AnimatedSprite : public Sprite
{
private:
    std::vector<Animation> _animations;  // animations
    int _spriteIndex = 0;               // current animation's frame index
    int _animationIndex = 0;            // animation index

    float _frameTime;                   // animation's frame time
    float _currentFrameTime = 0;        // elapsed time since last frame 

    /* Adds tick time to the animation's frame time */
    void IncrementFrameTime(float dt) override;
    Animation GetCurrentAnimation();

public:
    /**
     * Constructor
     * Takes a position, a scale factor and a vector of animations
     * Can also take if the image is reversed as parameter
     */
    AnimatedSprite::AnimatedSprite(Vector2i pos, float scale, std::vector<Animation> anim, bool reversed=false); 

    /* Draws the sprite on screen */
    void Draw(Renderer *r) override;

    /* Returns the animation index */
    int GetAnimationIndex() const;
    /* Returns the current animation's sprite index */
    int GetCurrentSpriteIndex() const;

    /* Sets the current animation's sprite index */
    void SetImageIndex(int index);
    /* Sets the sprite position */
    void SetPosition(Vector2i pos);
};

//==============================================================================
// Tilemap class
class Tilemap : public Sprite
{
private:
    SpriteSheet _sprites;               // map sprite sheet
    std::vector<int> _map;              // used to represent the map. an int represent the sprite index in the sheet
    Vector2i _mapSize;                  // map's dimensions

public:
    /**
     * Constructor
     * Takes a position, a scale factor, a sprite sheet and an int map and map dimensions
     * Can also take if the image is reversed as parameter
     */
    Tilemap(Vector2i pos, float scale, SpriteSheet sprites, std::vector<int> map, Vector2i mapSize);

    /* Draws the sprite on screen */
    void Draw(Renderer *r) override;

    /* Returns a copy of the sprite sheet */
    SpriteSheet GetSpriteSheet() const;

    /* Sets the map center */
    void SetPosition(Vector2i pos);
    /* Sets a new int map */
    void SetMap(std::vector<int> map);
    /* Sets a new sprite sheet */
    void SetSpriteSheet(SpriteSheet sheet);
};