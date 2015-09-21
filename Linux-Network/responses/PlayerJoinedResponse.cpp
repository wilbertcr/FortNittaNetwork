#include "PlayerJoinedResponse.h"

#include "../../game/Game.h"
#include "../../game/modes/RoomMenuMode.h"
#include "../../game/players/Player.h"

void CPlayerJoinedResponse::Process(CGame *game){
    CRoomMenuMode* Mode;
    game->GameState()->DRoom.DPlayerUsernames.push_back(DUsername);
}
