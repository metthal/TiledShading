#include <glm/gtc/matrix_transform.hpp>

#include "shaders/imgui_pipeline.h"
#include "window/window.h"

const std::string imguiVertexShaderSource = R"glsl(
#version 430 core

uniform mat4 project;

in vec2 pos;
in vec2 uv;
in vec4 color;

out vec2 frag_uv;
out vec4 frag_color;

void main()
{
	frag_uv = uv;
	frag_color = color;
	gl_Position = project * vec4(pos, 0.0, 1.0);
}
)glsl";

const std::string imguiFragmentShaderSource = R"glsl(
#version 430 core

uniform sampler2D tex;

in vec2 frag_uv;
in vec4 frag_color;

out vec4 out_color;

void main()
{
	out_color =	frag_color * texture(tex, frag_uv);
	//out_color = vec4(1, 0, 0, 0.3);
}
)glsl";

ImguiPipeline::ImguiPipeline() : _imguiProgram(nullptr), _vao(0), _vbo(0), _ebo(0), _fontTexture(0)
{
}

ImguiPipeline::~ImguiPipeline()
{
	if (_vao != 0)
		glDeleteVertexArrays(1, &_vao);

	if (_vbo != 0)
		glDeleteBuffers(1, &_vbo);

	if (_ebo != 0)
		glDeleteBuffers(1, &_ebo);

	if (_fontTexture != 0)
		glDeleteTextures(1, &_fontTexture);
}

bool ImguiPipeline::init(const Window* window, std::string& error)
{
	ImGuiIO& io = ImGui::GetIO();
	unsigned char* pixels;
	int width, height;

	auto imguiVertexShader = Shader::loadString(GL_VERTEX_SHADER, imguiVertexShaderSource, error);
	if (!imguiVertexShader)
		return false;

	auto imguiFragmentShader = Shader::loadString(GL_FRAGMENT_SHADER, imguiFragmentShaderSource, error);
	if (!imguiFragmentShader)
		return false;

	_imguiProgram = ShaderProgram::link(error, imguiVertexShader, imguiFragmentShader);
	if (!_imguiProgram)
		return false;

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glVertexAttribPointer(_imguiProgram->getAttributeId("pos"), 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), reinterpret_cast<void*>(offsetof(ImDrawVert, pos)));
	glVertexAttribPointer(_imguiProgram->getAttributeId("uv"), 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), reinterpret_cast<void*>(offsetof(ImDrawVert, uv)));
	glVertexAttribPointer(_imguiProgram->getAttributeId("color"), 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), reinterpret_cast<void*>(offsetof(ImDrawVert, col)));
	glEnableVertexAttribArray(_imguiProgram->getAttributeId("pos"));
	glEnableVertexAttribArray(_imguiProgram->getAttributeId("uv"));
	glEnableVertexAttribArray(_imguiProgram->getAttributeId("color"));

	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	glGenTextures(1, &_fontTexture);
	glBindTexture(GL_TEXTURE_2D, _fontTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	io.Fonts->TexID = reinterpret_cast<void*>(static_cast<intptr_t>(_fontTexture));

	io.RenderDrawListsFn = imguiDrawCallback;
	io.UserData = this;

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
}

void ImguiPipeline::run(const Window* window)
{
	auto windowSize = window->getDimensions();

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(window->getDimensions().x), static_cast<float>(window->getDimensions().y));
	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(windowSize.x - 150.0f, 0.0f));
	ImGui::SetNextWindowSizeConstraints(ImVec2(150.0f, 50.0f), ImVec2(150.0f, 100.f));
	ImGui::Begin("General");
	ImGui::Text("Hello, world!");
	ImGui::End();

	ImGui::Render();
}

GLuint ImguiPipeline::getVertexArrayId() const
{
	return _vao;
}

GLuint ImguiPipeline::getVertexBufferId() const
{
	return _vbo;
}

GLuint ImguiPipeline::getElementBufferId() const
{
	return _ebo;
}

const std::shared_ptr<ShaderProgram>& ImguiPipeline::getImguiProgram() const
{
	return _imguiProgram;
}

void ImguiPipeline::imguiDrawCallback(ImDrawData* data)
{
	ImGuiIO& io = ImGui::GetIO();
	ImguiPipeline* pipeline = static_cast<ImguiPipeline*>(io.UserData);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glActiveTexture(GL_TEXTURE0);

	auto orthoProjection = glm::ortho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f);
	pipeline->getImguiProgram()->activate();
	pipeline->getImguiProgram()->setUniform("tex", 0);
	pipeline->getImguiProgram()->setUniform("project", orthoProjection);

	glBindVertexArray(pipeline->getVertexArrayId());
	for (int i = 0; i < data->CmdListsCount; ++i)
	{
		const ImDrawList* drawList = data->CmdLists[i];
		const ImDrawIdx* drawIdxBufferOffset = reinterpret_cast<const ImDrawIdx*>(0);

		glBindBuffer(GL_ARRAY_BUFFER, pipeline->getVertexBufferId());
		glBufferData(GL_ARRAY_BUFFER, drawList->VtxBuffer.Size * sizeof(ImDrawVert), drawList->VtxBuffer.Data, GL_STREAM_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pipeline->getElementBufferId());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, drawList->IdxBuffer.Size * sizeof(ImDrawIdx), drawList->IdxBuffer.Data, GL_STREAM_DRAW);

		for (int j = 0; j < drawList->CmdBuffer.Size; ++j)
		{
			const ImDrawCmd* cmd = &drawList->CmdBuffer[j];
			if (cmd->UserCallback)
				cmd->UserCallback(drawList, cmd);
			else
			{
				glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(reinterpret_cast<intptr_t>(cmd->TextureId)));
				glScissor(cmd->ClipRect.x, io.DisplaySize.y - cmd->ClipRect.w, cmd->ClipRect.z - cmd->ClipRect.x, cmd->ClipRect.w - cmd->ClipRect.y);
				glDrawElements(GL_TRIANGLES, cmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, drawIdxBufferOffset);
			}
			drawIdxBufferOffset += cmd->ElemCount;
		}
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	pipeline->getImguiProgram()->deactivate();

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_SCISSOR_TEST);
}