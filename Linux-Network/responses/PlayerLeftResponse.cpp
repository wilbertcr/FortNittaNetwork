#include "PlayerLeftResponse.h"

#include "../../game/Game.h"
#include <algorithm>

void CPlayerLeftResponse::Process(CGame *game){
    CRoom& Room = game->GameState()->DRoom;
    std::vector<std::string>::iterator NewEnd;
    NewEnd = std::remove(Room.DPlayerUsernames.begin(),
            Room.DPlayerUsernames.end(),
            DUsername);
    Room.DPlayerUsernames.erase(NewEnd, Room.DPlayerUsernames.end());
}
