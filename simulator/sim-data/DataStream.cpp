
#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include "SharedFileOut.h"

using namespace std;

template <typename T, unsigned S>
inline unsigned arraysize(const T(&v)[S])
{
    return S;
}

struct SMElement 
{
    HANDLE hMapFile;
    unsigned char* mapFileBuffer;
};

SMElement m_graphics;
SMElement m_physics;
SMElement m_static;

void initPhysics()
{
	TCHAR szName[] = TEXT("Local\\acpmf_physics");
	m_physics.hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SPageFilePhysics), szName);
	if (!m_physics.hMapFile){
		MessageBoxA(GetActiveWindow(), "CreateFileMapping failed", "ACS", MB_OK);
	}
	m_physics.mapFileBuffer = (unsigned char*)MapViewOfFile(m_physics.hMapFile, FILE_MAP_READ, 0, 0, sizeof(SPageFilePhysics));
	if (!m_physics.mapFileBuffer){
		MessageBoxA(GetActiveWindow(), "MapViewOfFile failed", "ACS", MB_OK);
	}
}

void initGraphics() {
    TCHAR szName[] = TEXT("Local\\acpmf_graphics");
    m_graphics.hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SPageFileGraphic), szName);
    if(!m_graphics.hMapFile) {
        MessageBoxA(GetActiveWindow(), "CreateFileMapping failed", "ACS", MB_OK);
    }
    m_graphics.mapFileBuffer = (unsigned char*)MapViewOfFile(m_graphics.hMapFile, FILE_MAP_READ, 0, 0, sizeof(SPageFileGraphic));
    if(!m_graphics.mapFileBuffer) {
        MessageBoxA(GetActiveWindow(), "MapViewOfFile failed", "ACS", MB_OK);
    }
}

void initStatic() {
    TCHAR szName[] = TEXT("Local\\acpmf_static");
    m_static.hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SPageFileStatic), szName);
    if (!m_static.hMapFile) {
		MessageBoxA(GetActiveWindow(), "CreateFileMapping failed", "ACS", MB_OK);
	}
	m_static.mapFileBuffer = (unsigned char*)MapViewOfFile(m_static.hMapFile, FILE_MAP_READ, 0, 0, sizeof(SPageFileStatic));
	if (!m_static.mapFileBuffer) {
		MessageBoxA(GetActiveWindow(), "MapViewOfFile failed", "ACS", MB_OK);
	}
}

HANDLE hSerial;
bool status;

int initSerial() {
	TCHAR comPort[] = TEXT("\\\\.\\COM3");
	hSerial = CreateFile(comPort,
	GENERIC_READ | GENERIC_WRITE,
	0,
	NULL,
	OPEN_EXISTING,
	0,
	NULL);	

	if(hSerial == INVALID_HANDLE_VALUE) {
		printf("# Error in opening serial port");

		return 1;
	} else {
		wcout << "$ Opening serial port successfull" << endl;
		DCB dcbSerialParams = { 0 };
		dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
		status = GetCommState(hSerial, &dcbSerialParams);
		dcbSerialParams.BaudRate = 115200;
		dcbSerialParams.ByteSize = 8;
		dcbSerialParams.StopBits = ONESTOPBIT;
		dcbSerialParams.Parity = NOPARITY;
		SetCommState(hSerial, &dcbSerialParams);

		return 0;
	}

}

void closeHandle(HANDLE handle) {
	CloseHandle(handle);
}

void dismiss(SMElement element) {
    UnmapViewOfFile;
	(element.mapFileBuffer);
    CloseHandle(element.hMapFile);
}

void printData(string name, float value) {
    wcout << name.c_str() << " : " << value << endl;
}

template <typename T, unsigned S>
inline void printData(const string name, const T(&v)[S])
{
	wcout << name.c_str() << " : ";
	for (int i = 0; i < S; i++)
	{
		wcout << v[i];
		if (i < S - 1)
		{
			wcout << " , ";
		}
	}
	wcout << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	initPhysics();
	initGraphics();
	initStatic();
	initSerial();

	//get static data for this instance
	SPageFileStatic* spf = (SPageFileStatic*)m_static.mapFileBuffer;
	wcout << "$ Getting Assetto Data for :" << endl;
	wcout << "$ CAR MODEL: " << spf->carModel << endl;
	wcout << "$ TRACK    : " << spf->track << endl;

	while (true)
	{
		SPageFilePhysics* pf = (SPageFilePhysics*)m_physics.mapFileBuffer;
        //printData("rpms", pf->rpms);

		int rpm = pf->rpms;
		int gear = pf->gear;
		float gas = pf->gas;

		char lpBuffer[50];
		sprintf(lpBuffer, "<%d,%d,%f>", rpm, gear, gas);
		// sprintf(lpBuffer, "%d,%d,%f\n", rpm, gear, gas);
		DWORD dNoOFBytestoWrite;
		DWORD dNoOfBytesWritten = 0;
		dNoOFBytestoWrite = sizeof(lpBuffer);

		bool status = WriteFile(hSerial, 
			lpBuffer,
			dNoOFBytestoWrite,
			&dNoOfBytesWritten,
			NULL);

		// wcout << lpBuffer << endl;
		// printData("speed kmh", pf->speedKmh);
        // Sleep(5);
		// if (GetAsyncKeyState(0x31) != 0) // user pressed 1
		// {
		// 	wcout << "---------------PHYSICS INFO---------------" << endl;
		// 	SPageFilePhysics* pf = (SPageFilePhysics*)m_physics.mapFileBuffer;
		// 	printData("acc G", pf->accG);
		// 	printData("brake", pf->brake);
		// 	printData("camber rad", pf->camberRAD);
		// 	printData("damage", pf->carDamage);
		// 	printData("car height", pf->cgHeight);
		// 	printData("drs", pf->drs);
		// 	printData("tc", pf->tc);
		// 	printData("fuel", pf->fuel);
		// 	printData("gas", pf->gas);
		// 	printData("gear", pf->gear);
		// 	printData("number of tyres out", pf->numberOfTyresOut);
		// 	printData("packet id", pf->packetId);
		// 	printData("heading", pf->heading);
		// 	printData("pitch", pf->pitch);
		// 	printData("roll", pf->roll);
		// 	printData("rpms", pf->rpms);
		// 	printData("speed kmh", pf->speedKmh);
		// 	printData("steer ", pf->steerAngle);
		// 	printData("suspension travel", pf->suspensionTravel);
		// 	printData("tyre core temp", pf->tyreCoreTemperature);
		// 	printData("tyre dirty level", pf->tyreDirtyLevel);
		// 	printData("tyre wear", pf->tyreWear);
		// 	printData("velocity", pf->velocity);
		// 	printData("wheel angular speed", pf->wheelAngularSpeed);
		// 	printData("wheel load", pf->wheelLoad);
		// 	printData("wheel slip", pf->wheelSlip);
		// 	printData("wheel pressure", pf->wheelsPressure);
		// }

		// if (GetAsyncKeyState(0x32) != 0) // user pressed 2
		// {
		// 	wcout << "---------------GRAPHICS INFO---------------" << endl;
		// 	SPageFileGraphic* pf = (SPageFileGraphic*)m_graphics.mapFileBuffer;
		// 	printData("packetID ", pf->packetId);
		// 	printData("STATUS ", pf->status);
		// 	printData("session", pf->session);
		// 	printData("completed laps", pf->completedLaps);
		// 	printData("position", pf->position);
		// 	printData("current time s", pf->currentTime);
		// 	printData("current time", pf->iCurrentTime);
		// 	printData("last time", pf->iLastTime);
		// 	printData("best time", pf->iBestTime);
		// 	printData("sessionTimeLeft", pf->sessionTimeLeft);
		// 	printData("distanceTraveled", pf->distanceTraveled);
		// 	printData("isInPit", pf->isInPit);
		// 	printData("currentSectorIndex", pf->currentSectorIndex);
		// 	printData("lastSectorTime", pf->lastSectorTime);
		// 	printData("numberOfLaps", pf->numberOfLaps);
		// 	wcout << "TYRE COMPOUND : " << pf->tyreCompound << endl;
		// 	printData("replayMult", pf->replayTimeMultiplier);
		// 	printData("normalizedCarPosition", pf->normalizedCarPosition);
		// 	printData("carCoordinates", pf->carCoordinates);
		// }


		// if (GetAsyncKeyState(0x33) != 0) // user pressed 3
		// {
		// 	wcout << "---------------STATIC INFO---------------" << endl;
		// 	SPageFileStatic* pf = (SPageFileStatic*)m_static.mapFileBuffer;
		// 	wcout << "SM VERSION " << pf->smVersion << endl;
		// 	wcout << "AC VERSION " << pf->acVersion << endl;

		// 	printData("number of sessions ", pf->numberOfSessions);
		// 	printData("numCars", pf->numCars);
		// 	wcout << "Car model " << pf->carModel << endl;
		// 	wcout << "Car track " << pf->track << endl;
		// 	wcout << "Player Name " << pf->playerName << endl;
		// 	// wcout << "Player Name " << pf->playerNick << endl;
		// 	// wcout << "Player Name " << pf->playerSurname << endl;
		// 	printData("sectorCount", pf->sectorCount);

		// 	printData("maxTorque", pf->maxTorque);
		// 	printData("maxPower", pf->maxPower);
		// 	printData("maxRpm", pf->maxRpm);
		// 	printData("maxFuel", pf->maxFuel);
		// 	printData("suspensionMaxTravel", pf->suspensionMaxTravel);
		// 	printData("tyreRadius", pf->tyreRadius);

		// }
	}

	closeHandle(hSerial);

	dismiss(m_graphics);
	dismiss(m_physics);
	dismiss(m_static);

	return 0;
}