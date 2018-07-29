#pragma once
#include "Renderer.h"
#include <string>

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_filepath;
	unsigned char* m_Localbuffer;
	int m_Width, m_Heigth, m_BPP;
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot=0) const;
	void Unbind() const;

	int inline GetWidth() const { return m_Width;}
	int inline GetHeigth() const { return m_Heigth; }
};