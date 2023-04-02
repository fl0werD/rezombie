#pragma once

/**
 * You have to call MessageBegin before call these functions
 */

#include <cssdk/public/utils.h>
#include <metamod/engine.h>

namespace message {
    using namespace cssdk;
    using namespace metamod::engine;

    inline auto TE_BeamPoints(
        Vector& start, Vector& end, int spriteIndex, byte startingFrame, byte frameRate, byte life, byte lineWidth,
        int noiseAmplitude, byte color[3], byte brightness, byte scrollSpeed
    ) -> void {
        WriteByte(TE_BEAM_POINTS);
        WriteCoord(start.x);       // startposition.x
        WriteCoord(start.y);       // startposition.y
        WriteCoord(start.z);       // startposition.z
        WriteCoord(end.x);         // endposition.x
        WriteCoord(end.y);         // endposition.y
        WriteCoord(end.z);         // endposition.z
        WriteShort(spriteIndex);   // sprite index
        WriteByte(startingFrame);  // starting frame
        WriteByte(frameRate);      // frame rate in 0.1's
        WriteByte(life);           // life in 0.1's
        WriteByte(lineWidth);      // line width in 0.1's
        WriteByte(noiseAmplitude); // noise amplitude in 0.01's
        WriteByte(color[0]);       // red
        WriteByte(color[1]);       // green
        WriteByte(color[2]);       // blue
        WriteByte(brightness);     // brightness
        WriteByte(scrollSpeed);    // scroll speed in 0.1's
        MessageEnd();
    }

    inline auto TE_BeamEntityPoint(
        int startEntity, Vector& end, int spriteIndex, byte startingFrame, byte frameRate, byte life, byte lineWidth,
        byte noiseAmplitude, byte color[3], byte brightness, byte scrollSpeed
    ) -> void {
        WriteByte(TE_BEAM_ENT_POINT);
        WriteShort(startEntity);   // start entity
        WriteCoord(end.x);         // endposition.x
        WriteCoord(end.y);         // endposition.y
        WriteCoord(end.z);         // endposition.z
        WriteShort(spriteIndex);   // sprite index
        WriteByte(startingFrame);  // starting frame
        WriteByte(frameRate);      // frame rate in 0.1's
        WriteByte(life);           // life in 0.1's
        WriteByte(lineWidth);      // line width in 0.1's
        WriteByte(noiseAmplitude); // noise amplitude in 0.01's
        WriteByte(color[0]);       // red
        WriteByte(color[1]);       // green
        WriteByte(color[2]);       // blue
        WriteByte(brightness);     // brightness
        WriteByte(scrollSpeed);    // scroll speed in 0.1's
        MessageEnd();
    }

    inline auto TE_GunShot(Vector& position) -> void {
        WriteByte(TE_GUNSHOT);
        WriteCoord(position.x); // position.x
        WriteCoord(position.y); // position.y
        WriteCoord(position.z); // position.z
        MessageEnd();
    }

    inline auto TE_Explosion(Vector& position, int spriteIndex, byte scale, byte frameRate, byte flags) -> void {
        WriteByte(TE_EXPLOSION);
        WriteCoord(position.x);  // position.x
        WriteCoord(position.y);  // position.y
        WriteCoord(position.z);  // position.z
        WriteShort(spriteIndex); // sprite index
        WriteByte(scale);        // scale in 0.1's
        WriteByte(frameRate);    // framerate
        WriteByte(flags);        // flags
        MessageEnd();
    }

    inline auto TE_TarExplosion(Vector& position) -> void {
        WriteByte(TE_TAR_EXPLOSION);
        WriteCoord(position.x); // position.x
        WriteCoord(position.y); // position.y
        WriteCoord(position.z); // position.z
        MessageEnd();
    }

    inline auto TE_Smoke(Vector& position, int spriteIndex, byte scale, byte frameRate) -> void {
        WriteByte(TE_SMOKE);
        WriteCoord(position.x);  // position.x
        WriteCoord(position.y);  // position.y
        WriteCoord(position.z);  // position.z
        WriteShort(spriteIndex); // sprite index
        WriteByte(scale);        // scale in 0.1's
        WriteByte(frameRate);    // framerate
        MessageEnd();
    }

    inline auto TE_Tracer(Vector& start, Vector& end) -> void {
        WriteByte(TE_TRACER);
        WriteCoord(start.x); // startposition.x
        WriteCoord(start.y); // startposition.y
        WriteCoord(start.z); // startposition.z
        WriteCoord(end.x);   // endposition.x
        WriteCoord(end.y);   // endposition.y
        WriteCoord(end.z);   // endposition.z
        MessageEnd();
    }

    inline auto
    TE_Lighting(Vector& start, Vector& end, byte life, byte width, byte amplitude, int spriteIndex) -> void {
        WriteByte(TE_LIGHTNING);
        WriteCoord(start.x);     // startposition.x
        WriteCoord(start.y);     // startposition.y
        WriteCoord(start.z);     // startposition.z
        WriteCoord(end.x);       // endposition.x
        WriteCoord(end.y);       // endposition.y
        WriteCoord(end.z);       // endposition.z
        WriteByte(life);         // life in 0.1's
        WriteByte(width);        // width in 0.1's
        WriteByte(amplitude);    // amplitude in 0.01's
        WriteShort(spriteIndex); // sprite model index
        MessageEnd();
    }

    inline auto TE_BeamEntities(
        int startEntity, int endEntity, int spriteIndex, byte startingFrame, byte frameRate, byte life, byte lineWidth,
        byte noiseAmplitude, byte color[3], byte brightness, byte scrollSpeed
    ) -> void {
        WriteByte(TE_BEAM_ENTITIES);
        WriteShort(startEntity);   // start entity
        WriteShort(endEntity);     // end entity
        WriteShort(spriteIndex);   // sprite index
        WriteByte(startingFrame);  // starting frame
        WriteByte(frameRate);      // frame rate in 0.1's
        WriteByte(life);           // life in 0.1's
        WriteByte(lineWidth);      // line width in 0.1's
        WriteByte(noiseAmplitude); // noise amplitude in 0.01's
        WriteByte(color[0]);       // red
        WriteByte(color[1]);       // green
        WriteByte(color[2]);       // blue
        WriteByte(brightness);     // brightness
        WriteByte(scrollSpeed);    // scroll speed in 0.1's
        MessageEnd();
    }

    inline auto TE_Sparks(Vector& position) -> void {
        WriteByte(TE_SPARKS);
        WriteCoord(position.x); // position.x
        WriteCoord(position.y); // position.y
        WriteCoord(position.z); // position.z
        MessageEnd();
    }

    inline auto TE_LavaSplash(Vector& position) -> void {
        WriteByte(TE_LAVA_SPLASH);
        WriteCoord(position.x); // position.x
        WriteCoord(position.y); // position.y
        WriteCoord(position.z); // position.z
        MessageEnd();
    }

    inline auto TE_Teleport(Vector& position) -> void {
        WriteByte(TE_TELEPORT);
        WriteCoord(position.x); // position.x
        WriteCoord(position.y); // position.y
        WriteCoord(position.z); // position.z
        MessageEnd();
    }

    inline auto TE_Explosion2(Vector& position, byte startingColor, byte numColors) -> void {
        WriteByte(TE_EXPLOSION2);
        WriteCoord(position.x);   // position.x
        WriteCoord(position.y);   // position.y
        WriteCoord(position.z);   // position.z
        WriteByte(startingColor); // starting color
        WriteByte(numColors);     // num colors
        MessageEnd();
    }

    inline auto TE_BSPDecal(Vector& position, int textureIndex, int entityIndex, int modelIndex) -> void {
        WriteByte(TE_BSP_DECAL);
        WriteCoord(position.x);   // position.x
        WriteCoord(position.y);   // position.y
        WriteCoord(position.z);   // position.z
        WriteShort(textureIndex); // texture index of precached decal texture name)
        WriteShort(entityIndex);  // entity index
        if (entityIndex != 0) {
            WriteShort(modelIndex
            ); // index of model of above entity only included if previous short is non-zero (not the world)
        }
        MessageEnd();
    }

    inline auto TE_Implosion(Vector& position, byte radius, byte count, byte life) -> void {
        WriteByte(TE_IMPLOSION);
        WriteCoord(position.x); // position.x
        WriteCoord(position.y); // position.y
        WriteCoord(position.z); // position.z
        WriteByte(radius);      // radius
        WriteByte(count);       // count
        WriteByte(life);        // life in 0.1's
        MessageEnd();
    }

    inline auto TE_SpriteTrail(
        Vector& start, Vector& end, int spriteIndex, byte count, byte life, byte scale, byte velocity, byte randomness
    ) -> void {
        WriteByte(TE_SPRITE_TRAIL);
        WriteCoord(start.x);     // startposition.x
        WriteCoord(start.y);     // startposition.y
        WriteCoord(start.z);     // startposition.z
        WriteCoord(end.x);       // endposition.x
        WriteCoord(end.y);       // endposition.y
        WriteCoord(end.z);       // endposition.z
        WriteShort(spriteIndex); // sprite index
        WriteByte(count);        // count
        WriteByte(life);         // life in 0.1's
        WriteByte(scale);        // scale in 0.1's
        WriteByte(velocity);     // velocity along vector in 10's
        WriteByte(randomness);   // randomness of velocity in 10's
        MessageEnd();
    }

    inline auto TE_Sprite(Vector& position, int spriteIndex, byte scale, byte brightness) -> void {
        WriteByte(TE_SPRITE);
        WriteCoord(position.x);  // position.x
        WriteCoord(position.y);  // position.y
        WriteCoord(position.z);  // position.z
        WriteShort(spriteIndex); // sprite index
        WriteByte(scale);        // scale in 0.1's
        WriteByte(brightness);   // brightness
        MessageEnd();
    }

    inline auto TE_BeamSprite(Vector& start, Vector& end, int beamSpriteIndex, int endSpriteIndex) -> void {
        WriteByte(TE_BEAM_SPRITE);
        WriteCoord(start.x);         // startposition.x
        WriteCoord(start.y);         // startposition.y
        WriteCoord(start.z);         // startposition.z
        WriteCoord(end.x);           // endposition.x
        WriteCoord(end.y);           // endposition.y
        WriteCoord(end.z);           // endposition.z
        WriteShort(beamSpriteIndex); // beam sprite index
        WriteShort(endSpriteIndex);  // end sprite index
        MessageEnd();
    }

    inline auto TE_BeamTorus(
        Vector& position, Vector& axis, int spriteIndex, byte startingFrame, byte frameRate, byte life, byte lineWidth,
        byte noiseAmplitude, byte color[3], byte brightness, byte scrollSpeed
    ) -> void {
        WriteByte(TE_BEAM_TORUS);
        WriteCoord(position.x);    // position.x
        WriteCoord(position.y);    // position.y
        WriteCoord(position.z);    // position.z
        WriteCoord(axis.x);        // axis.x
        WriteCoord(axis.y);        // axis.y
        WriteCoord(axis.z);        // axis.z
        WriteShort(spriteIndex);   // sprite index
        WriteByte(startingFrame);  // starting frame
        WriteByte(frameRate);      // frame rate in 0.1's
        WriteByte(life);           // life in 0.1's
        WriteByte(lineWidth);      // line width in 0.1's
        WriteByte(noiseAmplitude); // noise amplitude in 0.01's
        WriteByte(color[0]);       // red
        WriteByte(color[1]);       // green
        WriteByte(color[2]);       // blue
        WriteByte(brightness);     // brightness
        WriteByte(scrollSpeed);    // scroll speed in 0.1's
        MessageEnd();
    }

    inline auto TE_BeamDisk(
        Vector& position, Vector& axis, int spriteIndex, byte startingFrame, byte frameRate, byte life, byte lineWidth,
        byte noiseAmplitude, byte color[3], byte brightness, byte scrollSpeed
    ) -> void {
        WriteByte(TE_BEAM_DISK);
        WriteCoord(position.x);    // position.x
        WriteCoord(position.y);    // position.y
        WriteCoord(position.z);    // position.z
        WriteCoord(axis.x);        // axis.x
        WriteCoord(axis.y);        // axis.y
        WriteCoord(axis.z);        // axis.z
        WriteShort(spriteIndex);   // sprite index
        WriteByte(startingFrame);  // starting frame
        WriteByte(frameRate);      // frame rate in 0.1's
        WriteByte(life);           // life in 0.1's
        WriteByte(lineWidth);      // line width in 0.1's
        WriteByte(noiseAmplitude); // noise amplitude in 0.01's
        WriteByte(color[0]);       // red
        WriteByte(color[1]);       // green
        WriteByte(color[2]);       // blue
        WriteByte(brightness);     // brightness
        WriteByte(scrollSpeed);    // scroll speed in 0.1's
        MessageEnd();
    }

    inline auto TE_BeamCylinder(
        Vector& position, Vector& axis, int spriteIndex, byte startingFrame, byte frameRate, byte life, byte lineWidth,
        byte noiseAmplitude, byte color[3], byte brightness, byte scrollSpeed
    ) -> void {
        WriteByte(TE_BEAM_CYLINDER);
        WriteCoord(position.x);    // position.x
        WriteCoord(position.y);    // position.y
        WriteCoord(position.z);    // position.z
        WriteCoord(axis.x);        // axis.x
        WriteCoord(axis.y);        // axis.y
        WriteCoord(axis.z);        // axis.z
        WriteShort(spriteIndex);   // sprite index
        WriteByte(startingFrame);  // starting frame
        WriteByte(frameRate);      // frame rate in 0.1's
        WriteByte(life);           // life in 0.1's
        WriteByte(lineWidth);      // line width in 0.1's
        WriteByte(noiseAmplitude); // noise amplitude in 0.01's
        WriteByte(color[0]);       // red
        WriteByte(color[1]);       // green
        WriteByte(color[2]);       // blue
        WriteByte(brightness);     // brightness
        WriteByte(scrollSpeed);    // scroll speed in 0.1's
        MessageEnd();
    }

    inline auto TE_BeamFollow(int entity, int spriteIndex, byte life, byte lineWidth, byte color[3], byte brightness)
    -> void {
        WriteByte(TE_BEAM_FOLLOW);
        WriteShort(entity);      // entity:attachment to follow
        WriteShort(spriteIndex); // sprite index
        WriteByte(life);         // life in 0.1's
        WriteByte(lineWidth);    // line width in 0.1's
        WriteByte(color[0]);     // red
        WriteByte(color[1]);     // green
        WriteByte(color[2]);     // blue
        WriteByte(brightness);   // brightness
        MessageEnd();
    }

    inline auto TE_GlowSprite(Vector& position, int modelIndex, byte scale, byte size, byte brightness) -> void {
        WriteByte(TE_GLOW_SPRITE);
        WriteCoord(position.x); // position.x
        WriteCoord(position.y); // position.y
        WriteCoord(position.z); // position.z
        WriteShort(modelIndex); // model index
        WriteByte(scale);       // scale * 10
        WriteByte(size);        // size
        WriteByte(brightness);  // brightness
        MessageEnd();
    }

    inline auto TE_BeamRing(
        int startEntity, int endEntity, int spriteIndex, byte startingFrame, byte frameRate, byte life, byte lineWidth,
        byte noiseAmplitude, byte color[3], byte brightness, byte scrollSpeed
    ) -> void {
        WriteByte(TE_BEAM_RING);
        WriteShort(startEntity);   // start entity
        WriteShort(endEntity);     // end entity
        WriteShort(spriteIndex);   // sprite index
        WriteByte(startingFrame);  // starting frame
        WriteByte(frameRate);      // frame rate in 0.1's
        WriteByte(life);           // life in 0.1's
        WriteByte(lineWidth);      // line width in 0.1's
        WriteByte(noiseAmplitude); // noise amplitude in 0.01's
        WriteByte(color[0]);       // red
        WriteByte(color[1]);       // green
        WriteByte(color[2]);       // blue
        WriteByte(brightness);     // brightness
        WriteByte(scrollSpeed);    // scroll speed in 0.1's
        MessageEnd();
    }

    inline auto TE_StreakSplash(Vector& start, Vector& vector, byte color, int count, int baseSpeed, int randomVelocity)
    -> void {
        WriteByte(TE_STREAK_SPLASH);
        WriteCoord(start.x);        // startposition.x
        WriteCoord(start.y);        // startposition.y
        WriteCoord(start.z);        // startposition.z
        WriteCoord(vector.x);       // vector.x
        WriteCoord(vector.y);       // vector.y
        WriteCoord(vector.z);       // vector.z
        WriteByte(color);           // color
        WriteShort(count);          // count
        WriteShort(baseSpeed);      // base speed
        WriteShort(randomVelocity); // random velocity
        MessageEnd();
    }

    inline auto TE_DynamicLight(Vector& position, byte radius, byte color[3], byte life, byte decayRate) -> void {
        WriteByte(TE_DYN_LIGHT);
        WriteCoord(position.x); // position.x
        WriteCoord(position.y); // position.y
        WriteCoord(position.z); // position.z
        WriteByte(radius);      // radius in 10's
        WriteByte(color[0]);    // red
        WriteByte(color[1]);    // green
        WriteByte(color[2]);    // blue
        WriteByte(life);        // life in 10's
        WriteByte(decayRate);   // decay rate in 10's
        MessageEnd();
    }

    inline auto TE_EntityLight(int entity, Vector& position, float radius, byte color[3], byte life, float decayRate)
    -> void {
        WriteByte(TE_ENT_LIGHT);
        WriteShort(entity);     // entity: attachment to follow
        WriteCoord(position.x); // position.x
        WriteCoord(position.y); // position.y
        WriteCoord(position.z); // position.z
        WriteCoord(radius);     // radius
        WriteByte(color[0]);    // red
        WriteByte(color[1]);    // green
        WriteByte(color[2]);    // blue
        WriteByte(life);        // life in 0.1's
        WriteCoord(decayRate);  // decay rate
        MessageEnd();
    }

    inline auto TE_TextMessage(
        int channel, int x, int y, byte effect, byte textColor[3], byte textAlpha, byte effectColor[3],
        byte effectAlpha,
        int fadeIn, int fadeOut, int holdTime, int fxTime, const char* text
    ) -> void {
        WriteByte(TE_TEXT_MESSAGE);
        WriteByte(channel);      // channel
        WriteShort(x);           // x -1 = center
        WriteShort(y);           // y -1 = center
        WriteByte(effect);       // effect: 0 = fade in/fade out, 1 is flickery credits, 2 is write out (training room)
        WriteByte(textColor[0]); // red - text color
        WriteByte(textColor[1]); // green
        WriteByte(textColor[2]); // blue
        WriteByte(textAlpha);    // alpha
        WriteByte(effectColor[0]); // red - effect color
        WriteByte(effectColor[1]); // green
        WriteByte(effectColor[2]); // blue
        WriteByte(effectAlpha);    // alpha
        WriteShort(fadeIn);        // fadein time
        WriteShort(fadeOut);       // fadeout time
        WriteShort(holdTime);      // hold time
        if (effect == 2) {
            WriteShort(fxTime); // fxtime: time the highlight lags behing the leading text in effect 2
        }
        WriteString(text); // 512 chars max string size
        MessageEnd();
    }

    inline auto TE_Line(Vector& start, Vector& end, int life, byte color[3]) -> void {
        WriteByte(TE_LINE);
        WriteCoord(start.x); // startposition.x
        WriteCoord(start.y); // startposition.y
        WriteCoord(start.z); // startposition.z
        WriteCoord(end.x);   // endposition.x
        WriteCoord(end.y);   // endposition.y
        WriteCoord(end.z);   // endposition.z
        WriteShort(life);    // life in 0.1's
        WriteByte(color[0]); // red
        WriteByte(color[1]); // green
        WriteByte(color[2]); // blue
        MessageEnd();
    }

    inline auto TE_Box(Vector& boxMins, Vector& boxMaxs, int life, byte color[3]) -> void {
        WriteByte(TE_BOX);
        WriteCoord(boxMins.x); // boxmins.x
        WriteCoord(boxMins.y); // boxmins.y
        WriteCoord(boxMins.z); // boxmins.z
        WriteCoord(boxMaxs.x); // boxmaxs.x
        WriteCoord(boxMaxs.y); // boxmaxs.y
        WriteCoord(boxMaxs.z); // boxmaxs.z
        WriteShort(life);      // life in 0.1's
        WriteByte(color[0]);   // red
        WriteByte(color[1]);   // green
        WriteByte(color[2]);   // blue
        MessageEnd();
    }

    inline auto TE_KillBeam(int entity) -> void {
        WriteByte(TE_KILL_BEAM);
        WriteShort(entity); // entity
        MessageEnd();
    }

    inline auto TE_LargeFunnel(Vector& position, int spriteIndex, int flags) -> void {
        WriteByte(TE_LARGE_FUNNEL);
        WriteCoord(position.x);  // position.x
        WriteCoord(position.y);  // position.y
        WriteCoord(position.z);  // position.z
        WriteShort(spriteIndex); // sprite index
        WriteShort(flags);       // flags
        MessageEnd();
    }

    inline auto TE_BloodStream(Vector& position, Vector& vector, byte color, byte speed) -> void {
        WriteByte(TE_BLOODSTREAM);
        WriteCoord(position.x); // position.x
        WriteCoord(position.y); // position.y
        WriteCoord(position.z); // position.z
        WriteCoord(vector.x);   // vector.x
        WriteCoord(vector.y);   // vector.y
        WriteCoord(vector.z);   // vector.z
        WriteByte(color);       // color
        WriteByte(speed);       // speed
        MessageEnd();
    }

    inline auto TE_ShowLine(Vector& start, Vector& end) -> void {
        WriteByte(TE_SHOW_LINE);
        WriteCoord(start.x); // startposition.x
        WriteCoord(start.y); // startposition.y
        WriteCoord(start.z); // startposition.z
        WriteCoord(end.x);   // endposition.x
        WriteCoord(end.y);   // endposition.y
        WriteCoord(end.z);   // endposition.z
        MessageEnd();
    }

    inline auto TE_Blood(Vector& position, Vector& vector, byte color, byte speed) -> void {
        WriteByte(TE_BLOOD);
        WriteCoord(position.x); // position.x
        WriteCoord(position.y); // position.y
        WriteCoord(position.z); // position.z
        WriteCoord(vector.x);   // vector.x
        WriteCoord(vector.y);   // vector.y
        WriteCoord(vector.z);   // vector.z
        WriteByte(color);       // color
        WriteByte(speed);       // speed
        MessageEnd();
    }

    inline auto TE_Decal(Vector& position, byte textureIndex, int entityIndex) -> void {
        WriteByte(TE_DECAL);
        WriteCoord(position.x);  // position.x
        WriteCoord(position.y);  // position.y
        WriteCoord(position.z);  // position.z
        WriteByte(textureIndex); // texture index of precached decal texture name
        WriteShort(entityIndex); // entity index
        MessageEnd();
    }

    inline auto TE_Fizz(int entity, int spriteIndex, byte density) -> void {
        WriteByte(TE_FIZZ);
        WriteShort(entity);      // entity
        WriteShort(spriteIndex); // sprite index
        WriteByte(density);      // density
        MessageEnd();
    }

    inline auto
    TE_Model(Vector& position, Vector& velocity, float initialYaw, int modelIndex, byte bounceSoundType, byte life)
    -> void {
        WriteByte(TE_MODEL);
        WriteCoord(position.x);     // position.x
        WriteCoord(position.y);     // position.y
        WriteCoord(position.z);     // position.z
        WriteCoord(velocity.x);     // velocity.x
        WriteCoord(velocity.y);     // velocity.y
        WriteCoord(velocity.z);     // velocity.z
        WriteAngle(initialYaw);     // initial yaw
        WriteShort(modelIndex);     // model index
        WriteByte(bounceSoundType); // bounce sound type
        WriteByte(life);            // life in 0.1's
        MessageEnd();
    }

    inline auto TE_ExplodeModel(Vector& origin, Vector& velocity, int modelIndex, int count, byte life) -> void {
        WriteByte(TE_EXPLODE_MODEL);
        WriteCoord(origin.x);   // origin.x
        WriteCoord(origin.y);   // origin.y
        WriteCoord(origin.z);   // origin.z
        WriteCoord(velocity.x); // velocity.x
        WriteCoord(velocity.y); // velocity.y
        WriteCoord(velocity.z); // velocity.z
        WriteShort(modelIndex); // model index
        WriteShort(count);      // count
        WriteByte(life);        // life in 0.1's
        MessageEnd();
    }

    inline auto TE_BreakModel(
        Vector& position, Vector& size, Vector& velocity, byte randomVelocity, int modelIndex, byte count, byte life,
        byte flags
    ) -> void {
        WriteByte(TE_BREAK_MODEL);
        WriteCoord(position.x);    // position.x
        WriteCoord(position.y);    // position.y
        WriteCoord(position.z);    // position.z
        WriteCoord(size.x);        // size.x
        WriteCoord(size.y);        // size.y
        WriteCoord(size.z);        // size.z
        WriteCoord(velocity.x);    // velocity.x
        WriteCoord(velocity.y);    // velocity.y
        WriteCoord(velocity.z);    // velocity.z
        WriteByte(randomVelocity); // random velocity in 10's
        WriteShort(modelIndex);    // sprite or model index
        WriteByte(count);          // count
        WriteByte(life);           // life in 0.1 secs
        WriteByte(flags);          // flags
        MessageEnd();
    }

    inline auto TE_GunShotDecal(Vector& position, int entityIndex, byte decalNumber) -> void {
        WriteByte(TE_GUNSHOT_DECAL);
        WriteCoord(position.x);  // position.x
        WriteCoord(position.y);  // position.y
        WriteCoord(position.z);  // position.z
        WriteShort(entityIndex); // entity index
        WriteByte(decalNumber);  // decal number
        MessageEnd();
    }

    inline auto TE_SpriteSpray(Vector& position, Vector& velocity, int spriteIndex, byte count, byte speed, byte noise)
    -> void {
        WriteByte(TE_SPRITE_SPRAY);
        WriteCoord(position.x);  // position.x
        WriteCoord(position.y);  // position.y
        WriteCoord(position.z);  // position.z
        WriteCoord(velocity.x);  // velocity.x
        WriteCoord(velocity.y);  // velocity.y
        WriteCoord(velocity.z);  // velocity.z
        WriteShort(spriteIndex); // sprite index
        WriteByte(count);        // count
        WriteByte(speed);        // speed
        WriteByte(noise);        // noise
        MessageEnd();
    }

    inline auto TE_ArmorRicochet(Vector& position, byte scale) -> void {
        WriteByte(TE_ARMOR_RICOCHET);
        WriteCoord(position.x); // position.x
        WriteCoord(position.y); // position.y
        WriteCoord(position.z); // position.z
        WriteByte(scale);       // scale in 0.1's
        MessageEnd();
    }

    inline auto TE_PlayerDecal(int playerindex, Vector& position, int entity, byte decalNumber) -> void {
        WriteByte(TE_PLAYER_DECAL);
        WriteByte(playerindex); // playerindex
        WriteCoord(position.x); // position.x
        WriteCoord(position.y); // position.y
        WriteCoord(position.z); // position.z
        WriteShort(entity);     // entity
        WriteByte(decalNumber); // decal number
        MessageEnd();
    }

    inline auto
    TE_Bubbles(
        Vector& positionMin, Vector& positionMax, float height, int modelIndex, byte count, float speed
    ) -> void {
        WriteByte(TE_BUBBLES);
        WriteCoord(positionMin.x); // position.x (min start position)
        WriteCoord(positionMin.y); // position.y
        WriteCoord(positionMin.z); // position.z
        WriteCoord(positionMax.x); // position.x (max start position)
        WriteCoord(positionMax.y); // position.y
        WriteCoord(positionMax.z); // position.z
        WriteCoord(height);        // float height
        WriteShort(modelIndex);    // model index
        WriteByte(count);          // count
        WriteCoord(speed);         // speed
        MessageEnd();
    }

    inline auto
    TE_BubbleTrail(Vector& positionMin, Vector& positionMax, float height, int modelIndex, byte count, float speed)
    -> void {
        WriteByte(TE_BUBBLE_TRAIL);
        WriteCoord(positionMin.x); // position.x (min start position)
        WriteCoord(positionMin.y); // position.y
        WriteCoord(positionMin.z); // position.z
        WriteCoord(positionMax.x); // position.x (max start position)
        WriteCoord(positionMax.y); // position.y
        WriteCoord(positionMax.z); // position.z
        WriteCoord(height);        // float height
        WriteShort(modelIndex);    // model index
        WriteByte(count);          // count
        WriteCoord(speed);         // speed
        MessageEnd();
    }

    inline auto TE_BloodSprite(Vector& position, int sprite1Index, int sprite2Index, byte color, byte scale) -> void {
        WriteByte(TE_BLOOD_SPRITE);
        WriteCoord(position.x);   // position.x
        WriteCoord(position.y);   // position.y
        WriteCoord(position.z);   // position.z
        WriteShort(sprite1Index); // sprite1 index
        WriteShort(sprite2Index); // sprite2 index
        WriteByte(color);         // color
        WriteByte(scale);         // scale
        MessageEnd();
    }

    inline auto TE_WorldDecal(Vector& position, byte textureIndex) -> void {
        WriteByte(TE_WORLD_DECAL);
        WriteCoord(position.x);  // position.x
        WriteCoord(position.y);  // position.y
        WriteCoord(position.z);  // position.z
        WriteByte(textureIndex); // texture index of precached decal texture name
        MessageEnd();
    }

    inline auto TE_WorldDecalHigh(Vector& position, byte textureIndex) -> void {
        WriteByte(TE_WORLD_DECAL_HIGH);
        WriteCoord(position.x);  // position.x
        WriteCoord(position.y);  // position.y
        WriteCoord(position.z);  // position.z
        WriteByte(textureIndex); // texture index of precached decal texture name - 256
        MessageEnd();
    }

    inline auto TE_DecalHigh(Vector& position, byte textureIndex, int entityIndex) -> void {
        WriteByte(TE_DECAL_HIGH);
        WriteCoord(position.x);  // position.x
        WriteCoord(position.y);  // position.y
        WriteCoord(position.z);  // position.z
        WriteByte(textureIndex); // txture index of precached decal texture name - 256)
        WriteShort(entityIndex); // entity index
        MessageEnd();
    }

    inline auto TE_Projectile(Vector& position, Vector& velocity, int modelIndex, byte life, byte owner) -> void {
        WriteByte(TE_PROJECTILE);
        WriteCoord(position.x); // position.x
        WriteCoord(position.y); // position.y
        WriteCoord(position.z); // position.z
        WriteCoord(velocity.x); // velocity.x
        WriteCoord(velocity.y); // velocity.y
        WriteCoord(velocity.z); // velocity.z
        WriteShort(modelIndex); // modelindex
        WriteByte(life);        // life
        WriteByte(owner); // owner: projectile won't collide with owner (if owner == 0, projectile will hit any client)
        MessageEnd();
    }

    inline auto
    TE_Spray(Vector& position, Vector& direction, int modelIndex, byte count, byte speed, byte noise, byte renderMode)
    -> void {
        WriteByte(TE_SPRAY);
        WriteCoord(position.x);  // position.x
        WriteCoord(position.y);  // position.y
        WriteCoord(position.z);  // position.z
        WriteCoord(direction.x); // direction.x
        WriteCoord(direction.y); // direction.y
        WriteCoord(direction.z); // direction.z
        WriteShort(modelIndex);  // modelindex
        WriteByte(count);        // count
        WriteByte(speed);        // speed
        WriteByte(noise);        // noise
        WriteByte(renderMode);   // rendermode
        MessageEnd();
    }

    inline auto TE_PlayerSprites(int playerNum, int spriteIndex, byte count, byte variance) -> void {
        WriteByte(TE_PLAYER_SPRITES);
        WriteShort(playerNum);   // playernum
        WriteShort(spriteIndex); // sprite modelindex
        WriteByte(count);        // count
        WriteByte(variance);     // variance (0 = no variance in size) (10 = 10% variance in size)
        MessageEnd();
    }

    inline auto TE_ParticleBurst(Vector& origin, int radius, byte particleColor, byte duration) -> void {
        WriteByte(TE_PARTICLE_BURST);
        WriteCoord(origin.x); // origin
        WriteCoord(origin.y);
        WriteCoord(origin.z);
        WriteShort(radius);       // radius
        WriteByte(particleColor); // particle color
        WriteByte(duration);      // duration * 10 (will be randomized a bit)
        MessageEnd();
    }

    inline auto
    TE_FireField(Vector& origin, int radius, int modelIndex, byte count, byte flags, byte duration) -> void {
        WriteByte(TE_FIRE_FIELD);
        WriteCoord(origin.x); // origin
        WriteCoord(origin.y);
        WriteCoord(origin.z);
        WriteShort(radius);     // radius (fire is made in a square around origin. -radius, -radius to radius, radius)
        WriteShort(modelIndex); // modelindex
        WriteByte(count);       // count
        WriteByte(flags);       // flags
        WriteByte(duration);    // duration (in seconds) * 10 (will be randomized a bit)
        MessageEnd();
    }

    inline auto TE_PlayerAttachment(byte entityIndex, float verticalOffset, int modelIndex, int life) -> void {
        WriteByte(TE_PLAYER_ATTACHMENT);
        WriteByte(entityIndex);     // entity index of player
        WriteCoord(verticalOffset); // vertical offset (attachment origin.z = player origin.z + vertical offset)
        WriteShort(modelIndex);     // model index
        WriteShort(life);           // life * 10
        MessageEnd();
    }

    inline auto TE_KillPlayerAttachments(byte entityIndex) -> void {
        WriteByte(TE_KILL_PLAYER_ATTACHMENTS);
        WriteByte(entityIndex); // entity index of player
        MessageEnd();
    }

    inline auto
    TE_MultiGunShot(
        Vector& origin, Vector& direction, float xNoise, float yNoise, byte count, byte textureIndex
    ) -> void {
        WriteByte(TE_MULTI_GUNSHOT);
        WriteCoord(origin.x);    // origin.x
        WriteCoord(origin.y);    // origin.y
        WriteCoord(origin.z);    // origin.z
        WriteCoord(direction.x); // direction.x
        WriteCoord(direction.y); // direction.y
        WriteCoord(direction.z); // direction.z
        WriteCoord(xNoise);      // x noise * 100
        WriteCoord(yNoise);      // y noise * 100
        WriteByte(count);        // count
        WriteByte(textureIndex); // bullethole decal texture index
        MessageEnd();
    }

    inline auto TE_UserTracer(Vector& origin, Vector& velocity, byte life, byte color, byte length) -> void {
        WriteByte(TE_USER_TRACER);
        WriteCoord(origin.x);   // origin.x
        WriteCoord(origin.y);   // origin.y
        WriteCoord(origin.z);   // origin.z
        WriteCoord(velocity.x); // velocity.x
        WriteCoord(velocity.y); // velocity.y
        WriteCoord(velocity.z); // velocity.z
        WriteByte(life);        // life * 10
        WriteByte(color);       // color: this is an index into an array of color vectors in the engine
        WriteByte(length);      // length * 10
        MessageEnd();
    }
}
