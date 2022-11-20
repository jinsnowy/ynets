#include "pch.h"
#include "HandshakePacketHandler.h"
#include "UserPacketHandler.h"
#include "Session.h"
#include "TcpNetwork.h"
#include "LoginSessionManager.h"
#include "LoginSession.h"

void HandshakePacketHandler::onClientHello(SessionPtrCRef session, PKT_CLIENT_HELLO* pkt)
{
	// TODO if pkt sessionId is valid still..
	PKT_SERVER_HELLO hello;
	hello.SessionId = session->GetSessionId();
	session->SendAsync(BufferSource::DefaultSerialize(hello));

	// Elaborate to ServerSession
	LoginSessionManager::getInstance()->Authorize(session->GetShared<LoginSession>());
}

void HandshakePacketHandler::onServerHello(SessionPtrCRef session, PKT_SERVER_HELLO* pkt)
{
	session->SetSessionId(pkt->SessionId);
	session->GetNetwork()->UninstallPacketHandler<HandshakePacketHandler>();
	session->GetNetwork()->InstallPacketHandler<UserPacketHandler>();

	LOG_INFO("Set SessionId %llu", session->GetSessionId());
}