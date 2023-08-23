#pragma once
#include <cstdint>
#include <cstddef>

// 2023-06-07 07:51:01.337255200 UTC

namespace hazedumper {
	constexpr ::std::int64_t timestamp = 1686124261;
	namespace netvars {
		constexpr ::std::ptrdiff_t cs_gamerules_data = 0x0;
		constexpr ::std::ptrdiff_t m_ArmorValue = 0x117CC;
		constexpr ::std::ptrdiff_t m_Collision = 0x320;
		constexpr ::std::ptrdiff_t m_CollisionGroup = 0x474;
		constexpr ::std::ptrdiff_t m_Local = 0x2FCC;
		constexpr ::std::ptrdiff_t m_MoveType = 0x25C;
		constexpr ::std::ptrdiff_t m_OriginalOwnerXuidHigh = 0x31D4;
		constexpr ::std::ptrdiff_t m_OriginalOwnerXuidLow = 0x31D0;
		constexpr ::std::ptrdiff_t m_SurvivalGameRuleDecisionTypes = 0x1328;
		constexpr ::std::ptrdiff_t m_SurvivalRules = 0xD00;
		constexpr ::std::ptrdiff_t m_aimPunchAngle = 0x303C;
		constexpr ::std::ptrdiff_t m_aimPunchAngleVel = 0x3048;
		constexpr ::std::ptrdiff_t m_angEyeAnglesX = 0x117D0;
		constexpr ::std::ptrdiff_t m_angEyeAnglesY = 0x117D4;
		constexpr ::std::ptrdiff_t m_bBombDefused = 0x29C0;
		constexpr ::std::ptrdiff_t m_bBombPlanted = 0x9A5;
		constexpr ::std::ptrdiff_t m_bBombTicking = 0x2990;
		constexpr ::std::ptrdiff_t m_bFreezePeriod = 0x20;
		constexpr ::std::ptrdiff_t m_bGunGameImmunity = 0x9990;
		constexpr ::std::ptrdiff_t m_bHasDefuser = 0x117DC;
		constexpr ::std::ptrdiff_t m_bHasHelmet = 0x117C0;
		constexpr ::std::ptrdiff_t m_bInReload = 0x32B5;
		constexpr ::std::ptrdiff_t m_bIsDefusing = 0x997C;
		constexpr ::std::ptrdiff_t m_bIsQueuedMatchmaking = 0x74;
		constexpr ::std::ptrdiff_t m_bIsScoped = 0x9974;
		constexpr ::std::ptrdiff_t m_bIsValveDS = 0x7C;
		constexpr ::std::ptrdiff_t m_bSpotted = 0x93D;
		constexpr ::std::ptrdiff_t m_bSpottedByMask = 0x980;
		constexpr ::std::ptrdiff_t m_bStartedArming = 0x3400;
		constexpr ::std::ptrdiff_t m_bUseCustomAutoExposureMax = 0x9D9;
		constexpr ::std::ptrdiff_t m_bUseCustomAutoExposureMin = 0x9D8;
		constexpr ::std::ptrdiff_t m_bUseCustomBloomScale = 0x9DA;
		constexpr ::std::ptrdiff_t m_clrRender = 0x70;
		constexpr ::std::ptrdiff_t m_dwBoneMatrix = 0x26A8;
		constexpr ::std::ptrdiff_t m_fAccuracyPenalty = 0x3340;
		constexpr ::std::ptrdiff_t m_fFlags = 0x104;
		constexpr ::std::ptrdiff_t m_flC4Blow = 0x29A0;
		constexpr ::std::ptrdiff_t m_flCustomAutoExposureMax = 0x9E0;
		constexpr ::std::ptrdiff_t m_flCustomAutoExposureMin = 0x9DC;
		constexpr ::std::ptrdiff_t m_flCustomBloomScale = 0x9E4;
		constexpr ::std::ptrdiff_t m_flDefuseCountDown = 0x29BC;
		constexpr ::std::ptrdiff_t m_flDefuseLength = 0x29B8;
		constexpr ::std::ptrdiff_t m_flFallbackWear = 0x31E0;
		constexpr ::std::ptrdiff_t m_flFlashDuration = 0x10470;
		constexpr ::std::ptrdiff_t m_flFlashMaxAlpha = 0x1046C;
		constexpr ::std::ptrdiff_t m_flLastBoneSetupTime = 0x2928;
		constexpr ::std::ptrdiff_t m_flLowerBodyYawTarget = 0x9ADC;
		constexpr ::std::ptrdiff_t m_flNextAttack = 0x2D80;
		constexpr ::std::ptrdiff_t m_flNextPrimaryAttack = 0x3248;
		constexpr ::std::ptrdiff_t m_flSimulationTime = 0x268;
		constexpr ::std::ptrdiff_t m_flTimerLength = 0x29A4;
		constexpr ::std::ptrdiff_t m_hActiveWeapon = 0x2F08;
		constexpr ::std::ptrdiff_t m_hBombDefuser = 0x29C4;
		constexpr ::std::ptrdiff_t m_hMyWeapons = 0x2E08;
		constexpr ::std::ptrdiff_t m_hObserverTarget = 0x339C;
		constexpr ::std::ptrdiff_t m_hOwner = 0x29DC;
		constexpr ::std::ptrdiff_t m_hOwnerEntity = 0x14C;
		constexpr ::std::ptrdiff_t m_hViewModel = 0x3308;
		constexpr ::std::ptrdiff_t m_iAccountID = 0x2FD8;
		constexpr ::std::ptrdiff_t m_iClip1 = 0x3274;
		constexpr ::std::ptrdiff_t m_iCompetitiveRanking = 0x1A84;
		constexpr ::std::ptrdiff_t m_iCompetitiveWins = 0x1B88;
		constexpr ::std::ptrdiff_t m_iCrosshairId = 0x11838;
		constexpr ::std::ptrdiff_t m_iDefaultFOV = 0x333C;
		constexpr ::std::ptrdiff_t m_iEntityQuality = 0x2FBC;
		constexpr ::std::ptrdiff_t m_iFOV = 0x31F4;
		constexpr ::std::ptrdiff_t m_iFOVStart = 0x31F8;
		constexpr ::std::ptrdiff_t m_iGlowIndex = 0x10488;
		constexpr ::std::ptrdiff_t m_iHealth = 0x100;
		constexpr ::std::ptrdiff_t m_iItemDefinitionIndex = 0x2FBA;
		constexpr ::std::ptrdiff_t m_iItemIDHigh = 0x2FD0;
		constexpr ::std::ptrdiff_t m_iMostRecentModelBoneCounter = 0x2690;
		constexpr ::std::ptrdiff_t m_iObserverMode = 0x3388;
		constexpr ::std::ptrdiff_t m_iShotsFired = 0x103E0;
		constexpr ::std::ptrdiff_t m_iState = 0x3268;
		constexpr ::std::ptrdiff_t m_iTeamNum = 0xF4;
		constexpr ::std::ptrdiff_t m_lifeState = 0x25F;
		constexpr ::std::ptrdiff_t m_nBombSite = 0x2994;
		constexpr ::std::ptrdiff_t m_nFallbackPaintKit = 0x31D8;
		constexpr ::std::ptrdiff_t m_nFallbackSeed = 0x31DC;
		constexpr ::std::ptrdiff_t m_nFallbackStatTrak = 0x31E4;
		constexpr ::std::ptrdiff_t m_nForceBone = 0x268C;
		constexpr ::std::ptrdiff_t m_nModelIndex = 0x258;
		constexpr ::std::ptrdiff_t m_nTickBase = 0x3440;
		constexpr ::std::ptrdiff_t m_nViewModelIndex = 0x29D0;
		constexpr ::std::ptrdiff_t m_rgflCoordinateFrame = 0x444;
		constexpr ::std::ptrdiff_t m_szCustomName = 0x304C;
		constexpr ::std::ptrdiff_t m_szLastPlaceName = 0x35C4;
		constexpr ::std::ptrdiff_t m_thirdPersonViewAngles = 0x31E8;
		constexpr ::std::ptrdiff_t m_vecOrigin = 0x138;
		constexpr ::std::ptrdiff_t m_vecVelocity = 0x114;
		constexpr ::std::ptrdiff_t m_vecViewOffset = 0x108;
		constexpr ::std::ptrdiff_t m_viewPunchAngle = 0x3030;
		constexpr ::std::ptrdiff_t m_zoomLevel = 0x33E0;
	} // namespace netvars
}

using namespace hazedumper;