#include "Texture.h"
#include "stb_image\stb_image.h"

Texture::Texture(const std::string & path)
	: m_RendererID(0), m_filepath(path), m_Localbuffer(nullptr), m_Width(0), m_Heigth(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_Localbuffer = stbi_load(path.c_str(), &m_Width, &m_Heigth, &m_BPP, 4);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Heigth, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Localbuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_Localbuffer)
		stbi_image_free(m_Localbuffer);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
