#include "PostLayer.h"
#include "..\utils\Utils.h"
#include "..\Game.h"


// post processing layer
cherry::PostLayer::PostLayer(const std::string vs, const std::string fs) { AddLayer(vs, fs); }

// post-processing layer
void cherry::PostLayer::AddLayer(const std::string vs, const std::string fs)
{
	// if the vertex sad'
	// if (!(util::fileAccessible(vs) && util::fileAccessible(fs)))
	// {
	// 
	// }

	Game* currGame = Game::GetRunningGame();

	// making the render buffer
	RenderBufferDesc mainColor = RenderBufferDesc();
	mainColor.ShaderReadable = true;
	mainColor.Attachment = RenderTargetAttachment::Color0;
	mainColor.Format = RenderTargetType::Color24;

	// making the vertices for the quad
	Vertex* verts = new Vertex[4]
	{
		//  {nx, ny, nz}, {r, g, b, a}, {nx, ny, nz}, {u, v}
		{{ -1.0F / 2.0F, -1.0F / 2.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {0.0F, 0.0F}}, // bottom left
		{{  1.0F / 2.0F, -1.0F / 2.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {1.0F, 0.0F}}, // bottom right
		{{ -1.0F / 2.0F,  1.0F / 2.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {0.0F, 1.0F}}, // top left
		{{  1.0F / 2.0F,  1.0F / 2.0F, 0.0f }, { 1.0F, 1.0F, 1.0F, 1.0F }, {0.0F, 0.0F, 1.0F}, {1.0F, 1.0F}}, // top right
	};

	// making the indices for the quad
	uint32_t* indices = new uint32_t[6]{
		0, 1, 2,
		2, 1, 3
	};

	// full screen quadrilateral
	myFullscreenQuad = std::make_shared<cherry::Mesh>(verts, 4, indices, 6);

	// making the shader
	Shader::Sptr shader = std::make_shared<Shader>();
	shader->Load(vs.c_str(), fs.c_str());
	// shader->Link();

	// making the output of the framebuffer
	cherry::FrameBuffer::Sptr output = std::make_shared<cherry::FrameBuffer>(currGame->GetWindowWidth(), currGame->GetWindowHeight());
	output->AddAttachment(mainColor);
	output->Validate();

	// Add the pass to the post processing stack
	myPasses.push_back({ shader, output });
}

// resizes the layers
void cherry::PostLayer::OnWindowResize(uint32_t width, uint32_t height)
{
	for (auto& pass : myPasses)
		pass.Output->Resize(width, height);
}

// renders the post layer
void cherry::PostLayer::PostRender()
{
	// gets the game being run for its screen size.
	Game* game = Game::GetRunningGame();
	// auto& ecs = CurrentRegistry();

	
	// We grab the application singleton to get the size of the screen
	// ecs.ctx_or_set<AppFrameState>();
	// CurrentRegistry().ctx_or_set<FrameBuffer::Sptr>();
	FrameBuffer::Sptr mainBuffer = CurrentRegistry().ctx<FrameBuffer::Sptr>();

	glDisable(GL_DEPTH_TEST);
	// The last output will start as the output from the rendering
	FrameBuffer::Sptr lastPass = mainBuffer;

	// We'll iterate over all of our render passes
	for (const PostPass& pass : myPasses) {

		// We'll bind our post-processing output as the current render target and clear it
		pass.Output->Bind(RenderTargetBinding::Draw);
		glClear(GL_COLOR_BUFFER_BIT);
		// Set the viewport to be the entire size of the passes output
		glViewport(0, 0, pass.Output->GetWidth(), pass.Output->GetHeight());

		// Use the post processing shader to draw the fullscreen quad
		// pass.Shader->Use(); // TODO: add this function?
		
		lastPass->GetAttachment(RenderTargetAttachment::Color0)->Bind(0);
		pass.Shader->SetUniform("xImage", 0);
		pass.Shader->SetUniform("xScreenRes", glm::vec2(game->GetWindowSize()));

		myFullscreenQuad->Draw();

		// Unbind the output pass so that we can read from it
		pass.Output->UnBind();
		// Update the last pass output to be this passes output
		lastPass = pass.Output;
	}

	// Bind the last buffer we wrote to as our source for read operations
	lastPass->Bind(RenderTargetBinding::Read);

	// Copies the image from lastPass into the default back buffer
	FrameBuffer::Blit({ 0, 0, lastPass->GetWidth(), lastPass->GetHeight() },
		{ 0, 0, game->GetWindowWidth(), game->GetWindowHeight() }, BufferFlags::All, MagFilter::Nearest);

	// Unbind the last buffer from read operations, so we can write to it again later
	lastPass->UnBind();
}
