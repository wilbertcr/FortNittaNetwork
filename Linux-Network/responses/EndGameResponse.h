/*
 * EndGameResponse.h
 *
 *  Created on: Mar 4, 2015
 *      Author: wsequeira
 */

#ifndef ENDGAMERESPONSE_H_
#define ENDGAMERESPONSE_H_

#include "NetworkResponse.h"

#include "../../game/Game.h"

class CEndGameResponse: public CNetworkResponse {
public:
	CEndGameResponse(int16_t winnerID) : CNetworkResponse(1) {
		DWinnerID = winnerID;
		DAction = (int16_t) END_GAME;
	}

	void Process(CGame* game);


protected:
	int16_t DWinnerID;

};

#endif /* SOURCE_NETWORK_RESPONSES_ENDGAMERESPONSE_H_ */
