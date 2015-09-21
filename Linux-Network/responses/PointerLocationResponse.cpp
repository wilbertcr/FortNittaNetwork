#include <iostream>
#include "PointerLocationResponse.h"

#include "../../game/Game.h"

void CPointerLocationResponse::Process(CGame* game){
    CPlayer* Player = game->GameState()->
        GetPlayerWithColor(CPlayer::GetColorForID(DPlayerID));
    if(Player == NULL){
        g_print("Player [%d] not found", DPlayerID);
    }else{
        Player->DCursorPosition = game->GameState()
            ->Network()->ConvertFromNetworkPosition(game, SInt2(DXcoordinate, DYcoordinate));
        Player->UpdateCursorTilePosition(game);
    }
}
