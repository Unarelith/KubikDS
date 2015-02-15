/*
 * =====================================================================================
 *
 *       Filename:  Sprite.cpp
 *
 *    Description:  
 *
 *        Created:  15/02/2015 16:27:33
 *       Compiler:  gcc
 *
 *         Author:  Quentin Bazin, <gnidmoo@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#include "Sprite.hpp" 

Sprite::Sprite(int id, SpriteSize size, SpriteColorFormat format, s16 x, s16 y, OamState* oam) {
    m_id = id;
    
	m_gfx = oamAllocateGfx(m_oam, m_size, m_format); // Allocate memory space for sprite gfx
    
	m_size = size;
    m_format = format;
    
	m_x = x;
    m_y = y;
	
	m_oam = oam;
}

Sprite::Sprite(Sprite &&sprite) {
	m_id = sprite.m_id;
	
	m_gfx = sprite.m_gfx;
	sprite.m_gfx = nullptr;
	
	m_size = sprite.m_size;
	m_format = sprite.m_format;
	
	m_x = sprite.m_x;
	m_y = sprite.m_y;
	
	m_oam = sprite.m_oam;
}

Sprite::~Sprite() {
	if(m_gfx) oamFreeGfx(m_oam, m_gfx);
}

void Sprite::draw() {
    oamSet(m_oam, m_id, m_x, m_y, 0,
           15, m_size, m_format, m_gfx, -1,
           false, false, false, false, false);
}
