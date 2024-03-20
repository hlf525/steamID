#include "steamID.h"
const char TYPECHARS[] = { 'I', 'U', 'M', 'G', 'A', 'P', 'C', 'g', 'T', 'a' };

SteamID::SteamID(const std::string& steamid)
{
	if (steamid.empty())
	{
		this->universe = UNIVERSE_INVALID;
		this->type = TYPE_INVALID;
		this->instance = INSTANCE_ALL;
		this->accountid = 0;
		return;
	}

	// steam2id
	std::regex steam2pattern("^STEAM_([0-5]):([0-1]):([0-9]+)$");

	// steam3id
	std::regex steam3pattern("^\\[([a-zA-Z]):([0-5]):([0-9]+)(:[0-9]+)?\\]$");

	// steammatch store the match result 
	std::smatch steammatch;

	if (std::regex_match(steamid, steammatch, steam2pattern))
	{
		if (steammatch.size() < 4)
		{
			return;
		}
		// steam2id
		// steam2match[0]: steamid
		// steam2match[1]: universe type [0-5]
		// steam2match[2]: instance type [0-1]
		// steam2match[3]: account id [0-9]+
		if (std::stoi(steammatch[1].str()) <= UNIVERSE_PUBLIC)
		{
			this->universe = UNIVERSE_PUBLIC;
		}
		else
		{
			this->universe = std::stoi(steammatch[1].str());
		}
		this->type = TYPE_INDIVIDUAL;
		this->instance = INSTANCE_DESKTOP; 
		this->accountid = (std::stoi(steammatch[3].str()) * 2) + std::stoi(steammatch[2].str());
		return;
	}
	else if (std::regex_match(steamid, steammatch, steam3pattern))
	{
		if (steammatch.size() < 5)
		{
			return;
		}
		// steam3id
		// matches[0]: steamid
		// matches[1]: type char
		// matches[2]: universe type
		// matches[3]: account id
		// matches[4]: instance type
		this->universe = std::stoi(steammatch[2].str());
		this->accountid = std::stoi(steammatch[3].str());
		
		std::string type_char = steammatch[1].str();
		if (!steammatch[4].str().empty())
		{
			this->instance = std::stoi(steammatch[4].str().substr(1));
		}
		else
		{
			if (type_char == "g" || type_char == "c" || type_char == "T" || type_char == "L")
			{
				this->instance = INSTANCE_ALL;
			}
			this->instance = INSTANCE_DESKTOP; 
		}

		if (type_char == "c")
		{
			this->instance |= CHAT_INSTANCE_FLAG_CLAN;
			this->type = TYPE_CHAT;
		}
		else if (type_char == "L")
		{
			this->instance |= CHAT_INSTANCE_FLAG_LOBBY;
			this->type = TYPE_CHAT;
		}
		else
		{
			this->type = getTypeFromChar(type_char[0]);
		}
	}
	else
	{
		// SteamID64
		int64_t id = std::stoll(steamid);
		this->universe = id >> 56;
		this->type = (id >> 52) & 0xF;
		this->instance = (id >> 32) & 0xFFFFF;
		this->accountid = id & 0xFFFFFFFF;
	}
}

/**
 * Gets the rendered STEAM_X:Y:Z format.
 */
std::string SteamID::getSteam2RenderedID(bool newerFormat = false)
{
	if (this->type != TYPE_INDIVIDUAL)
	{
		throw std::runtime_error("this isn't an individual account SteamID");
	}
	
	int64_t _universe = this->universe;
	if (_universe == 1 && !newerFormat)
	{
		_universe = 0;
	}

	// Check if accountid is odd or even by checking the least significant bit 
	int64_t oddeven = this->accountid & 1;

	// Divide the accountid by 2 and floor the result
	int64_t halfaccountid = floor(this->accountid / 2);

	std::string steamid2;
	//steamid2 = "STEAM_" + std::to_string(_universe) + ":" + std::to_string(oddeven) + ":" + std::to_string(halfaccountid);
	steamid2.append("STEAM_").append(std::to_string(_universe)).append(":").append(std::to_string(oddeven)).append(":").append(std::to_string(halfaccountid));

	return steamid2;
}

/**
 * Gets the rendered [T:U:A(:I)] format (T = type, U = universe, A = accountid, I = instance)
 */
std::string SteamID::getSteam3RenderedID()
{
	char type_char = getCharFromType(this->type) != '\0' ? getCharFromType(this->type) : 'i';

	if (this->instance & CHAT_INSTANCE_FLAG_CLAN)
	{
		type_char = 'c';
	}
	else if (this->instance & CHAT_INSTANCE_FLAG_LOBBY)
	{
		type_char = 'L';
	}
	
	bool render_instance = (this->type == TYPE_ANON_GAMESERVER || this->type == TYPE_MULTISEAT || (this->type == TYPE_INDIVIDUAL && this->instance != INSTANCE_DESKTOP));

	std::string steamid3;
	//steamid3 = "[" + std::string(sizeof(type_char), type_char) + ":" + std::to_string(this->universe) + ":" + std::to_string(this->accountid) + ":" + (render_instance ? ":" + std::to_string(this->instance) : "") + "]";
	steamid3.append("[").append(std::string(sizeof(type_char), type_char)).append(":").append(std::to_string(this->universe)).append(":").append(std::to_string(this->accountid));
	
	if (render_instance)
	{
		steamid3.append(":").append(std::to_string(this->instance));
	}
	steamid3.append("]");

	return steamid3;
}

std::string SteamID::getSteamID64()
{
	return std::to_string(((this->universe << 56) | (this->type << 52) | (this->instance << 32) | (this->accountid)));
}

bool SteamID::isValid()
{
	if (this->type <= TYPE_INVALID || this->type > TYPE_ANON_USER)
	{
		return false;
	}
	if (this->universe <= UNIVERSE_INVALID || this->universe > UNIVERSE_DEV)
	{
		return false;
	}
	if (this->type == TYPE_INDIVIDUAL && (this->accountid == 0 || this->instance > INSTANCE_WEB))
	{
		return false;
	}
	if (this->type == TYPE_CLAN && (this->accountid == 0 || this->instance != INSTANCE_ALL))
	{
		return false;
	}
	if (this->type == TYPE_GAMESERVER && this->accountid == 0)
	{
		return false;
	}
	return true;
}

bool SteamID::isClanChat()
{
	return this->type == TYPE_CHAT && (this->instance & CHAT_INSTANCE_FLAG_CLAN);
}

bool SteamID::isLobbyChat()
{
	return this->type == TYPE_CHAT && (this->instance & CHAT_INSTANCE_FLAG_LOBBY);
}

bool SteamID::isMMSLobbyChat()
{
	return this->type == TYPE_CHAT && (this->instance & CHAT_INSTANCE_FLAG_MMSLOBBY);
}

int SteamID::getTypeFromChar(char Type)
{
	for (int i = 0; i < sizeof(TYPECHARS); i++)
	{
		if (Type == TYPECHARS[i])
		{
			return i;
		}
	}
	return TYPE_INVALID;
}

char SteamID::getCharFromType(int64_t accountType)
{
	if (accountType < 0 || accountType > sizeof(accountType))
	{
		return '\0';
	}
	return TYPECHARS[accountType];
}
