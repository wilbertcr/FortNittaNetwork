#include "SpectatorJoinedResponse.h"

#include "../../game/Game.h"
#include "../../game/modes/RoomMenuMode.h"
#include "../../game/players/Player.h"

void CSpectatorJoinedResponse::Process(CGame *game){
    CRoomMenuMode* Mode;
    game->GameState()->DRoom.DSpectatorUsernames.push_back(DUsername);
}
