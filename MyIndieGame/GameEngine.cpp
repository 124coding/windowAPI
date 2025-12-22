#pragma once

#include "GameEngine.h"
#include "GameObject.h"

#include "CFMOD.h"

#include "CRenderer.h"

#include "CInputMgr.h"
#include "CSceneMgr.h"
#include "CResourceMgr.h"
#include "CCollisionMgr.h"
#include "CUIMgr.h"
#include "CMonsterSpawnMgr.h"
#include "CDataMgr.h"
#include "CEffectMgr.h"

#include "CPlayScene.h"
#include "CTitleScene.h"
#include "CEndingScene.h"
#include "CToolScene.h"
#include "CSettingScene.h"

#include "SVector2D.h"
#include "time.h"

class CBabyAlien;

void GameEngine::OnCreate() {
	int a = 0;
	srand((unsigned int)(&a));

	mainEngine = this;

	CDataMgr::OnCreate();
	LoadResources();
	CMonsterSpawnMgr::OnCreate();
	// CFMOD::OnCreate();
	CCollisionMgr::OnCreate();
	CUIMgr::OnCreate();
	CSceneMgr::OnCreate();
	LoadScenes();

	CInputMgr::GetInst()->AddKeyInfo("DoMoveLt", 'A');
	CInputMgr::GetInst()->AddKeyInfo("DoMoveRt", 'D');
	CInputMgr::GetInst()->AddKeyInfo("DoMoveFt", 'W');
	CInputMgr::GetInst()->AddKeyInfo("DoMoveBt", 'S');
	CInputMgr::GetInst()->AddKeyInfo("DoSave", 'K');
	CInputMgr::GetInst()->AddKeyInfo("DoLoad", 'L');
	CInputMgr::GetInst()->AddKeyInfo("MouseLeftClick", VK_LBUTTON);
	CInputMgr::GetInst()->AddKeyInfo("MouseRightClick", VK_RBUTTON);
	CInputMgr::GetInst()->AddKeyInfo("MouseWheelClick", VK_MBUTTON);
}

void GameEngine::OnDestroy() {
	DestroyResources();
	DestroyScenes();
	CUIMgr::OnDestroy();
	CEffectMgr::OnDestroy();
	CCollisionMgr::OnDestroy();
	CMonsterSpawnMgr::OnDestroy();
	CDataMgr::OnDestroy();
}

void GameEngine::OnUpdate(float tDeltaTime) {
	CUIMgr::OnUpdate(tDeltaTime);
	CEffectMgr::OnUpdate(tDeltaTime);
	CCollisionMgr::OnUpdate(tDeltaTime);
	CSceneMgr::OnUpdate(tDeltaTime);
	RemoveDeadObjects();

	Render();
}

void GameEngine::OnLateUpdate(float tDeltaTime) {
	CUIMgr::OnLateUpdate(tDeltaTime);
	CEffectMgr::OnLateUpdate(tDeltaTime);
	CCollisionMgr::OnLateUpdate(tDeltaTime);
	CSceneMgr::OnLateUpdate(tDeltaTime);
}

void GameEngine::Render() {
	this->Clear(0.1f, 0.0f, 0.1f);
	CSceneMgr::Render(mBackBuffer->GetDCMem());
	CCollisionMgr::Render(mBackBuffer->GetDCMem());
	CEffectMgr::Render(mBackBuffer->GetDCMem());
	CUIMgr::Render(mBackBuffer->GetDCMem());

	this->Present();
}

void GameEngine::LoadScenes()
{
	CSceneMgr::CreateScene<CPlayScene>(L"PlayScene");
	CSceneMgr::CreateScene<CTitleScene>(L"TitleScene");
	CSceneMgr::CreateScene<CSettingScene>(L"SettingScene");
	CSceneMgr::CreateScene<CEndingScene>(L"EndingScene");
	CSceneMgr::CreateScene <CToolScene>(L"ToolScene");
	

	CSceneMgr::LoadScene(L"SettingScene");
}

void GameEngine::LoadResources()
{
	// Font
	AddFontResourceEx(L"..Resources/Font/NotoSansKR-Medium.otf", FR_PRIVATE, 0);

	// TitleScene
	CResourceMgr::Load<CTexture>(L"TitleMap", L"../resources/Maps/Title/Brotato_Title_Screen_Map.png");
	CResourceMgr::Load<CTexture>(L"TitleMob1", L"../resources/Maps/Title/Brotato_Title_Screen_Mobs_1.png");
	CResourceMgr::Load<CTexture>(L"TitleMob2", L"../resources/Maps/Title/Brotato_Title_Screen_Mobs_2.png");
	CResourceMgr::Load<CTexture>(L"TitleMob3", L"../resources/Maps/Title/Brotato_Title_Screen_Mobs_3.png");
	CResourceMgr::Load<CTexture>(L"TitleLight", L"../resources/Maps/Title/Brotato_Title_Screen_Light.png");
	CResourceMgr::Load<CTexture>(L"TitleLogo", L"../resources/Maps/Title/Logo.png");
	CResourceMgr::Load<CTexture>(L"TitleBrotato", L"../resources/Maps/Title/splash_art_brotato.png");

	// SettingScene
	CResourceMgr::Load<CTexture>(L"SettingMap", L"../resources/UI/shop_background.png")->CreateHBitmapFromGdiPlus(false);



	// Enemies
	LoadMonster(L"BabyAlien", L"../resources/Enemy/BabyAlien/baby_alien.png");

	CResourceMgr::Load<CTexture>(L"EnemyBullet", L"../resources/Enemy/EnemyBullet/enemy_bullet.png");
	CTexture* entityBirth = CResourceMgr::Load<CTexture>(L"EntityBirth", L"../resources/Enemy/EnemyBirth/entity_birth.png");

	CTexture* enemyBirth = CTexture::Create(L"EnemyBirth", entityBirth->GetBaseWidth(), entityBirth->GetBaseHeight());
	CTexture* treeBirth = CTexture::Create(L"TreeBirth", entityBirth->GetBaseWidth(), entityBirth->GetBaseHeight());

	CTexture::ApplyOtherColorToWantedAreas(30, 255, 255.0f, 0.0f, 0.0f, enemyBirth->GetImage(), entityBirth->GetImage());
	CTexture::ApplyOtherColorToWantedAreas(30, 255, 0.0f, 255.0f, 0.0f, treeBirth->GetImage(), entityBirth->GetImage());

	// Player
	CResourceMgr::Load<CTexture>(L"PlayerBase", L"../resources/Player/potato.png")->BakedTex(25.0f, 80.0f, 100.0f, 50.0f, CResourceMgr::Load<CTexture>(L"PlayerLegs", L"../resources/Player/legs.png")->GetImage());


	// Items
	// Acid
	CResourceMgr::Load<CTexture>(L"산성", L"../resources/Items/All/Alien_Magic/alien_magic.png");
	CResourceMgr::Load<CTexture>(L"산성Icon", L"../resources/Items/All/Alien_Magic/alien_magic_icon.png");

	// Alien_Magic
	CResourceMgr::Load<CTexture>(L"외계인 마법", L"../resources/Items/All/Acid/acid.png");
	CResourceMgr::Load<CTexture>(L"외계인 마법Icon", L"../resources/Items/All/Acid/acid_icon.png");

	// Alloy
	CResourceMgr::Load<CTexture>(L"합금", L"../resources/Items/All/Alloy/alloy.png");
	CResourceMgr::Load<CTexture>(L"합금Icon", L"../resources/Items/All/Alloy/alloy_icon.png");

	// Bag
	CResourceMgr::Load<CTexture>(L"가방", L"../resources/Items/All/Bag/bag.png");
	CResourceMgr::Load<CTexture>(L"가방Icon", L"../resources/Items/All/Bag/bag_icon.png");

	// Beanie
	CResourceMgr::Load<CTexture>(L"비니", L"../resources/Items/All/Beanie/beanie.png");
	CResourceMgr::Load<CTexture>(L"비니Icon", L"../resources/Items/All/Beanie/beanie_icon.png");

	// Blindfold
	CResourceMgr::Load<CTexture>(L"눈가리개", L"../resources/Items/All/Blindfold/blindfold.png");
	CResourceMgr::Load<CTexture>(L"눈가리개Icon", L"../resources/Items/All/Blindfold/blindfold_icon.png");

	// Blood_Leech
	CResourceMgr::Load<CTexture>(L"거머리", L"../resources/Items/All/Blood_Leech/blood_leech.png");
	CResourceMgr::Load<CTexture>(L"거머리Icon", L"../resources/Items/All/Blood_Leech/blood_leech_icon.png");

	// Broken_Mouth
	CResourceMgr::Load<CTexture>(L"깨진 입", L"../resources/Items/All/Broken_Mouth/broken_mouth.png");
	CResourceMgr::Load<CTexture>(L"깨진 입Icon", L"../resources/Items/All/Broken_Mouth/broken_mouth_icon.png");

	// Cake
	CResourceMgr::Load<CTexture>(L"케이크", L"../resources/Items/All/Cake/cake.png");
	CResourceMgr::Load<CTexture>(L"케이크Icon", L"../resources/Items/All/Cake/cake_icon.png");

	// Claw_Tree
	CResourceMgr::Load<CTexture>(L"발톱 나무Icon", L"../resources/Items/All/Claw_Tree/claw_tree_icon.png");
	 
	// Coffee
	CResourceMgr::Load<CTexture>(L"커피", L"../resources/Items/All/Coffee/coffee.png");
	CResourceMgr::Load<CTexture>(L"커피Icon", L"../resources/Items/All/Coffee/coffee_icon.png");

	// Coupon
	CResourceMgr::Load<CTexture>(L"쿠폰Icon", L"../resources/Items/All/Coupon/coupon_icon.png");

	// Cyclops_Worm
	CResourceMgr::Load<CTexture>(L"외눈박이 벌레", L"../resources/Items/All/Cyclops_Worm/cyclops_worm.png");
	CResourceMgr::Load<CTexture>(L"외눈박이 벌레Icon", L"../resources/Items/All/Cyclops_Worm/cyclops_worm_icon.png");

	// Dangerous_Bunny
	CResourceMgr::Load<CTexture>(L"위험한 토끼", L"../resources/Items/All/Dangerous_Bunny/dangerous_bunny.png");
	CResourceMgr::Load<CTexture>(L"위험한 토끼Icon", L"../resources/Items/All/Dangerous_Bunny/dangerous_bunny_icon.png");

	// Defective_Steroids
	CResourceMgr::Load<CTexture>(L"불량 스테로이드Icon", L"../resources/Items/All/Defective_Steroids/defective_steroids_icon.png");

	// Exoskeleton
	CResourceMgr::Load<CTexture>(L"외골격Icon", L"../resources/Items/All/Exoskeleton/exoskeleton_icon.png");

	// Fin
	CResourceMgr::Load<CTexture>(L"지느러미", L"../resources/Items/All/Fin/fin.png");
	CResourceMgr::Load<CTexture>(L"지느러미Icon", L"../resources/Items/All/Fin/fin_icon.png");

	// Fresh_Meat
	CResourceMgr::Load<CTexture>(L"신선한 고기", L"../resources/Items/All/Fresh_Meat/fresh_meat.png");
	CResourceMgr::Load<CTexture>(L"신선한 고기Icon", L"../resources/Items/All/Fresh_Meat/fresh_meat_icon.png");

	// Gambling_Token
	CResourceMgr::Load<CTexture>(L"갬블링 토큰Icon", L"../resources/Items/All/Gambling_Token/gambling_token_icon.png");

	// Ghost_Outfit
	CResourceMgr::Load<CTexture>(L"유령 의상", L"../resources/Items/All/Ghost_Outfit/ghost_outfit.png");
	CResourceMgr::Load<CTexture>(L"유령 의상Icon", L"../resources/Items/All/Ghost_Outfit/ghost_outfit_icon.png");

	// Glass_Cannon
	CResourceMgr::Load<CTexture>(L"유리 대포Icon", L"../resources/Items/All/Glass_Cannon/glass_cannon_icon.png");

	// Glasses
	CResourceMgr::Load<CTexture>(L"안경", L"../resources/Items/All/Glasses/glasses.png");
	CResourceMgr::Load<CTexture>(L"안경Icon", L"../resources/Items/All/Glasses/glasses_icon.png");

	// Goat_Skull
	CResourceMgr::Load<CTexture>(L"염소 두개골Icon", L"../resources/Items/All/Goat_Skull/goat_skull_icon.png");

	// Gummy_Berserker
	CResourceMgr::Load<CTexture>(L"진득한 버서커", L"../resources/Items/All/Gummy_Berserker/gummy_berserker.png");
	CResourceMgr::Load<CTexture>(L"진득한 버서커Icon", L"../resources/Items/All/Gummy_Berserker/gummy_berserker_icon.png");

	// Head_Injury
	CResourceMgr::Load<CTexture>(L"머리 부상", L"../resources/Items/All/Head_Injury/head_injury.png");
	CResourceMgr::Load<CTexture>(L"머리 부상Icon", L"../resources/Items/All/Head_Injury/head_injury_icon.png");

	// Hedgehog
	CResourceMgr::Load<CTexture>(L"고슴도치", L"../resources/Items/All/Hedgehog/hedgehog.png");
	CResourceMgr::Load<CTexture>(L"고슴도치Icon", L"../resources/Items/All/Hedgehog/hedgehog_icon.png");

	// Injection
	CResourceMgr::Load<CTexture>(L"주입Icon", L"../resources/Items/All/Injection/injection_icon.png");

	// Insanity
	CResourceMgr::Load<CTexture>(L"광기", L"../resources/Items/All/Insanity/insanity.png");
	CResourceMgr::Load<CTexture>(L"광기Icon", L"../resources/Items/All/Insanity/insanity_icon.png");

	// Jetpack
	CResourceMgr::Load<CTexture>(L"제트팩Icon", L"../resources/Items/All/Jetpack/jetpack_icon.png");

	// Leather_Vest
	CResourceMgr::Load<CTexture>(L"가죽 조끼", L"../resources/Items/All/Leather_Vest/leather_vest.png");
	CResourceMgr::Load<CTexture>(L"가죽 조끼Icon", L"../resources/Items/All/Leather_Vest/leather_vest_icon.png");

	// Lens
	CResourceMgr::Load<CTexture>(L"렌즈", L"../resources/Items/All/Lens/lens.png");
	CResourceMgr::Load<CTexture>(L"렌즈Icon", L"../resources/Items/All/Lens/lens_icon.png");

	// Little_Muscley_Dude_Icon
	CResourceMgr::Load<CTexture>(L"작은 근육질의 친구Icon", L"../resources/Items/All/Little_Muscley_Dude_Icon/little_muscley_dude_icon.png");

	// Mammoth
	CResourceMgr::Load<CTexture>(L"맘모스", L"../resources/Items/All/Mammoth/mammoth.png");
	CResourceMgr::Load<CTexture>(L"맘모스Icon", L"../resources/Items/All/Mammoth/mammoth_icon.png");

	// Mastery
	CResourceMgr::Load<CTexture>(L"통달", L"../resources/Items/All/Mastery/mastery.png");
	CResourceMgr::Load<CTexture>(L"통달Icon", L"../resources/Items/All/Mastery/mastery_icon.png");

	// Medal
	CResourceMgr::Load<CTexture>(L"메달", L"../resources/Items/All/Medal/medal.png");
	CResourceMgr::Load<CTexture>(L"메달Icon", L"../resources/Items/All/Medal/medal_icon.png");

	// Metal_Plate
	CResourceMgr::Load<CTexture>(L"철판", L"../resources/Items/All/Metal_Plate/metal_plate.png");
	CResourceMgr::Load<CTexture>(L"철판Icon", L"../resources/Items/All/Metal_Plate/metal_plate_icon.png");

	// Missile
	CResourceMgr::Load<CTexture>(L"미사일Icon", L"../resources/Items/All/Missile/missile_icon.png");

	// Plant
	CResourceMgr::Load<CTexture>(L"식물Icon", L"../resources/Items/All/Plant/plant_icon.png");

	// Poisonous_Tonic
	CResourceMgr::Load<CTexture>(L"유독한 토닉Icon", L"../resources/Items/All/Poisonous_Tonic/poisonous_tonic_icon.png");

	// Potato
	CResourceMgr::Load<CTexture>(L"감자", L"../resources/Items/All/Potato/potato.png");
	CResourceMgr::Load<CTexture>(L"감자Icon", L"../resources/Items/All/Potato/potato_icon.png");

	// Characters
	
	// WellRounded
	CResourceMgr::Load<CTexture>(L"WellRoundedEyes", L"../resources/Items/Character/Well_rounded/well_rounded_eyes.png");
	CResourceMgr::Load<CTexture>(L"WellRoundedMouth", L"../resources/Items/Character/Well_rounded/well_rounded_mouth.png");
	CResourceMgr::Load<CTexture>(L"WellRoundedIcon", L"../resources/Items/Character/Well_rounded/well_rounded_icon.png");

	// Ranger
	CResourceMgr::Load<CTexture>(L"RangerEyes", L"../resources/Items/Character/Ranger/ranger_eyes.png");
	CResourceMgr::Load<CTexture>(L"RangerMouth", L"../resources/Items/Character/Ranger/ranger_mouth.png");
	CResourceMgr::Load<CTexture>(L"RangerIcon", L"../resources/Items/Character/Ranger/ranger_icon.png");

	// material
	CResourceMgr::Load<CTexture>(L"HarvestIcon", L"../resources/Items/materials/harvesting_icon.png");

	//Weapon
	// Dagger
	CResourceMgr::Load<CTexture>(L"칼", L"../resources/Weapons/Dagger/dagger.png");
	CResourceMgr::Load<CTexture>(L"DaggerIcon", L"../resources/Weapons/Dagger/dagger_icon.png");

	// Spear
	CResourceMgr::Load<CTexture>(L"창", L"../resources/Weapons/Spear/spear.png");
	CResourceMgr::Load<CTexture>(L"SpearIcon", L"../resources/Weapons/Spear/spear_icon.png");

	// Fist
	CResourceMgr::Load<CTexture>(L"주먹", L"../resources/Weapons/Fist/fist.png");
	CResourceMgr::Load<CTexture>(L"FistIcon", L"../resources/Weapons/Fist/fist_icon.png");

	// Claw
	CResourceMgr::Load<CTexture>(L"클로", L"../resources/Weapons/Claw/claw.png");
	CResourceMgr::Load<CTexture>(L"ClawIcon", L"../resources/Weapons/Claw/claw_icon.png");

	// Scissors
	CResourceMgr::Load<CTexture>(L"가위", L"../resources/Weapons/Scissors/scissors.png");
	CResourceMgr::Load<CTexture>(L"ScissorsIcon", L"../resources/Weapons/Scissors/scissors_icon.png");
	
	// PlayerBullet
	CResourceMgr::Load<CTexture>(L"PlayerBullet", L"../resources/Weapons/bullet_0001.png");

	// EnemyBullet


	// Pistol
	CResourceMgr::Load<CTexture>(L"권총", L"../resources/Weapons/Pistol/pistol.png");
	CResourceMgr::Load<CTexture>(L"PistolIcon", L"../resources/Weapons/Pistol/pistol_icon.png");

	// MedicalGun
	CResourceMgr::Load<CTexture>(L"의료 총", L"../resources/Weapons/Medical_Gun/medical_gun.png");
	CResourceMgr::Load<CTexture>(L"MedicalGunIcon", L"../resources/Weapons/Medical_Gun/medical_gun_icon.png");

	// SMG
	CResourceMgr::Load<CTexture>(L"SMG", L"../resources/Weapons/SMG/smg.png");
	CResourceMgr::Load<CTexture>(L"SMGIcon", L"../resources/Weapons/SMG/smg_icon.png");

	// stats
	CResourceMgr::Load<CTexture>(L"MaxHP", L"../resources/Items/Stats/max_hp.png");
	CResourceMgr::Load<CTexture>(L"HPRegen", L"../resources/Items/Stats/hp_regeneration.png");
	CResourceMgr::Load<CTexture>(L"LifeSteal", L"../resources/Items/Stats/lifesteal.png");
	CResourceMgr::Load<CTexture>(L"Armor", L"../resources/Items/Stats/armor.png");
	CResourceMgr::Load<CTexture>(L"Damage", L"../resources/Items/Stats/percent_damage.png");
	CResourceMgr::Load<CTexture>(L"MeleeDamage", L"../resources/Items/Stats/melee_damage.png");
	CResourceMgr::Load<CTexture>(L"RangedDamage", L"../resources/Items/Stats/ranged_damage.png");
	CResourceMgr::Load<CTexture>(L"AttackSpeed", L"../resources/Items/Stats/attack_speed.png");
	CResourceMgr::Load<CTexture>(L"CritChance", L"../resources/Items/Stats/crit_chance.png");
	CResourceMgr::Load<CTexture>(L"Range", L"../resources/Items/Stats/range.png");
	CResourceMgr::Load<CTexture>(L"Dodge", L"../resources/Items/Stats/dodge.png");
	CResourceMgr::Load<CTexture>(L"Speed", L"../resources/Items/Stats/speed.png");

	// upgrades
	CResourceMgr::Load<CTexture>(L"UpgradeIcon", L"../resources/Items/Upgrades/upgrade_icon.png");


	// UI
	CResourceMgr::Load<CTexture>(L"UIHUDBg", L"../resources/UI/uiHUD/ui_lifebar_bg.png");
	CTexture* hudFill = CResourceMgr::Load<CTexture>(L"UIHUDFill", L"../resources/UI/uiHUD/ui_lifebar_fill.png");
	CResourceMgr::Load<CTexture>(L"UIHUDFrame", L"../resources/UI/uiHUD/ui_lifebar_frame.png");

	CTexture* lifeFill = CTexture::Create(L"UILifeFill", hudFill->GetWidth(), hudFill->GetHeight());
	CTexture* EXPFill = CTexture::Create(L"UIEXPFill", hudFill->GetWidth(), hudFill->GetHeight());

	CTexture::ApplyOtherColorToWantedAreas(0, 255, 255.0f, 0.0f, 0.0f, lifeFill->GetImage(), hudFill->GetImage());
	CTexture::ApplyOtherColorToWantedAreas(0, 255, 0.0f, 255.0f, 0.0f, EXPFill->GetImage(), hudFill->GetImage());


	// TileMap
	CResourceMgr::Load<CTexture>(L"Tile1", L"../resources/Maps/Tiles/tiles_1.png");
	CResourceMgr::Load<CTexture>(L"Tile2", L"../resources/Maps/Tiles/tiles_2.png");
	CResourceMgr::Load<CTexture>(L"Tile3", L"../resources/Maps/Tiles/tiles_3.png");
	CResourceMgr::Load<CTexture>(L"Tile4", L"../resources/Maps/Tiles/tiles_4.png");
	CResourceMgr::Load<CTexture>(L"Tile5", L"../resources/Maps/Tiles/tiles_5.png");
	CResourceMgr::Load<CTexture>(L"Tile6", L"../resources/Maps/Tiles/tiles_6.png");
	CResourceMgr::Load<CTexture>(L"TileOutLine", L"../resources/Maps/Tiles/tiles_outline_modify.png");


}

void GameEngine::LoadMonster(std::wstring tName, std::wstring tPath)
{
	std::wstring collisionMonster = tName + L"Collision";

	CTexture* monsterImg = CResourceMgr::Load<CTexture>(tName, tPath);
	CTexture* monsterCollisionImg = CTexture::Create(collisionMonster, monsterImg->GetWidth(), monsterImg->GetHeight());

	CTexture::ApplySolidColor((BYTE)255, (BYTE)255, (BYTE)255, monsterCollisionImg->GetImage(), monsterImg->GetImage());

	monsterImg->CreateHBitmapFromGdiPlus(false);
	monsterCollisionImg->CreateHBitmapFromGdiPlus(false);
}

void GameEngine::DestroyScenes()
{
	CSceneMgr::OnDestroy();
}

void GameEngine::DestroyResources()
{
	CResourceMgr::OnDestroy();
	RemoveFontResourceEx(L"..Resources/Font/NotoSansKR-Medium.otf", FR_PRIVATE, 0);
}

void GameEngine::RemoveDeadObjects()
{
	CSceneMgr::RemoveDeadObjects();
}
