#include "rezombie/player/player.h"
#include "rezombie/thirdcamera/third_camera.h"
#include <metamod/utils.h>

namespace rz
{
    auto Player::setThirdCamera(bool isEnabled) -> void {
        auto& camera = getThirdCameraVars();
        camera.setEnabled(isEnabled);
        if (camera.isEnabled()) {
            SetView(*this, ThirdCamera.getCamera());
        } else {
            SetView(*this, *this);
        }
    }

    auto Player::ThirdCameraUpdate() -> void {
        auto& camera = getThirdCameraVars();
        if (!camera.isEnabled()) {
            return;
        }
        float maxDistance;
        float maxRightOffset;

        //metamod::utils::LogConsole("%.0f %.0f %.0f", getViewAngle().x, getViewAngle().y, getViewAngle().z);

        bool isZooming;
        auto distance = 96.f;//gViewCamera[id][ViewCamera_Distance];
        auto rightOffset = 0.f;//gViewCamera[id][ViewCamera_RightOffset];

        if (getButton() & IN_ATTACK2) {
            isZooming = true;
            maxDistance = 64;
            maxRightOffset = 16;
        } else {
            isZooming = false;
            maxDistance = 96;//gViewCamera[id][ViewCamera_MaxDistance];
            maxRightOffset = 0;
        }

        if (isZooming) {
            distance = std::max(distance - 3, maxDistance);
            rightOffset = std::min(rightOffset + 1, maxRightOffset);
        } else {
            distance = std::min(distance + 1, maxDistance);
            rightOffset = std::max(rightOffset - 1, maxRightOffset);
        }

        //gViewCamera[id][ViewCamera_RightOffset] = rightOffset;
        //gViewCamera[id][ViewCamera_Distance] = distance;

        //new Float:vecVelocity[3];
        //get_entvar(id, var_velocity, vecVelocity);

        /*if (~flags & FL_ONGROUND && vecVelocity[2] > 0.0)
        {
            //static Float:vecDest[3];
            vecDest = vecSrc;
            vecDest[2] = -8192.0;

            engfunc(EngFunc_TraceHull, vecSrc, vecDest, DONT_IGNORE_MONSTERS, HULL_HEAD, id, 0);
            get_tr2(0, TR_vecEndPos, vecSrc);

            vecSrc[2] += 42.0;
        }
        else*/
        auto vecSrc = getOrigin();
        vecSrc.z += (getFlags() & FL_DUCKING) ? 42.0 : 24.0;

        MakeVectors(getViewAngle());
        auto vecEndPos = vecSrc + g_global_vars->vec_forward * -distance + g_global_vars->vec_right * rightOffset;

        TraceResult trace;
        TraceHull(vecSrc, vecEndPos, TR_IGNORE_MONSTERS, 3, *this, &trace);

        if (trace.fraction != 1) {
            vecEndPos = trace.end_position;
        }

        auto modelAngles = camera.getModelAngles();
        if (isZooming) {
            modelAngles.x = getViewAngle().x;
            modelAngles.y = getViewAngle().y;
        } else {
            modelAngles.x = 0.0;
            if (getButton() & IN_FORWARD) {
                modelAngles.y = getViewAngle().y;
            } else if (getButton() & IN_BACK) {
                modelAngles.y = getViewAngle().y - 180;
            }
            if (getButton() & IN_MOVE_LEFT && ~getButton() & IN_MOVE_RIGHT) {
                modelAngles.y = getViewAngle().y + 90;
                if (getButton() & IN_FORWARD) {
                    modelAngles.y -= 45;
                } else if (getButton() & IN_BACK) {
                    modelAngles.y += 45;
                }
            } else if (getButton() & IN_MOVE_RIGHT && ~getButton() & IN_MOVE_LEFT) {
                modelAngles.y = getViewAngle().y - 90;
                if (getButton() & IN_FORWARD) {
                    modelAngles.y += 45;
                } else if (getButton() & IN_BACK) {
                    modelAngles.y -= 45;
                }
            }
        }
        camera.setOrigin(vecEndPos);
        camera.setAngles(getViewAngle());
        camera.setModelAngles(modelAngles);
        setAngles(modelAngles);

        //set_entvar( iPlayer, var_renderamt, floatmax( get_distance_f( vecEndPos, vecOrigin ) / 32.0 * 255.0, 128.0 ) );
    }
}
