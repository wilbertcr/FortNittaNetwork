#include "OwnerLeftRoomResponse.h"

#include "../../game/Game.h"
#include "../../game/modes/MultiplayerMenuMode.h"

void COwnerLeftRoomResponse::Process(CGame* game){
    game->SwitchMode(new CMultiplayerMenuMode(game));
}
