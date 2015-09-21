#include "LoginResponse.h"
#include "../../game/Game.h"
#include "../../game/modes/MultiplayerMenuMode.h"
#include "../../game/players/NetworkedHumanPlayer.h"

void CLoginResponse::Process(CGame *game){
    // TODO check if login was success and do something else if not
    CNetworkedHumanPlayer* Player = new CNetworkedHumanPlayer();
    Player->DUsername = game->GameState()->DUsername;
    Player->DColor = CPlayer::pcBlue;
    game->GameState()->DPlayers.push_back(Player);    
    game->SwitchMode(new CMultiplayerMenuMode(game));
}
