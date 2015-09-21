#include <iostream>
#include "SelectedCastleResponse.h"

#include "../../game/Game.h"

void CSelectedCastleResponse::Process(CGame *game){
    CPointerLocationResponse::Process(game);
    CPlayer* Player = game->GameState()->GetPlayerWithColor(
            CPlayer::GetColorForID(DPlayerID));
    Player->UpdateHoveredCastle(game);
    Player->PlaceHomeCastle(game, Player->DHoveredCastle);
    Player->DHoveredCastle = NULL;
}
