#pragma once

typedef struct _texture_info
{
	LPDIRECT3DTEXTURE9 pTexture;	// 이미지 한장을 제어하기 위한 컴객체. 
	D3DXIMAGE_INFO tImageInfo;		//이미지의 정보를 담아둘 구조체. 
}TextureInfo;