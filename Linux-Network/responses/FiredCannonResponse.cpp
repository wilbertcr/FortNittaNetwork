#include "FiredCannonResponse.h"

#include "../../game/Game.h"

void CFiredCannonResponse::Process(CGame* game){
    CPointerLocationResponse::Process(game);
    CPlayer* Player = game->GameState()->GetPlayerWithColor(
            CPlayer::GetColorForID(DPlayerID));
    Player->UpdateCursorTilePosition(game);
    Player->FireNextCannon(game);
}
