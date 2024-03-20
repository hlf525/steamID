#pragma once
#include <iostream>
#include <string.h>
#include <regex>

class SteamID
{
public:

	/**
	 * SteamID universes
	 */
	static const int UNIVERSE_INVALID = 0;
	static const int UNIVERSE_PUBLIC = 1;
	static const int UNIVERSE_BETA = 2;
	static const int UNIVERSE_INTERNAL = 3;
	static const int UNIVERSE_DEV = 4;

	/**
	 * Types of Steam Accounts
	 */
	static const int TYPE_INVALID = 0;
	static const int TYPE_INDIVIDUAL = 1;
	static const int TYPE_MULTISEAT = 2;
	static const int TYPE_GAMESERVER = 3;
	static const int TYPE_ANON_GAMESERVER = 4;
	static const int TYPE_PENDING = 5;
	static const int TYPE_CONTENT_SERVER = 6;
	static const int TYPE_CLAN = 7;
	static const int TYPE_CHAT = 8;
	static const int TYPE_P2P_SUPER_SEEDER = 9;
	static const int TYPE_ANON_USER = 10;

	/**
	 * SteamID account instances
	 */
	static const int INSTANCE_ALL = 0;
	static const int INSTANCE_DESKTOP = 1;
	static const int INSTANCE_CONSOLE = 2;
	static const int INSTANCE_WEB = 4;

	/**
	 * Instance mask is used in chat instance flag calculations, but older versions of PHP don't accept
	 * expressions in const vars so we can't use them here.
	 */
	/*static const int64_t ACCOUNTID_MASK = 0xFFFFFFFF;
	static const int64_t ACCOUNT_INSTANCE_MASK = 0x000FFFFF;*/

	/**
	 * Chat instance flags (check if your instanceid has one of these flags to see if it's that kind of chat)
	 */
	static const int64_t CHAT_INSTANCE_FLAG_CLAN = 0x80000;
	static const int64_t CHAT_INSTANCE_FLAG_LOBBY = 0x40000;
	static const int64_t CHAT_INSTANCE_FLAG_MMSLOBBY = 0x20000;

	/**
	 * Check whether the entered ID is a SteamID
	 */
	SteamID(const std::string& steamid);

	/**
	 * getIdSteam2Rendered(bool newerFormat) 
	 * Gets the rendered STEAM_X:Y:Z format.
	 * @param bool newerFormat If the universe is public, should X be 1 instead of 0?
	 * @return string
	 * @throws Exception If this isn't an individual account SteamID
	 */
	std::string getSteam2RenderedID(bool newerFormat);

	/**
	 * getSteam3RenderedID()
	 * Gets the rendered [T:U:A(:I)] format (T = type, U = universe, A = accountid, I = instance)
	 * @return string
	 */
	std::string getSteam3RenderedID();

	/**
	 * Gets the SteamID as a 64-bit integer
	 * @return string
	 */
	std::string getSteamID64();

	/**
	 * @return bool True if the SteamID is considered "valid", false otherwise
	 */
	bool isValid();

	/**
	 * Returns whether or not this SteamID is for a clan (Steam group) chat
	 * @return bool
	 */
	bool isClanChat();

	/**
	 * Returns whether or not this SteamID is for a lobby
	 * @return bool
	 */
	bool isLobbyChat();

	/**
	 * Returns whether or not this SteamID is for a matchmaking lobby
	 * @return bool
	 */
	bool isMMSLobbyChat();

private:

	int64_t universe;
	int64_t type;
	int64_t instance;
	int64_t accountid;

	static int getTypeFromChar(char);
	static char getCharFromType(int64_t accountType);

};