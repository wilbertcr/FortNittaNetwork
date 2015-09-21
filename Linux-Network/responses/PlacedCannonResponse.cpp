#include "PlacedCannonResponse.h"

#include "../../game/Game.h"

void CPlacedCannonResponse::Process(CGame* game){
    CPointerLocationResponse::Process(game);
    CPlayer* Player = game->GameState()->GetPlayerWithColor(
            CPlayer::GetColorForID(DPlayerID));
    Player->UpdateCursorTilePosition(game);
    Player->TryToPlaceCannon(game, Player->DCursorTilePosition);
}
