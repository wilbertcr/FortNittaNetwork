#include "../game/Game.h"
#include "responses/NetworkResponse.h"

//#define TEST_JOIN
#define SHOW_TIME_STEP

void UpdateFakeEvents(CGame* game, std::deque<CNetworkResponse*>& DEvents){
    int NextTime = 100;
    CPlayer::EPlayerColor OtherPlayer = 
        game->GameState()->DRoom.DIsOwnedByLocalUser ?
            CPlayer::pcRed : CPlayer::pcBlue;
#ifdef SHOW_TIME_STEP
    g_print("[%d]\n", game->GameState()->DTimeStep);
#endif
#ifdef TEST_JOIN
    if(!game->GameState()->DRoom.DIsOwnedByLocalUser){
        if(game->GameState()->DTimeStep == NextTime){
            std::vector<std::string> Players;
            Players.push_back("bob");
            Players.push_back("test");
            DEvents.push_back(new CStartGameResponse(Players, ""));
        }
    }
#endif
    if(game->GameState()->GetPlayerWithColor(OtherPlayer) != NULL){
        if(game->GameState()->DTimeStep == (NextTime += 100)){
            DEvents.push_back(new CEndGameResponse(OtherPlayer)); 
        }
        if(game->GameState()->DTimeStep == (NextTime += 100)){
            DEvents.push_back(new CPointerLocationResponse(game->GameState()->DTimeStep,
                        OtherPlayer, 8000, 8000)); 
        }
        if(game->GameState()->DTimeStep == (NextTime+=50)){
            DEvents.push_back(new CSelectedCastleResponse(game->GameState()->DTimeStep,
                        OtherPlayer, 1000, 1000)); 
        }
        if(game->GameState()->DTimeStep == (NextTime+=100)){
            DEvents.push_back(new CPlacedCannonResponse(game->GameState()->DTimeStep,
                        OtherPlayer, 3625, 7812)); 
        }
        if(game->GameState()->DTimeStep == (NextTime+=10)){
            DEvents.push_back(new CPlacedCannonResponse(game->GameState()->DTimeStep,
                        OtherPlayer, 4083, 6875)); 
        }
        if(game->GameState()->DTimeStep == (NextTime+=10)){
            DEvents.push_back(new CPlacedCannonResponse(game->GameState()->DTimeStep,
                        OtherPlayer, 4062, 8506)); 
        }
        if(game->GameState()->DTimeStep == (NextTime+=100)){
            DEvents.push_back(new CPointerLocationResponse(game->GameState()->DTimeStep,
                        OtherPlayer, 3000, 3000)); 
        }
        if(game->GameState()->DTimeStep == (NextTime+=10)){
            DEvents.push_back(new CFiredCannonResponse(game->GameState()->DTimeStep,
                        OtherPlayer, 5812, 3506)); 
        }
        if(game->GameState()->DTimeStep == (NextTime+=10)){
            DEvents.push_back(new CFiredCannonResponse(game->GameState()->DTimeStep,
                        OtherPlayer, 5812, 3506)); 
        }
        if(game->GameState()->DTimeStep == (NextTime+=10)){
            DEvents.push_back(new CFiredCannonResponse(game->GameState()->DTimeStep,
                        OtherPlayer, 5812, 3506)); 
        }
        if(game->GameState()->DTimeStep == (NextTime+=150)){
            DEvents.push_back(new CPointerLocationResponse(game->GameState()->DTimeStep,
                        OtherPlayer, 6000, 8506)); 
        }
        if(game->GameState()->DTimeStep == (NextTime+=10)){
            DEvents.push_back(new CPlacedWallResponse(game->GameState()->DTimeStep,
                        OtherPlayer, 6100, 8506)); 
        }
        if(game->GameState()->DTimeStep == (NextTime+=10)){
            DEvents.push_back(new CPlacedWallResponse(game->GameState()->DTimeStep,
                        OtherPlayer, 6320, 8506)); 
        }
        if(game->GameState()->DTimeStep == (NextTime+=10)){
            DEvents.push_back(new CRotatedWallResponse(game->GameState()->DTimeStep,
                        OtherPlayer, 6520, 8506)); 
        }
        if(game->GameState()->DTimeStep == (NextTime+=10)){
            DEvents.push_back(new CPlacedWallResponse(game->GameState()->DTimeStep,
                        OtherPlayer, 6520, 8506)); 
        }
    }
}
