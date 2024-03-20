#include "steamID.h"
#include <iostream>
using namespace std;

int main()
{
	cout << "--------STEAM64---------" << endl;
	SteamID* s1 = new SteamID("76561199188156301");
	cout << "STEAM64: " << s1->getSteamID64() << endl;
	cout << "STEAM2: " << s1->getSteam2RenderedID(false) << endl;
	cout << "STEAM3: " << s1->getSteam3RenderedID() << endl;

	cout << "--------STEAM2---------" << endl;
	SteamID* s2 = new SteamID("STEAM_0:1:613945286");
	cout << "STEAM64: " << s1->getSteamID64() << endl;
	cout << "STEAM2: " << s1->getSteam2RenderedID(false) << endl;
	cout << "STEAM3: " << s1->getSteam3RenderedID() << endl;

	cout << "--------STEAM3---------" << endl;
	SteamID* s3 = new SteamID("[U:1:1227890573]");
	cout << "STEAM64: " << s1->getSteamID64() << endl;
	cout << "STEAM2: " << s1->getSteam2RenderedID(false) << endl;
	cout << "STEAM3: " << s1->getSteam3RenderedID() << endl;

}