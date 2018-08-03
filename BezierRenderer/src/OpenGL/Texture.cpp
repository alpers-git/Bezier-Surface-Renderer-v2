#include "Texture.h"
#include "stb_image\stb_image.h"

Texture::Texture(const std::string & path, const std::string& uni_name)
	: m_RendererID(0), m_filepath(path), m_Localbuffer(nullptr), m_Width(0), m_Heigth(0), m_BPP(0), m_uniform_name(uni_name)
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

Texture& Texture::operator=(Texture rhs)
{
	if (this != &rhs)
	{
		m_BPP = rhs.m_BPP;
		m_filepath = rhs.m_filepath;
		m_Heigth = rhs.m_Heigth;
		//m_Localbuffer = nullptr;
		m_RendererID = rhs.m_RendererID;
		m_uniform_name = rhs.m_uniform_name;
		m_Width = rhs.m_Width;

		stbi_set_flip_vertically_on_load(1);
		m_Localbuffer = stbi_load(m_filepath.c_str(), &m_Width, &m_Heigth, &m_BPP, 4);

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
	return *this;
}

Texture::Texture(const Texture & other)
	:m_RendererID(other.m_RendererID),
	m_filepath(other.m_filepath),
	//m_Localbuffer(other.m_Localbuffer),
	m_Width(other.m_Width),
	m_Heigth(other.m_Heigth),
	m_BPP(other.m_BPP),
	m_uniform_name(other.m_uniform_name)
{
	stbi_set_flip_vertically_on_load(1);
	m_Localbuffer = stbi_load(m_filepath.c_str(), &m_Width, &m_Heigth, &m_BPP, 4);

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
