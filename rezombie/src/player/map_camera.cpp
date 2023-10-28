#include "rezombie/player/player.h"

namespace rz
{
    auto Player::setMapCamera(int cameraId) -> void {
        auto& camera = getMapCameraVars();
        camera.setEnabled(cameraId != 0);
        camera.setCamera(cameraId);
        if (camera.isEnabled()) {
            SetView(*this, MapCameras.getCamera());
        } else {
            SetView(*this, *this);
        }
    }

    auto Player::MapCameraUpdate() -> void {
        auto& camera = getMapCameraVars();
        if (!camera.isEnabled()) {
            return;
        }
        if (camera.getNextCameraTime() != 0.f && camera.getNextCameraTime() <= g_global_vars->time) {
            camera.setCamera(MapCameras.begin() + ((camera.getCamera() - MapCameras.begin()) + 1) % MapCameras.count());
            camera.setNextCameraTime(g_global_vars->time + 2);
        }
    }
}
