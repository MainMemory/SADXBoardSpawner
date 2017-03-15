// SADXBoardSpawner.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "..\sadx-mod-loader\SADXModLoader\include\SADXModLoader.h"

ObjectMaster *snowboard;

ObjectMaster *LoadSnowboardObject(LoadObj flags, char index, ObjectFuncPtr loadSub)
{
	return snowboard = LoadObject(flags, index, loadSub);
}

void __cdecl Snowboard_Delete_r(ObjectMaster *obj)
{
	njReleaseTexture((NJS_TEXLIST *)obj->Data1->LoopData);
	if (obj == snowboard)
		snowboard = nullptr;
}

DataPointer(int, PressedButtons, 0x3B0E354);
extern "C"
{
	__declspec(dllexport) void __cdecl OnFrame()
	{
		if (GameMode == GameModes_Menu || CurrentLevel == LevelIDs_SkyChase1 || CurrentLevel == LevelIDs_SkyChase2 || !GetCharacterObject(0))
			return;
		ObjectMaster *obj = GetCharacterObject(0);
		if (PressedButtons & Buttons_Z)
			switch (obj->Data1->CharID)
			{
			case Characters_Sonic:
				if (obj->Data1->Action >= 62 && obj->Data1->Action <= 68)
					ForcePlayerAction(0, 24);
				else
				{
					ForcePlayerAction(0, 44);
					if (!snowboard)
						snowboard = LoadObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), 2, Snowboard_Sonic_Load);
				}
				break;
			case Characters_Tails:
				if (obj->Data1->Action >= 48 && obj->Data1->Action <= 54)
				{
					ForcePlayerAction(0, 24);
					// fix a bug where Tails' physics data doesn't get reset to normal
					((EntityData2*)obj->Data2)->CharacterData->PhysicsData.CollisionSize = PhysicsArray[Characters_Tails].CollisionSize;
					((EntityData2*)obj->Data2)->CharacterData->PhysicsData.YOff = PhysicsArray[Characters_Tails].YOff;
					((EntityData2*)obj->Data2)->CharacterData->PhysicsData.JumpSpeed = PhysicsArray[Characters_Tails].JumpSpeed;
				}
				else
				{
					ForcePlayerAction(0, 44);
					if (!snowboard)
						snowboard = LoadObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), 2, Snowboard_Tails_Load);
				}
				break;
			}
		else if (!snowboard)
			switch (obj->Data1->CharID)
			{
			case Characters_Sonic:
				if (obj->Data1->Action >= 62 && obj->Data1->Action <= 68)
					snowboard = LoadObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), 2, Snowboard_Sonic_Load);
				break;
			case Characters_Tails:
				if (obj->Data1->Action >= 48 && obj->Data1->Action <= 54)
					snowboard = LoadObject((LoadObj)(LoadObj_Data1 | LoadObj_Data2), 2, Snowboard_Tails_Load);
				break;
			}
	}

	__declspec(dllexport) void Init(const char *path, const HelperFunctions &helperFunctions)
	{
		WriteCall((void*)0x4E9423, LoadSnowboardObject);
		WriteCall((void*)0x4E967E, LoadSnowboardObject);
		WriteCall((void*)0x4E9698, LoadSnowboardObject);
		WriteCall((void*)0x597B34, LoadSnowboardObject);
		WriteCall((void*)0x597B46, LoadSnowboardObject);
		WriteJump(Snowboard_Delete, Snowboard_Delete_r);
	}

	__declspec(dllexport) ModInfo SADXModInfo { ModLoaderVer };
}