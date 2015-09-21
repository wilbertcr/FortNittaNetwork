#include "RoomsAvailableResponse.h"

#include "../../game/modes/MultiplayerMenuMode.h"
#include "../../game/Game.h"

void CRoomsAvailableResponse::Process(CGame* game){
    CMultiplayerMenuMode* Mode;    
    if((Mode = dynamic_cast<CMultiplayerMenuMode*>(game->GameMode()))){
        Mode->UpdateAvailableRooms(game, DRooms);        
    } 
}
