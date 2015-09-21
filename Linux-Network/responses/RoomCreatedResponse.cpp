#include "RoomCreatedResponse.h"
#include "../../game/Game.h"
#include "../../game/modes/RoomMenuMode.h"

void CRoomCreatedResponse::Process(CGame *game){
    game->SwitchMode(new CRoomMenuMode(game));
}
