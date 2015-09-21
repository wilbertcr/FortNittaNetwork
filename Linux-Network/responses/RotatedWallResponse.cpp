#include "RotatedWallResponse.h"

#include "../../game/Game.h"

void CRotatedWallResponse::Process(CGame* game){
    CPointerLocationResponse::Process(game);
    CPlayer* Player = game->GameState()->GetPlayerWithColor(
          	CPlayer::GetColorForID(DPlayerID));
    Player->UpdateCursorTilePosition(game);
    Player->RotateWall(game);
}
