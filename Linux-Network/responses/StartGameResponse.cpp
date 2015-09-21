#include "StartGameResponse.h"

#include "../../game/Game.h"
#include "../../game/modes/SelectCastleMode.h"
#include "../../game/modes/BannerTransitionMode.h"
#include "../../game/players/NetworkedHumanPlayer.h"

void CStartGameResponse::Process(CGame* game){
    int i;
    std::vector<std::string>::iterator it;

    game->GameState()->DTimeStep = 0;
    game->GameState()->DPlayers.clear();
    for(i = 0, it = DPlayers.begin();
            it != DPlayers.end();
            i++, it++){
        CPlayer* Player;
        //g_print("%s\n", game->GameState()->DUsername.c_str());
        if(*it == game->GameState()->DUsername){
            Player = new CNetworkedHumanPlayer();
        }else{
            Player = new CPlayer();
        }
        Player->DUsername = *it;
        Player->DColor = CPlayer::GetColorForID(i+1);
        game->GameState()->DPlayers.push_back(Player);
    }
    CTerrainMap* TerrainMap = new CTerrainMap();
    TerrainMap->LoadMapString(
            &(game->Resources()->DTilesets->D2DTerrainTileset),
            &(game->Resources()->DTilesets->D3DTerrainTileset),
            DMap);
    game->GameState()->TerrainMap(TerrainMap);
    game->GameState()->TerrainMap()->LoadMapTileset(game);
    game->GameState()->TerrainMap()->Reset();
    game->SwitchMode(new CBannerTransitionMode(
                game, "SELECT CASTLE",
                game->GameMode(), new CSelectCastleMode()));
}
