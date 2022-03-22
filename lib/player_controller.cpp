#include "player_controller.h"
#include <loguru.hpp>

using namespace std;

PlayerController::PlayerController(Scene *scene, Player *player)
    : _scene(scene), _player(player)
{
}