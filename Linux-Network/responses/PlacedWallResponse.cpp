#include "PlacedWallResponse.h"

#include "../../game/Game.h"

void CPlacedWallResponse::Process(CGame* game){
    CPointerLocationResponse::Process(game);
    CPlayer* Player = game->GameState()->GetPlayerWithColor(
            (CPlayer::EPlayerColor)DPlayerID);
    Player->UpdateCursorTilePosition(game);
    if(Player->TryToPlaceWall(game, Player->DCursorTilePosition)){
        Player->DWallShape.Randomize(Player->DRandomNumberGenerator.Random());
    }
}
