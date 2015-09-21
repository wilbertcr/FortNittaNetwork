/*
 * CEndGameMessage.cpp
 *
 *  Created on: Mar 4, 2015
 *      Author: wsequeira
 */

#include "EndGameMessage.h"

CEndGameMessage::CEndGameMessage(int playerID, int winnerID) {
	DPlayerID = playerID;
	DWinnerID = winnerID;

	DMessage = new char[MAX_MESSAGE_SIZE];
	DPacketFactory = new CNetworkPacketFactory();
	DMessageLength = 0;
	DMessageLength = DPacketFactory->EndGameActionPacket(DMessage, MAX_MESSAGE_SIZE, DPlayerID, DWinnerID);
	DAction = (int16_t) END_GAME;
}

void CEndGameMessage::MakeMessage(){

}

char* CEndGameMessage::GetMessage(){
	return DMessage;
}

const int CEndGameMessage::GetMessageLength(){
	return DMessageLength;
}

CEndGameMessage::~CEndGameMessage() {
	delete DMessage;
	delete DPacketFactory;
}

