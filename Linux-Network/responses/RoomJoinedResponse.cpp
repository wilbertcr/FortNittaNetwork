#include "RoomJoinedResponse.h"

#include "../../game/modes/RoomMenuMode.h"
#include "../../game/Game.h"

#include <fstream>

//#define TEST_BROADCAST

std::ofstream LogJoined("Logjoined.txt", std::ofstream::app);
void CRoomJoinedResponse::Process(CGame* game){

    game->GameState()->DRoom  = CRoom(DRoomName, DCapacity);
    game->GameState()->DRoom.DIsMultiplayer = true;
#ifdef TEST_BROADCAST
    game->GameState()->DRoom.DIsBroadcast = true;
#endif
    LogJoined << "Joining room: " << game->GameState()->DRoom.Name() << "\n";
    LogJoined.flush();
    LogJoined << "Players on room: \n";
    LogJoined.flush();
    for(std::vector<std::string>::iterator it = DPlayers.begin();
            it != DPlayers.end();
            it++){
    	LogJoined << *it << "\n";
    	LogJoined.flush();
        game->GameState()->DRoom.DPlayerUsernames.push_back(*it);
    }
    game->SwitchMode(new CRoomMenuMode(game));
}
