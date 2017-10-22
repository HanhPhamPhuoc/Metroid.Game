#include "Texture.h"

Texture::Texture()
{
	//set
	_Transcolor = D3DCOLOR_XRGB(255, 255, 255);
	_ImageSize.Set(0, 0);
	_Image = NULL;
}

Texture::Texture(std::string filePath)
{
	//set basic information
	_Transcolor = D3DCOLOR_XRGB(255, 255, 255);
	_FilePath = filePath;
	_Image = NULL;

	//get path from string
	std::wstring stemp = StringToWstring(filePath);
	LPCWSTR path = stemp.c_str();

	//Get image from file
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(path, &info);
	if (result != D3D_OK)
	{
		return;
	}

	//set size
	_ImageSize.Set(info.Width, info.Height);

	result = D3DXCreateTextureFromFileEx(
		d3ddevice,
		path,
		info.Width,
		info.Height,
		1,
		D3DPOOL_DEFAULT,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		_Transcolor,
		&info,
		NULL,
		&_Image);


	if (result != D3D_OK)
	{
		return;
	}


}


Texture::Texture(const Texture &texture)
{
	_Transcolor = texture._Transcolor;
	_ImageSize = texture._ImageSize;
	_FilePath = texture._FilePath;

	//get path from string
	std::wstring stemp = StringToWstring(_FilePath);
	LPCWSTR path = stemp.c_str();

	//Get image from file
	_Image = NULL;
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFile(path, &info);
	D3DXCreateTextureFromFileEx(
		d3ddevice,
		path,
		info.Width,
		info.Height,
		1,
		D3DPOOL_DEFAULT,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		_Transcolor,
		&info,
		NULL,
		&_Image);

}

Texture& Texture::operator=(const Texture &texture)
{
	_Transcolor = texture._Transcolor;
	_ImageSize = texture._ImageSize;	
	_FilePath = texture._FilePath;
	
	//get path from string
	std::wstring stemp = StringToWstring(_FilePath);
	LPCWSTR path = stemp.c_str();

	//Get image from file
	_Image = NULL;
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFile(path, &info);
	D3DXCreateTextureFromFileEx(
		d3ddevice,
		path,
		info.Width,
		info.Height,
		1,
		D3DPOOL_DEFAULT,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		_Transcolor,
		&info,
		NULL,
		&_Image);


	return *this;
}

//all get functions
D3DCOLOR Texture::GetTranscolor() const
{
	return _Transcolor;
}

LPDIRECT3DTEXTURE9 Texture::GetImage() const
{
	return _Image;
}

//all set functions
void Texture::SetTranscolor(D3DCOLOR transcolor)
{
	_Transcolor = transcolor;
}

Vector2 Texture::GetImageSize() const
{
	return _ImageSize;
}

std::wstring Texture::StringToWstring(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;

	return r;
}

Texture::~Texture()
{

	if (_Image != NULL)
	{
		_Image->Release();
		_Image = NULL;
	}

}
