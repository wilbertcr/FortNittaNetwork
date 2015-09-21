#include "RoomDeniedResponse.h"

#include "../../game/Game.h"
#include "../../game/modes/MultiplayerMenuMode.h"

void CRoomDeniedResponse::Process(CGame *game){
    CMultiplayerMenuMode* Mode;    
    if((Mode = dynamic_cast<CMultiplayerMenuMode*>(game->GameMode()))){
        Mode->SetResponseText(ROOM_DENIED_TEXT);
    } 
}
