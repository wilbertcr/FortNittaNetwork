#include "SpectatorLeftResponse.h"

#include "../../game/Game.h"
#include <algorithm>

void CSpectatorLeftResponse::Process(CGame *game){
    CRoom& Room = game->GameState()->DRoom;
    std::vector<std::string>::iterator NewEnd;
    NewEnd = std::remove(Room.DSpectatorUsernames.begin(),
            Room.DSpectatorUsernames.end(),
            DUsername);
    Room.DSpectatorUsernames.erase(NewEnd, Room.DSpectatorUsernames.end());
}
