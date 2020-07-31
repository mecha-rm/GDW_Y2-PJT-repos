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

cnz::Player::Player(const Player& obj) : Object(obj) 
{
	position = obj.GetPosition();
	dash = obj.dash;
	dashDist = obj.dashDist;
	dashTime = obj.dashTime;

	// cT = obj.cT;
	// cB = obj.cB;
	// cL = obj.cL;
	// cR = obj.cR;

	pBodySize = obj.pBodySize;
	drawPBody = obj.drawPBody;
	
	degreeAngle = obj.degreeAngle;
	radianAngle = obj.radianAngle;

	worldAngle = obj.worldAngle;

	playerCurMovement = obj.playerCurMovement;
	state = obj.state;

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
void cnz::Player::SetDash(bool dash) { this->dash = dash; }

// generates the default character.
cnz::Player* cnz::Player::GenerateDefault(std::string scene, cherry::Vec3 position)
{
	// TODO: the player object gets remade in CNZ_GameplayScene, so this doesn't work.
	// that will need to be fixed.
	using namespace cherry;

	Player* plyr = new Player("res/objects/hero_ver.2/One_T.obj", scene, position);

	Vec3 dmns = plyr->GetMeshBodyMaximum() - plyr->GetMeshBodyMinimum();
	plyr->AddPhysicsBody(new PhysicsBodyBox(dmns / 2.0F, Vec3(2, 2, 3)));

	// setting emissive colour and power
	plyr->SetEmissiveColor(Vec3(0.0F, 1.0F, 1.0F));
	plyr->SetEmissivePower(0.10F);

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
		// full
		// these take WAY to long to add, but they work.
		// animations
		cnz::AnimStruct playerCharging = { 26, "res/objects/anims/player/Attack_Charge/One_Charge_0000", 1.08f, new cherry::MorphAnimation() };
		
		// this seems to be a leftover from an earlier build
		cnz::AnimStruct playerCharged = { 26, "res/objects/anims/player/Attack_Charged/One_Charge_0000", 0.01f, new cherry::MorphAnimation() };
		
		cnz::AnimStruct playerDashing = { 26, "res/objects/anims/player/Attack_Dash/One_Dash_0000", 0.27f, new cherry::MorphAnimation() };
		cnz::AnimStruct playerRun_F = { 24, "res/objects/anims/player/Run_F/One_RF_0000", 1.0f, new cherry::MorphAnimation() };
		cnz::AnimStruct playerRun_B = { 24, "res/objects/anims/player/Run_B/One_RB_0000", 1.0f, new cherry::MorphAnimation() };
		cnz::AnimStruct playerRun_L = { 24, "res/objects/anims/player/Run_L/One_RL_0000", 1.0f, new cherry::MorphAnimation() };
		cnz::AnimStruct playerRun_R = { 24, "res/objects/anims/player/Run_R/One_RR_0000", 1.0f, new cherry::MorphAnimation() };
		cnz::AnimStruct playerDodge_F = { 12, "res/objects/anims/player/Dodge_F/One_DF_0000", 0.46f, new cherry::MorphAnimation() };
		cnz::AnimStruct playerDodge_B = { 12, "res/objects/anims/player/Dodge_B/One_DB_0000", 0.46f, new cherry::MorphAnimation() };
		cnz::AnimStruct playerDodge_L = { 12, "res/objects/anims/player/Dodge_L/One_DL_0000", 0.46f, new cherry::MorphAnimation() };
		cnz::AnimStruct playerDodge_R = { 12, "res/objects/anims/player/Dodge_R/One_DR_0000", 0.46f, new cherry::MorphAnimation() };
		cnz::AnimStruct playerDeath = { 52, "res/objects/anims/player/Took_Damage/One_Die_0000", 2.16f, new cherry::MorphAnimation() };
		cnz::AnimStruct playerSpecial = { 37, "res/objects/anims/player/Special_Attack/Special_Attack_0000", 1.54f, new cherry::MorphAnimation() };
		cnz::AnimStruct playerBash = { 16, "res/objects/anims/player/Bash/One_Bash_0000", 0.63f, new cherry::MorphAnimation() };

		// add animations to player.

		// charging - 26 total frames
		// pretty sure this animation was never set.
		plyr->AddAnimation(playerCharging.anim);
		playerCharging.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000000.obj", 1.08f));
		playerCharging.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000006.obj", 1.08f));
		playerCharging.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000012.obj", 1.08f));
		playerCharging.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000018.obj", 1.08f));
		playerCharging.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000025.obj", 1.08f));

		// seems like this was a leftover.
		// attack charged animation - 26 frames
		plyr->AddAnimation(playerCharged.anim);
		playerCharged.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charged/One_Charge_000000.obj", 0.01f));
		// playerCharged.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charged/One_Charge_000006.obj", 0.01f));
		// playerCharged.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charged/One_Charge_000012.obj", 0.01f));
		// playerCharged.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charged/One_Charge_000018.obj", 0.01f));
		// playerCharged.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charged/One_Charge_000025.obj", 0.01f));

		// dashing animation - 26 frames
		plyr->AddAnimation(playerDashing.anim);
		playerDashing.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Dash/One_Dash_000000.obj", 0.27f));
		playerDashing.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Dash/One_Dash_000006.obj", 0.27f));
		playerDashing.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Dash/One_Dash_000012.obj", 0.27f));
		playerDashing.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Dash/One_Dash_000018.obj", 0.27f));
		playerDashing.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Dash/One_Dash_000025.obj", 0.27f));
		
		// running animation forward - 24 frames
		plyr->AddAnimation(playerRun_F.anim);
		playerRun_F.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_F/One_RF_000000.obj", 1.0f));
		playerRun_F.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_F/One_RF_000006.obj", 1.0f));
		playerRun_F.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_F/One_RF_000012.obj", 1.0f));
		playerRun_F.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_F/One_RF_000018.obj", 1.0f));
		playerRun_F.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_F/One_RF_000023.obj", 1.0f));

		// running animation backward - 24 frames
		plyr->AddAnimation(playerRun_B.anim);
		playerRun_B.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_B/One_RB_000000.obj", 1.0f));
		playerRun_B.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_B/One_RB_000006.obj", 1.0f));
		playerRun_B.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_B/One_RB_000012.obj", 1.0f));
		playerRun_B.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_B/One_RB_000018.obj", 1.0f));
		playerRun_B.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_B/One_RB_000023.obj", 1.0f));

		// running animation left - 24 frames
		plyr->AddAnimation(playerRun_L.anim);
		playerRun_L.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_L/One_RL_000000.obj", 1.0f));
		playerRun_L.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_L/One_RL_000006.obj", 1.0f));
		playerRun_L.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_L/One_RL_000012.obj", 1.0f));
		playerRun_L.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_L/One_RL_000018.obj", 1.0f));
		playerRun_L.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_L/One_RL_000023.obj", 1.0f));
		
		// running animation right - 24 frames
		plyr->AddAnimation(playerRun_R.anim);
		playerRun_R.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_R/One_RR_000000.obj", 1.0f));
		playerRun_R.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_R/One_RR_000006.obj", 1.0f));
		playerRun_R.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_R/One_RR_000012.obj", 1.0f));
		playerRun_R.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_R/One_RR_000018.obj", 1.0f));
		playerRun_R.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Run_R/One_RR_000023.obj", 1.0f));
		
		// dodge animation forward - 12 frames
		plyr->AddAnimation(playerDodge_F.anim);
		playerDodge_F.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_F/One_DF_000000.obj", 0.46f));
		playerDodge_F.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_F/One_DF_000004.obj", 0.46f));
		playerDodge_F.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_F/One_DF_000008.obj", 0.46f));
		playerDodge_F.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_F/One_DF_000011.obj", 0.46f));

		// dodge animation backward - 12 frames
		plyr->AddAnimation(playerDodge_B.anim);
		playerDodge_B.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_B/One_DB_000000.obj", 0.46f));
		playerDodge_B.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_B/One_DB_000004.obj", 0.46f));
		playerDodge_B.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_B/One_DB_000008.obj", 0.46f));
		playerDodge_B.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_B/One_DB_000011.obj", 0.46f));

		// dodge animation left - 12 frames
		plyr->AddAnimation(playerDodge_L.anim);
		playerDodge_L.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_L/One_DL_000000.obj", 0.46f));
		playerDodge_L.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_L/One_DL_000004.obj", 0.46f));
		playerDodge_L.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_L/One_DL_000008.obj", 0.46f));
		playerDodge_L.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_L/One_DL_000011.obj", 0.46f));

		// dodge animation right - 12 frames
		plyr->AddAnimation(playerDodge_R.anim);
		playerDodge_R.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_R/One_DR_000000.obj", 0.46f));
		playerDodge_R.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_R/One_DR_000004.obj", 0.46f));
		playerDodge_R.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_R/One_DR_000008.obj", 0.46f));
		playerDodge_R.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Dodge_R/One_DR_000011.obj", 0.46f));

		// player death - 52 frames
		plyr->AddAnimation(playerDeath.anim);
		playerDeath.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Took_Damage/One_Die_000000.obj", 0.46f));
		playerDeath.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Took_Damage/One_Die_000010.obj", 0.46f));
		playerDeath.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Took_Damage/One_Die_000020.obj", 0.46f));
		playerDeath.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Took_Damage/One_Die_000030.obj", 0.46f));
		playerDeath.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Took_Damage/One_Die_000040.obj", 0.46f));
		playerDeath.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Took_Damage/One_Die_000050.obj", 0.46f));

		// player special - 37 frames
		plyr->AddAnimation(playerSpecial.anim);
		playerSpecial.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Special_Attack/Special_Attack_000000.obj", 1.54f));
		playerSpecial.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Special_Attack/Special_Attack_000007.obj", 1.54f));
		playerSpecial.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Special_Attack/Special_Attack_000014.obj", 1.54f));
		playerSpecial.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Special_Attack/Special_Attack_000021.obj", 1.54f));
		playerSpecial.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Special_Attack/Special_Attack_000028.obj", 1.54f));
		playerSpecial.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Special_Attack/Special_Attack_000035.obj", 1.54f));
		
		// player bash - 16 frames
		plyr->AddAnimation(playerBash.anim);
		playerBash.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Bash/One_Bash_000000.obj", 0.63f));
		playerBash.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Bash/One_Bash_000004.obj", 0.63f));
		playerBash.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Bash/One_Bash_000008.obj", 0.63f));
		playerBash.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Bash/One_Bash_000012.obj", 0.63f));
		playerBash.anim->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Bash/One_Bash_000015.obj", 0.63f));
	
	}

	return plyr;
}

// gets whether the player is currently dashing.
bool cnz::Player::IsDashing() const { return dash; }

// gets the dash time.
float cnz::Player::GetDashTime() const { return dashTime; }

// sets dash time for the player
void cnz::Player::SetDashTime(float dashTime) { this->dashTime = dashTime; }


// TODO: holdovers from object class. May not be needed?
float cnz::Player::GetDegreeAngle() const { return degreeAngle; }

float cnz::Player::GetRadianAngle() const { return radianAngle; }

glm::vec3 cnz::Player::GetVec3Angle() const { return this->worldAngle; }

void cnz::Player::UpdateAngle(cherry::Camera::Sptr camera, double xpos, double ypos, unsigned int width, unsigned int height) {

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

void cnz::Player::Update(float deltaTime) {
	// Object::Update(deltaTime);
	if (this == nullptr) {
		return;
	}

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

	Object::Update(deltaTime);
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

glm::vec3 cnz::Player::GetDash(float dist) const {
	glm::vec3 dash;
	// glm is backwards so both need to be negative to dash in the correct direction.
	// afaik GetDegreeAngle() is not working correctly so we use Radians converted to degrees instead.
	dash.x = -(dist * glm::degrees(cosf(this->GetRadianAngle())));
	dash.y = -(dist * glm::degrees(sinf(this->GetRadianAngle())));
	dash.z = 0.0f;

	return dash;
}

bool cnz::Player::SetDrawPBody(bool draw)
{
	if (this->GetPhysicsBodyCount() == 0) {
		this->drawPBody = false;
		return false;
	}
	else {
		this->drawPBody = true;
		return true;
	}
}

bool cnz::Player::GetDrawPBody() const
{
	return this->drawPBody;
}

cherry::Vec3 cnz::Player::GetPBodySize() const
{
	return this->pBodySize;
}

float cnz::Player::GetPBodyWidth() const
{
	return this->GetPBodySize().GetX() / 2;
}

float cnz::Player::GetPBodyHeight() const
{
	return this->GetPBodySize().GetY() / 2;
}

float cnz::Player::GetPBodyDepth() const
{
	return this->GetPBodySize().GetZ() / 2;
}
