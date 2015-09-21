/*
 * EndGameResponse.cpp
 *
 *  Created on: Mar 4, 2015
 *      Author: wsequeira
 */

#include "EndGameResponse.h"

#include "../../game/Game.h"
#include "../../game/modes/GameOverMode.h"

void CEndGameResponse::Process(CGame *game){
    game->SwitchMode(new CGameOverMode(DWinnerID));
}

