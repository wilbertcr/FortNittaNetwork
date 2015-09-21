/*
 * EndGameMessage.h
 *
 *  Created on: Mar 4, 2015
 *      Author: wsequeira
 */

#ifndef END_GAME_MESSAGE_H_
#define END_GAME_MESSAGE_H_

#include "NetworkMessage.h"
#include "../NetworkPacketFactory.h"
#include "../NetworkConstants.h"

#include <string>

class CEndGameMessage: public CNetworkMessage {
public:
	CEndGameMessage(int playerID, int winnerID);
	char* GetMessage();
	const int GetMessageLength();
	virtual ~CEndGameMessage();
protected:
	void MakeMessage();
	CNetworkPacketFactory* DPacketFactory;
	char* DMessage;
	int DMessageLength;
	int DPlayerID;
	int DWinnerID;
};

#endif /* SOURCE_NETWORK_MESSAGES_EndGameMessage_H_ */
