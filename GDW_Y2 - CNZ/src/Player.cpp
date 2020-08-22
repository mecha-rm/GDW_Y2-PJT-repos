#include "Player.h"
#include "cherry/objects/Primitive.h"
#include "cherry/physics/PhysicsBody.h"
#include "cherry/animate/AnimationManager.h"

// creates a player with 
cnz::Player::Player(std::string modelFile) : Player(modelFile, cherry::Vec3()) {}

// sets the player file, and position
cnz::Player::Player(std::string modelPath, float posX, float posY, float posZ)
	: Player(modelPath, cherry::Vec3(posX, posY, posZ)) {}

// sets the player file, and position
cnz::Player::Player(std::string modelFile, cherry::Vec3 pos) : Object(modelFile)
{
	position = pos;
	// TODO: replace with a more accurate hitbox
	//AddPhysicsBody(new cherry::PhysicsBodyBox(3.5F, 3.5F, 3.5F));
}

// creates the player at the world's origin
cnz::Player::Player(std::string modelPath, std::string scene)
	: Player(modelPath, scene, cherry::Vec3())
{}

// creates the player in hte provided scene
cnz::Player::Player(std::string modelPath, std::string scene, cherry::Vec3 pos)
	:Object(modelPath, scene, true, true)
{
	position = pos;
}

// copy constructor
cnz::Player::Player(Player* player, std::string sceneName)
	: Player(*player)
{
	SetScene(sceneName);
}

// copy constructor
cnz::Player::Player(const Player& obj) : Object(obj) 
{
	// values
	position = obj.GetPosition();
	chargingDash = obj.chargingDash;
	dashDist = obj.dashDist;

	// charge values
	dashChargeFactor = obj.dashChargeFactor;
	dashChargeTimeMax = obj.dashChargeTimeMax;
	dashChargeTimeCurr = obj.dashChargeTimeCurr;

	// cT = obj.cT;
	// cB = obj.cB;
	// cL = obj.cL;
	// cR = obj.cR;

	pBodySize = obj.pBodySize;
	drawPBody = obj.drawPBody;

	// since the physics bodies are remade, the player gets its own physics body as the primary.
	if (GetPhysicsBodyCount() > 0)
		primaryBody = GetPhysicsBody(0);

	degreeAngle = obj.degreeAngle;
	radianAngle = obj.radianAngle;

	worldAngle = obj.worldAngle;

	playerCurMovement = obj.playerCurMovement;
	state = obj.state;

	// // setting start animation
	// if(aniSpecial.index)
	// plyr->SetCurrentAnimationByPointer(playerSpecial.anim);
	// plyr->SetCurrentAnimationByIndex(plyr->aniSpecial.index);
	// // todo: you need to have this animation playing on the copied object.
	// playerSpecial.anim->Play();
	
	// getting the animations
	{
		int index = 0;
	
		// charging animation
		index = obj.GetChargingAnimationIndex();
		if (index != -1)
		{
			aniCharging.index = index;
			aniCharging.animation = GetAnimation(index);
		}

		// charged animation
		index = obj.GetChargedAnimationIndex();
		if (index != -1)
		{
			aniCharged.index = index;
			aniCharged.animation = GetAnimation(index);
		}

		// dashing animation
		index = obj.GetDashingAnimationIndex();
		if (index != -1)
		{
			aniDashing.index = index;
			aniDashing.animation = GetAnimation(index);
		}

		// run (F) animation
		index = obj.GetRunFAnimationIndex();
		if (index != -1)
		{
			aniRun_F.index = index;
			aniRun_F.animation = GetAnimation(index);
		}

		// run (B) animation
		index = obj.GetRunBAnimationIndex();
		if (index != -1)
		{
			aniRun_B.index = index;
			aniRun_B.animation = GetAnimation(index);
		}

		// run (L) animation
		index = obj.GetRunLAnimationIndex();
		if (index != -1)
		{
			aniRun_L.index = index;
			aniRun_L.animation = GetAnimation(index);
		}

		// run (R) animation
		index = obj.GetRunRAnimationIndex();
		if (index != -1)
		{
			aniRun_R.index = index;
			aniRun_R.animation = GetAnimation(index);
		}

		// dodge (F) animation
		index = obj.GetDodgeFAnimationIndex();
		if (index != -1)
		{
			aniDodge_F.index = index;
			aniDodge_F.animation = GetAnimation(index);
		}

		// dodge (B) animation
		index = obj.GetDodgeBAnimationIndex();
		if (index != -1)
		{
			aniDodge_B.index = index;
			aniDodge_B.animation = GetAnimation(index);
		}

		// dodge (L) animation
		index = obj.GetDodgeLAnimationIndex();
		if (index != -1)
		{
			aniDodge_L.index = index;
			aniDodge_L.animation = GetAnimation(index);
		}

		// dodge (R) animation
		index = obj.GetDodgeRAnimationIndex();
		if (index != -1)
		{
			aniDodge_R.index = index;
			aniDodge_R.animation = GetAnimation(index);
		}

		// death animation
		index = obj.GetDeathAnimationIndex();
		if (index != -1)
		{
			aniDeath.index = index;
			aniDeath.animation = GetAnimation(index);
		}

		// special animation
		index = obj.GetSpecialAnimationIndex();
		if (index != -1)
		{
			aniSpecial.index = index;
			aniSpecial.animation = GetAnimation(index);
		}

		// bash animation
		index = obj.GetBashAnimationIndex();
		if (index != -1)
		{
			aniBash.index = index;
			aniBash.animation = GetAnimation(index);
		}

		// DEFAULT ANIMATION
		if (aniRun_F.index != -1)
		{
			SetCurrentAnimationByIndex(aniRun_F.index);
			aniRun_F.animation->Play();
		}
	}

}

// TODO: either fix this, or remove it.
// copies a primitive (doesn't work)
//cnz::Player::Player(const cherry::Primitive * model)
//{
//	if (model == nullptr)
//	{
//		safe = false;
//		return;
//	}
//	// if(model->getSafe)
//
//	model->getSafe();
//
//	// copies the memory for vertices and indices
//	memcpy(vertices, model->GetVertices(), sizeof(cherry::Vertex) * model->getVerticesTotal());
//	memcpy(indices, model->GetIndices(), sizeof(uint32_t) * model->GetIndicesTotal());
//
//
//	mesh = std::make_shared<cherry::Mesh>(vertices, verticesTotal, indices, indicesTotal); // creates the mesh
//	position = model->GetPosition();
//}


// sets dash for the player
void cnz::Player::SetChargingDash(bool dash) 
{
	this->chargingDash = dash; 
	
	// no longer dashing, so the current charge time is set to 0.
	if (dash == false)
		dashChargeTimeCurr = 0.0F;
}

// is dash fully charged.
bool cnz::Player::IsDashFullyCharged() const
{
	return (dashChargeTimeCurr >= dashChargeTimeMax);
}

// returns the maximum dash charge time.
float cnz::Player::GetMaximumDashChargeTime() const
{
	return dashChargeTimeMax;
}

// sets the maximum dash charge time.
void cnz::Player::SetMaximumDashChargeTime(float mdt)
{
	// the maximum dash time cannot be negative.
	dashChargeTimeMax = (mdt >= 0.0F) ? mdt : 0.0F;
}

// gets the current dash charge time.
float cnz::Player::GetCurrentDashChargeTime() const
{
	return dashChargeTimeCurr;
}

// sets the current dash charge time.
void cnz::Player::SetCurrentDashChargeTime(float dct)
{
	dashChargeTimeCurr = (dct >= 0.0F) ? dct : 0.0F;
}

// generates the default character.
cnz::Player* cnz::Player::GenerateDefault(std::string scene, cherry::Vec3 position)
{
	// TODO: the player object gets remade in CNZ_GameplayScene, so this doesn't work.
	// that will need to be fixed.
	using namespace cherry;

	Player* plyr = new Player("res/objects/hero_ver.2/One_T.obj", scene, position);

	Vec3 dmns = plyr->GetMeshBodyMaximum() - plyr->GetMeshBodyMinimum();
	
	// collision body
	cherry::PhysicsBodyBox* body = new PhysicsBodyBox(dmns / 2.0F, Vec3(2, 2, 3));
	plyr->AddPhysicsBody(body);
	plyr->primaryBody = body;

	// setting emissive colour and power
	plyr->SetEmissiveColor(Vec3(0.0F, 1.0F, 1.0F));
	plyr->SetEmissivePower(0.10F);

	// dash values
	plyr->SetDashChargeFactor(1.25F);
	plyr->SetMaximumDashChargeTime(1.0F);

	// animation 1
	// if(false) // commenting out
	// {
	// 	MorphAnimation* mph = new MorphAnimation(plyr);
	// 
	// 	// first 10 frames
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000000.obj"));
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000001.obj"));
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000002.obj"));
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000003.obj"));
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000004.obj"));
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000005.obj"));
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000006.obj"));
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000007.obj"));
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000008.obj"));
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000009.obj"));
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000010.obj"));
	// 
	// 	// second 10 frames
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000011.obj"));
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000012.obj"));
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000013.obj"));
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000014.obj"));
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000015.obj"));
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000016.obj"));
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000017.obj"));
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000018.obj"));
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000019.obj"));
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000020.obj"));
	// 
	// 	// final frames
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000021.obj"));
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000022.obj"));
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000023.obj"));
	// 	mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000024.obj"));
	// 
	// 	plyr->AddAnimation(mph, true);
	// 	mph->Play();
	// }

	// TODO: optimize
	// loading animations (ver. 1)
	// {
	// 	// full
	// 	// these take WAY to long to add, but they work.
	// 	// animations
	// 	cnz::AnimStruct playerCharging = { 26, "res/objects/anims/player/Attack_Charge/One_Charge_0000", 1.08f, new cherry::MorphAnimation() };
	// 	cnz::AnimStruct playerCharged = { 26, "res/objects/anims/player/Attack_Charged/One_Charge_0000", 0.01f, new cherry::MorphAnimation() };
	// 	cnz::AnimStruct playerDashing = { 26, "res/objects/anims/player/Attack_Dash/One_Dash_0000", 0.27f, new cherry::MorphAnimation() };
	// 	cnz::AnimStruct playerRun_F = { 24, "res/objects/anims/player/Run_F/One_RF_0000", 1.0f, new cherry::MorphAnimation() };
	// 	cnz::AnimStruct playerRun_B = { 24, "res/objects/anims/player/Run_B/One_RB_0000", 1.0f, new cherry::MorphAnimation() };
	// 	cnz::AnimStruct playerRun_L = { 24, "res/objects/anims/player/Run_L/One_RL_0000", 1.0f, new cherry::MorphAnimation() };
	// 	cnz::AnimStruct playerRun_R = { 24, "res/objects/anims/player/Run_R/One_RR_0000", 1.0f, new cherry::MorphAnimation() };
	// 	cnz::AnimStruct playerDodge_F = { 12, "res/objects/anims/player/Dodge_F/One_DF_0000", 0.46f, new cherry::MorphAnimation() };
	// 	cnz::AnimStruct playerDodge_B = { 12, "res/objects/anims/player/Dodge_B/One_DB_0000", 0.46f, new cherry::MorphAnimation() };
	// 	cnz::AnimStruct playerDodge_L = { 12, "res/objects/anims/player/Dodge_L/One_DL_0000", 0.46f, new cherry::MorphAnimation() };
	// 	cnz::AnimStruct playerDodge_R = { 12, "res/objects/anims/player/Dodge_R/One_DR_0000", 0.46f, new cherry::MorphAnimation() };
	// 	cnz::AnimStruct playerDeath = { 52, "res/objects/anims/player/Took_Damage/One_Die_0000", 2.16f, new cherry::MorphAnimation() };
	// 	cnz::AnimStruct playerSpecial = { 37, "res/objects/anims/player/Special_Attack/Special_Attack_0000", 1.54f, new cherry::MorphAnimation() };
	// 	cnz::AnimStruct playerBash = { 16, "res/objects/anims/player/Bash/One_Bash_0000", 0.63f, new cherry::MorphAnimation() };
	// 
	// 	// testing
	// 	// cnz::AnimStruct playerCharging = { 1, "res/objects/anims/player/Attack_Charge/One_Charge_0000", 1.08f, new cherry::MorphAnimation() };
	// 	// cnz::AnimStruct playerCharged = { 1, "res/objects/anims/player/Attack_Charged/One_Charge_0000", 0.01f, new cherry::MorphAnimation() };
	// 	// cnz::AnimStruct playerDashing = { 1, "res/objects/anims/player/Attack_Dash/One_Dash_0000", 0.27f, new cherry::MorphAnimation() };
	// 	// cnz::AnimStruct playerRun_F = { 1, "res/objects/anims/player/Run_F/One_RF_0000", 1.0f, new cherry::MorphAnimation() };
	// 	// cnz::AnimStruct playerRun_B = { 1, "res/objects/anims/player/Run_B/One_RB_0000", 1.0f, new cherry::MorphAnimation() };
	// 	// cnz::AnimStruct playerRun_L = { 1, "res/objects/anims/player/Run_L/One_RL_0000", 1.0f, new cherry::MorphAnimation() };
	// 	// cnz::AnimStruct playerRun_R = { 1, "res/objects/anims/player/Run_R/One_RR_0000", 1.0f, new cherry::MorphAnimation() };
	// 	// cnz::AnimStruct playerDodge_F = { 1, "res/objects/anims/player/Dodge_F/One_DF_0000", 0.46f, new cherry::MorphAnimation() };
	// 	// cnz::AnimStruct playerDodge_B = { 1, "res/objects/anims/player/Dodge_B/One_DB_0000", 0.46f, new cherry::MorphAnimation() };
	// 	// cnz::AnimStruct playerDodge_L = { 1, "res/objects/anims/player/Dodge_L/One_DL_0000", 0.46f, new cherry::MorphAnimation() };
	// 	// cnz::AnimStruct playerDodge_R = { 1, "res/objects/anims/player/Dodge_R/One_DR_0000", 0.46f, new cherry::MorphAnimation() };
	// 	// cnz::AnimStruct playerDeath = { 1, "res/objects/anims/player/Took_Damage/One_Die_0000", 2.16f, new cherry::MorphAnimation() };
	// 	// cnz::AnimStruct playerSpecial = { 1, "res/objects/anims/player/Special Attack/Special_Attack_0000", 1.54f, new cherry::MorphAnimation() };
	// 	// cnz::AnimStruct playerBash = { 1, "res/objects/anims/player/Bash/One_Bash_0000", 0.63f, new cherry::MorphAnimation() };
	// 
	// 	// add animations to player.
	// 	plyr->AddAnimation(playerCharging.anim);
	// 	plyr->AddAnimation(playerCharged.anim);
	// 	plyr->AddAnimation(playerDashing.anim);
	// 	plyr->AddAnimation(playerRun_F.anim);
	// 	plyr->AddAnimation(playerRun_B.anim);
	// 	plyr->AddAnimation(playerRun_L.anim);
	// 	plyr->AddAnimation(playerRun_R.anim);
	// 	plyr->AddAnimation(playerDodge_F.anim);
	// 	plyr->AddAnimation(playerDodge_B.anim);
	// 	plyr->AddAnimation(playerDodge_L.anim);
	// 	plyr->AddAnimation(playerDodge_R.anim);
	// 	plyr->AddAnimation(playerDeath.anim);
	// 	plyr->AddAnimation(playerSpecial.anim);
	// 	plyr->AddAnimation(playerBash.anim);
	// 
	// 	std::vector<cnz::AnimStruct*> animList;
	// 
	// 	animList.push_back(&playerCharging);
	// 	animList.push_back(&playerCharged);
	// 	animList.push_back(&playerDashing);
	// 	animList.push_back(&playerRun_F);
	// 	animList.push_back(&playerRun_B);
	// 	animList.push_back(&playerRun_L);
	// 	animList.push_back(&playerRun_R);
	// 	animList.push_back(&playerDodge_F);
	// 	animList.push_back(&playerDodge_B);
	// 	animList.push_back(&playerDodge_L);
	// 	animList.push_back(&playerDodge_R);
	// 	animList.push_back(&playerDeath);
	// 	animList.push_back(&playerSpecial);
	// 	animList.push_back(&playerBash);
	// 
	// 	// 
	// 	// //// Auto creation of animations based on data (DO NOT CHANGE THIS)
	// 	// // this takes a lot of time.
	// 	std::stringstream curPath;
	// 	float curFrameTime;
	// 	for (int i = 0; i < animList.size(); i++) {
	// 		curFrameTime = animList[i]->animTime / animList[i]->numFrames;
	// 		for (int frame = 0; frame < animList[i]->numFrames; frame++) {
	// 			curPath = std::stringstream();
	// 			if (frame < 10) {
	// 				curPath << animList[i]->basePath << 0 << frame << ".obj";
	// 			}
	// 			else {
	// 				curPath << animList[i]->basePath << frame << ".obj";
	// 			}
	// 			animList[i]->anim->AddFrame(new cherry::MorphAnimationFrame(curPath.str(), curFrameTime));
	// 		}
	// 	}
	// 
	// 	// MOVED
	// 	//// Add finished animations to objects
	// 	// Player
	// 	// plyr->AddAnimation(playerCharging.anim);
	// 	// plyr->AddAnimation(playerCharged.anim);
	// 	// plyr->AddAnimation(playerDashing.anim);
	// 	// plyr->AddAnimation(playerRun_F.anim);
	// 	// plyr->AddAnimation(playerRun_B.anim);
	// 	// plyr->AddAnimation(playerRun_L.anim);
	// 	// plyr->AddAnimation(playerRun_R.anim);
	// 	// plyr->AddAnimation(playerDodge_F.anim);
	// 	// plyr->AddAnimation(playerDodge_B.anim);
	// 	// plyr->AddAnimation(playerDodge_L.anim);
	// 	// plyr->AddAnimation(playerDodge_R.anim);
	// 	// plyr->AddAnimation(playerDeath.anim);
	// 	// plyr->AddAnimation(playerSpecial.anim);
	// 	// plyr->AddAnimation(playerBash.anim);
	// }

	// loading animations (ver. 2)
	{
		// TODO: some meshes were NOT triangluated. Check the walking mesh since it seems like it could be the problem.
		// TODO: remove animations that are not being used.
		// full
		// these take WAY to long to add, but they work.
		// animations
		// cnz::AnimStruct playerCharging = { 26, "res/objects/anims/player/Attack_Charge/One_Charge_0000", 1.08f, new cherry::MorphAnimation() };
		// 
		// // this seems to be a leftover from an earlier build
		// cnz::AnimStruct playerCharged = { 26, "res/objects/anims/player/Attack_Charged/One_Charge_0000", 0.01f, new cherry::MorphAnimation() };
		// 
		// cnz::AnimStruct playerDashing = { 26, "res/objects/anims/player/Attack_Dash/One_Dash_0000", 0.27f, new cherry::MorphAnimation() };
		// cnz::AnimStruct playerRun_F = { 24, "res/objects/anims/player/Run_F/One_RF_0000", 1.0f, new cherry::MorphAnimation() };
		// cnz::AnimStruct playerRun_B = { 24, "res/objects/anims/player/Run_B/One_RB_0000", 1.0f, new cherry::MorphAnimation() };
		// cnz::AnimStruct playerRun_L = { 24, "res/objects/anims/player/Run_L/One_RL_0000", 1.0f, new cherry::MorphAnimation() };
		// cnz::AnimStruct playerRun_R = { 24, "res/objects/anims/player/Run_R/One_RR_0000", 1.0f, new cherry::MorphAnimation() };
		// cnz::AnimStruct playerDodge_F = { 12, "res/objects/anims/player/Dodge_F/One_DF_0000", 0.46f, new cherry::MorphAnimation() };
		// cnz::AnimStruct playerDodge_B = { 12, "res/objects/anims/player/Dodge_B/One_DB_0000", 0.46f, new cherry::MorphAnimation() };
		// cnz::AnimStruct playerDodge_L = { 12, "res/objects/anims/player/Dodge_L/One_DL_0000", 0.46f, new cherry::MorphAnimation() };
		// cnz::AnimStruct playerDodge_R = { 12, "res/objects/anims/player/Dodge_R/One_DR_0000", 0.46f, new cherry::MorphAnimation() };
		// cnz::AnimStruct playerDeath = { 52, "res/objects/anims/player/Took_Damage/One_Die_0000", 2.16f, new cherry::MorphAnimation() };
		// cnz::AnimStruct playerSpecial = { 37, "res/objects/anims/player/Special_Attack/Special_Attack_0000", 1.54f, new cherry::MorphAnimation() };
		// cnz::AnimStruct playerBash = { 16, "res/objects/anims/player/Bash/One_Bash_0000", 0.63f, new cherry::MorphAnimation() };

		// add animations to player.
		// the bool determines whether to load the animation or not.
		bool loadCharging = true;
		bool loadCharged = true;
		bool loadDashing = true;
		bool loadRun_F = true;
		bool loadRun_B = true;
		bool loadRun_L = true;
		bool loadRun_R = true;
		bool loadDodge_F = true;
		bool loadDodge_B = true;
		bool loadDodge_L = true;
		bool loadDodge_R = true;
		bool loadDeath = true;
		bool loadSpecial = true;
		bool loadBash = true;

		// charging - 26 total frames
		// pretty sure this animation was never set.
		
		// player charging
		if(loadCharging)
		{
			cnz::AnimStruct playerCharging = { 
				26, "res/objects/anims/player/Attack_Charge/One_Charge_0000", 1.08f, new cherry::MorphAnimation() };

			plyr->AddAnimation(playerCharging.anim);

			playerCharging.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000000.obj", 1.08f));
			playerCharging.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000006.obj", 1.08f));
			playerCharging.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000012.obj", 1.08f));
			playerCharging.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000018.obj", 1.08f));
			playerCharging.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000025.obj", 1.08f));

			// saves the animation and its index
			plyr->aniCharging.animation = playerCharging.anim;
			plyr->aniCharging.index = plyr->GetAnimationCount() - 1;
		}

		// seems like this was a leftover.
		// attack charged animation - 26 frames
		if(loadCharged)
		{
			cnz::AnimStruct playerCharged = { 26, "res/objects/anims/player/Attack_Charged/One_Charge_0000", 0.01f, new cherry::MorphAnimation() };

			plyr->AddAnimation(playerCharged.anim);
			playerCharged.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charged/One_Charge_000000.obj", 0.01f));
			// playerCharged.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charged/One_Charge_000006.obj", 0.01f));
			// playerCharged.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charged/One_Charge_000012.obj", 0.01f));
			// playerCharged.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charged/One_Charge_000018.obj", 0.01f));
			// playerCharged.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charged/One_Charge_000025.obj", 0.01f));
		
			// saves the animation and its index
			plyr->aniCharged.animation = playerCharged.anim;
			plyr->aniCharged.index = plyr->GetAnimationCount() - 1;
		}

		// dashing animation - 26 frames
		if(loadDashing)
		{
			cnz::AnimStruct playerDashing = { 26, "res/objects/anims/player/Attack_Dash/One_Dash_0000", 0.27f, new cherry::MorphAnimation() };
			plyr->AddAnimation(playerDashing.anim);

			playerDashing.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Dash/One_Dash_000000.obj", 0.27f));
			playerDashing.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Dash/One_Dash_000006.obj", 0.27f));
			playerDashing.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Dash/One_Dash_000012.obj", 0.27f));
			playerDashing.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Dash/One_Dash_000018.obj", 0.27f));
			playerDashing.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Dash/One_Dash_000025.obj", 0.27f));

			// saves the animation and its index
			plyr->aniDashing.animation = playerDashing.anim;
			plyr->aniDashing.index = plyr->GetAnimationCount() - 1;
		}

		// running animation forward - 24 frames
		if(loadRun_F)
		{
			cnz::AnimStruct playerRun_F = { 24, "res/objects/anims/player/Run_F/One_RF_0000", 1.0f, new cherry::MorphAnimation() };
			plyr->AddAnimation(playerRun_F.anim);

			playerRun_F.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_F/One_RF_000000.obj", 1.0f));
			playerRun_F.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_F/One_RF_000006.obj", 1.0f));
			playerRun_F.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_F/One_RF_000012.obj", 1.0f));
			playerRun_F.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_F/One_RF_000018.obj", 1.0f));
			playerRun_F.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_F/One_RF_000023.obj", 1.0f));

			// saves the animation and its index
			playerRun_F.anim->SetInfiniteLoop(true);
			plyr->aniRun_F.animation = playerRun_F.anim;
			plyr->aniRun_F.index = plyr->GetAnimationCount() - 1;
		}

		// running animation backward - 24 frames
		if (loadRun_B)
		{
			cnz::AnimStruct playerRun_B = { 24, "res/objects/anims/player/Run_B/One_RB_0000", 1.0f, new cherry::MorphAnimation() };
			plyr->AddAnimation(playerRun_B.anim);
			playerRun_B.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_B/One_RB_000000.obj", 1.0f));
			playerRun_B.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_B/One_RB_000006.obj", 1.0f));
			playerRun_B.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_B/One_RB_000012.obj", 1.0f));
			playerRun_B.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_B/One_RB_000018.obj", 1.0f));
			playerRun_B.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_B/One_RB_000023.obj", 1.0f));

			// saves the animation and its index
			playerRun_B.anim->SetInfiniteLoop(true);
			plyr->aniRun_B.animation = playerRun_B.anim;
			plyr->aniRun_B.index = plyr->GetAnimationCount() - 1;
		}

		// running animation left - 24 frames
		if (loadRun_L)
		{
			cnz::AnimStruct playerRun_L = { 24, "res/objects/anims/player/Run_L/One_RL_0000", 1.0f, new cherry::MorphAnimation() };

			plyr->AddAnimation(playerRun_L.anim);
			playerRun_L.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_L/One_RL_000000.obj", 1.0f));
			playerRun_L.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_L/One_RL_000006.obj", 1.0f));
			playerRun_L.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_L/One_RL_000012.obj", 1.0f));
			playerRun_L.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_L/One_RL_000018.obj", 1.0f));
			playerRun_L.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_L/One_RL_000023.obj", 1.0f));

			// saves the animation and its index
			playerRun_L.anim->SetInfiniteLoop(true);
			plyr->aniRun_L.animation = playerRun_L.anim;
			plyr->aniRun_L.index = plyr->GetAnimationCount() - 1;
		}
		
		// running animation right - 24 frames
		if (loadRun_R)
		{
			cnz::AnimStruct playerRun_R = { 24, "res/objects/anims/player/Run_R/One_RR_0000", 1.0f, new cherry::MorphAnimation() };

			plyr->AddAnimation(playerRun_R.anim);
			playerRun_R.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_R/One_RR_000000.obj", 1.0f));
			playerRun_R.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_R/One_RR_000006.obj", 1.0f));
			playerRun_R.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_R/One_RR_000012.obj", 1.0f));
			playerRun_R.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_R/One_RR_000018.obj", 1.0f));
			playerRun_R.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_R/One_RR_000023.obj", 1.0f));

			// saves the animation and its index
			playerRun_R.anim->SetInfiniteLoop(true);
			plyr->aniRun_R.animation = playerRun_R.anim;
			plyr->aniRun_R.index = plyr->GetAnimationCount() - 1;
		}

		// dodge animation forward - 12 frames
		if (loadDodge_F)
		{
			cnz::AnimStruct playerDodge_F = { 12, "res/objects/anims/player/Dodge_F/One_DF_0000", 0.46f, new cherry::MorphAnimation() };

			plyr->AddAnimation(playerDodge_F.anim);
			playerDodge_F.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_F/One_DF_000000.obj", 0.46f));
			playerDodge_F.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_F/One_DF_000004.obj", 0.46f));
			playerDodge_F.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_F/One_DF_000008.obj", 0.46f));
			playerDodge_F.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_F/One_DF_000011.obj", 0.46f));

			// saves the animation and its index
			plyr->aniDodge_F.animation = playerDodge_F.anim;
			plyr->aniDodge_F.index = plyr->GetAnimationCount() - 1;
		}

		// dodge animation backward - 12 frames
		if (loadDodge_B)
		{
			cnz::AnimStruct playerDodge_B = { 12, "res/objects/anims/player/Dodge_B/One_DB_0000", 0.46f, new cherry::MorphAnimation() };
			plyr->AddAnimation(playerDodge_B.anim);

			playerDodge_B.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_B/One_DB_000000.obj", 0.46f));
			playerDodge_B.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_B/One_DB_000004.obj", 0.46f));
			playerDodge_B.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_B/One_DB_000008.obj", 0.46f));
			playerDodge_B.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_B/One_DB_000011.obj", 0.46f));

			// saves the animation and its index
			plyr->aniDodge_B.animation = playerDodge_B.anim;
			plyr->aniDodge_B.index = plyr->GetAnimationCount() - 1;
		}

		// dodge animation left - 12 frames
		if (loadDodge_L)
		{
			cnz::AnimStruct playerDodge_L = { 12, "res/objects/anims/player/Dodge_L/One_DL_0000", 0.46f, new cherry::MorphAnimation() };
			plyr->AddAnimation(playerDodge_L.anim);

			playerDodge_L.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_L/One_DL_000000.obj", 0.46f));
			playerDodge_L.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_L/One_DL_000004.obj", 0.46f));
			playerDodge_L.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_L/One_DL_000008.obj", 0.46f));
			playerDodge_L.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_L/One_DL_000011.obj", 0.46f));

			// saves the animation and its index
			plyr->aniDodge_L.animation = playerDodge_L.anim;
			plyr->aniDodge_L.index = plyr->GetAnimationCount() - 1;
		}

		// dodge animation right - 12 frames
		if (loadDodge_R)
		{
			cnz::AnimStruct playerDodge_R = { 12, "res/objects/anims/player/Dodge_R/One_DR_0000", 0.46f, new cherry::MorphAnimation() };
			plyr->AddAnimation(playerDodge_R.anim);
			
			playerDodge_R.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_R/One_DR_000000.obj", 0.46f));
			playerDodge_R.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_R/One_DR_000004.obj", 0.46f));
			playerDodge_R.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_R/One_DR_000008.obj", 0.46f));
			playerDodge_R.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_R/One_DR_000011.obj", 0.46f));

			// saves the animation and its index
			plyr->aniDodge_R.animation = playerDodge_R.anim;
			plyr->aniDodge_R.index = plyr->GetAnimationCount() - 1;
		}

		// player death - 52 frames
		if (loadDeath)
		{
			cnz::AnimStruct playerDeath = { 52, "res/objects/anims/player/Took_Damage/One_Die_0000", 2.16f, new cherry::MorphAnimation() };
			plyr->AddAnimation(playerDeath.anim);

			playerDeath.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Took_Damage/One_Die_000000.obj", 0.46f));
			playerDeath.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Took_Damage/One_Die_000010.obj", 0.46f));
			playerDeath.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Took_Damage/One_Die_000020.obj", 0.46f));
			playerDeath.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Took_Damage/One_Die_000030.obj", 0.46f));
			playerDeath.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Took_Damage/One_Die_000040.obj", 0.46f));
			playerDeath.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Took_Damage/One_Die_000050.obj", 0.46f));

			// saves the animation and its index
			plyr->aniDeath.animation = playerDeath.anim;
			plyr->aniDeath.index = plyr->GetAnimationCount() - 1;
		}

		// player special - 37 frames
		if (loadSpecial)
		{
			cnz::AnimStruct playerSpecial = { 37, "res/objects/anims/player/Special_Attack/Special_Attack_0000", 1.54f, new cherry::MorphAnimation() };
			plyr->AddAnimation(playerSpecial.anim);

			playerSpecial.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Special_Attack/Special_Attack_000000.obj", 1.54f));
			playerSpecial.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Special_Attack/Special_Attack_000007.obj", 1.54f));
			playerSpecial.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Special_Attack/Special_Attack_000014.obj", 1.54f));
			playerSpecial.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Special_Attack/Special_Attack_000021.obj", 1.54f));
			playerSpecial.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Special_Attack/Special_Attack_000028.obj", 1.54f));
			playerSpecial.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Special_Attack/Special_Attack_000035.obj", 1.54f));
		
			// saves the animation and its index
			plyr->aniSpecial.animation = playerSpecial.anim;
			plyr->aniSpecial.index = plyr->GetAnimationCount() - 1;
		}

		// player bash - 16 frames
		if (loadBash)
		{
			cnz::AnimStruct playerBash = { 16, "res/objects/anims/player/Bash/One_Bash_0000", 0.63f, new cherry::MorphAnimation() };
			plyr->AddAnimation(playerBash.anim);

			playerBash.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Bash/One_Bash_000000.obj", 0.63f));
			playerBash.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Bash/One_Bash_000004.obj", 0.63f));
			playerBash.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Bash/One_Bash_000008.obj", 0.63f));
			playerBash.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Bash/One_Bash_000012.obj", 0.63f));
			playerBash.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Bash/One_Bash_000015.obj", 0.63f));

			// saves the animation and its index
			plyr->aniBash.animation = playerBash.anim;
			plyr->aniBash.index = plyr->GetAnimationCount() - 1;
		}
	}

	return plyr;
}

// gets whether the player is currently dashing.
bool cnz::Player::IsChargingDash() const { return chargingDash; }

// get dash charge factor
float cnz::Player::GetDashChargeFactor() const
{
	return dashChargeFactor;
}

// sets the dash charge factor
void cnz::Player::SetDashChargeFactor(float dcf)
{
	dashChargeFactor = (dcf > 0.0F) ? dcf : 1.0F;
}

// TODO: maybe these can be taken out? Degrees and radians are just different representations of the same thing.
// gets degree angle
float cnz::Player::GetDegreeAngle() const { return degreeAngle; }

// gets radian angle
float cnz::Player::GetRadianAngle() const { return radianAngle; }

// gets the vector 3 angle.
glm::vec3 cnz::Player::GetVec3Angle() const { return this->worldAngle; }

// updates the angle
void cnz::Player::UpdateAngle(cherry::Camera::Sptr camera, double xpos, double ypos, unsigned int width, unsigned int height) 
{

	float a = atanf((float)ypos / (float)xpos);

	if (ypos <= 0 && xpos <= 0) {
		a = 90 * (M_PI / 180.0f) - a;
		a += 90.0f * (M_PI / 180.0f);
	}

	if (ypos > 0 && xpos <= 0) {
		a = 90 * (M_PI / 180.0f) - a;
		a += 90.0f * (M_PI / 180.0f);
	}

	if (ypos >= 0 && xpos >= 0) {
		a = 90 * (M_PI / 180.0f) - a;
		a += 270.0f * (M_PI / 180.0f);
	}

	if (ypos < 0 && xpos >= 0) {
		a = 90 * (M_PI / 180.0f) - a;
		a += 270.0f * (M_PI / 180.0f);
	}

	this->radianAngle = a;
	this->degreeAngle = a * (180.0f / M_PI);

	// Update world angle
	glm::mat4 pv = camera->GetViewProjection();
	pv = glm::inverse(pv);
	glm::vec4 pos = glm::vec4(xpos / width, ypos / height, 0.0f, 1.0f);
	pv *= pos;


	pos.w = 1.0 / pos.w;
	pos.x *= pos.w;
	pos.y *= pos.w;
	pos.z *= pos.w;

	this->worldAngle = glm::vec3(pos.x, pos.y, pos.z);
	glm::quat rotation = glm::quat(this->worldAngle);
}



// sets the angle
void cnz::Player::SetAngle(float angle, bool isDegrees) {
	if (isDegrees) {
		this->degreeAngle = angle;
		this->radianAngle = (this->degreeAngle) * (M_PI / 180.0f);
	}
	else if (!isDegrees) {
		this->radianAngle = angle;
		this->degreeAngle = (this->radianAngle) * (180.0f / M_PI);
	}

	// add code here to convert from screenspace to world space
}

void cnz::Player::SetAngle(glm::vec3 angle) { this->worldAngle = angle; }

// gets the dash distance
glm::vec3 cnz::Player::GetDash(float dist) const {
	glm::vec3 dash;
	// glm is backwards so both need to be negative to dash in the correct direction.
	// afaik GetDegreeAngle() is not working correctly so we use Radians converted to degrees instead.
	dash.x = -(dist * glm::degrees(cosf(this->GetRadianAngle())));
	dash.y = -(dist * glm::degrees(sinf(this->GetRadianAngle())));
	dash.z = 0.0f;

	return dash;
}

// gets the primary phyiscs body of the player
cherry::PhysicsBody* cnz::Player::GetPrimaryPhysicsBody() const
{
	// gets the primary body if it is not set to nullptr.
	if (primaryBody != nullptr)
	{
		return primaryBody;
	}
	else // returns the first physics body in the list.
	{
		// will return nullptr if there are no bodies
		return GetPhysicsBody(0);
	}
}

// gets the charging animation
cherry::Animation* cnz::Player::GetChargingAnimation() const
{
	return aniCharging.animation;
}

int cnz::Player::GetChargingAnimationIndex() const
{
	return aniCharging.index;
}

cherry::Animation* cnz::Player::GetChargedAnimation() const
{
	return aniCharged.animation;
}

int cnz::Player::GetChargedAnimationIndex() const
{
	return aniCharged.index;
}

cherry::Animation* cnz::Player::GetDashingAnimation() const
{
	return aniDashing.animation;
}

int cnz::Player::GetDashingAnimationIndex() const
{
	return aniDashing.index;
}

cherry::Animation* cnz::Player::GetRunFAnimation() const
{
	return aniRun_F.animation;
}

int cnz::Player::GetRunFAnimationIndex() const
{
	return aniRun_F.index;
}

cherry::Animation* cnz::Player::GetRunBAnimation() const
{
	return aniRun_B.animation;
}

int cnz::Player::GetRunBAnimationIndex() const
{
	return aniRun_B.index;
}

cherry::Animation* cnz::Player::GetRunLAnimation() const
{
	return aniRun_L.animation;
}

int cnz::Player::GetRunLAnimationIndex() const
{
	return aniRun_L.index;
}

cherry::Animation* cnz::Player::GetRunRAnimation() const
{
	return aniRun_R.animation;
}

int cnz::Player::GetRunRAnimationIndex() const
{
	return aniRun_R.index;
}

cherry::Animation* cnz::Player::GetDodgeFAnimation() const
{
	return aniDodge_F.animation;
}

int cnz::Player::GetDodgeFAnimationIndex() const
{
	return aniDodge_F.index;
}

cherry::Animation* cnz::Player::GetDodgeBAnimation() const
{
	return aniDodge_B.animation;;
}

int cnz::Player::GetDodgeBAnimationIndex() const
{
	return aniDodge_B.index;
}

cherry::Animation* cnz::Player::GetDodgeLAnimation() const
{
	return aniDodge_L.animation;
}

int cnz::Player::GetDodgeLAnimationIndex() const
{
	return aniDodge_L.index;
}

cherry::Animation* cnz::Player::GetDodgeRAnimation() const
{
	return aniDodge_R.animation;
}

int cnz::Player::GetDodgeRAnimationIndex() const
{
	return aniDodge_R.index;
}

cherry::Animation* cnz::Player::GetDeathAnimation() const
{
	return aniDeath.animation;
}

int cnz::Player::GetDeathAnimationIndex() const
{
	return aniDeath.index;
}

cherry::Animation* cnz::Player::GetSpecialAnimation() const
{
	return aniSpecial.animation;
}

int cnz::Player::GetSpecialAnimationIndex() const
{
	return aniSpecial.index;
}


cherry::Animation* cnz::Player::GetBashAnimation() const
{
	return aniBash.animation;
}

int cnz::Player::GetBashAnimationIndex() const
{
	return aniBash.index;
}

// sets the dra pbody
bool cnz::Player::SetDrawPBody(bool draw)
{
	if (this->GetPhysicsBodyCount() == 0) {
		drawPBody = false;
		return false;
	}
	else {
		drawPBody = true;
		return true;
	}
}

// gets the draw pbody.
bool cnz::Player::GetDrawPBody() const
{
	return drawPBody;
}

// gets pbody size
cherry::Vec3 cnz::Player::GetPBodySize() const
{
	return pBodySize;
}

// gets pbody weight
float cnz::Player::GetPBodyWidth() const
{
	return this->GetPBodySize().GetX() / 2;
}

// gets pbody height
float cnz::Player::GetPBodyHeight() const
{
	return this->GetPBodySize().GetY() / 2;
}

float cnz::Player::GetPBodyDepth() const
{
	return this->GetPBodySize().GetZ() / 2;
}

// gets the state
int cnz::Player::GetState() const
{
	return state;
}

// sets the state
void cnz::Player::SetState(int newState)
{
	state = newState;
}

// player update
void cnz::Player::Update(float deltaTime) {
	// Object::Update(deltaTime);

	// TODO: remove this for the final version.
	// rotation.SetX(rotation.GetX() + 15.0F * deltaTime);
	// rotation.SetZ(rotation.GetZ() + 90.0F * deltaTime);

	// already in Object update.
	// runs the path and sets the new position
	//if (followPath)
	//	position = path.Run(deltaTime);

	//// if the animation is playing
	//if (animations.GetCurrentAnimation() != nullptr) {
	//	animations.GetCurrentAnimation()->isPlaying();
	//	animations.GetCurrentAnimation()->Update(deltaTime);
	//}

	//// updating the physics bodies
	//for (cherry::PhysicsBody* body : bodies)
	//	body->Update(deltaTime);

	// if the dash is being charged.
	if (chargingDash)
	{
		// increments time
		dashChargeTimeCurr += dashChargeFactor * deltaTime;

		// maximum time reached.
		if (dashChargeTimeCurr > dashChargeTimeMax)
			dashChargeTimeCurr = dashChargeTimeMax;
	}

	Object::Update(deltaTime);
}