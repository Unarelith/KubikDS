/*
 * =====================================================================================
 *
 *       Filename:  Sprite.hpp
 *
 *    Description:  
 *
 *        Created:  15/02/2015 16:27:31
 *       Compiler:  gcc
 *
 *         Author:  Quentin Bazin, <gnidmoo@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef SPRITE_HPP_
#define SPRITE_HPP_

#include <nds.h>

class Sprite {
    public:
        Sprite(int id, SpriteSize size, SpriteColorFormat format, s16 x, s16 y, OamState* oam = &oamMain);
		Sprite(const Sprite &) = default;
		Sprite(Sprite &&sprite);
		~Sprite();
		
		//Sprite &operator=(const Sprite &) = delete;
		//Sprite &operator=(Sprite &&) = default;
		
        void draw();
		
        u16* gfx() const { return m_gfx; }
		
        s16 x() const { return m_x; }
        s16 y() const { return m_y; }
		
        int id() const { return m_id; }
        
        void setPosition(s16 x, s16 y) { m_x = x; m_y = y; }
    
    protected:
        int m_id = -1;
		
        u16* m_gfx = nullptr;
		
        SpriteSize m_size;
        SpriteColorFormat m_format;
		
        s16 m_x = 0;
        s16 m_y = 0;
		
		OamState* m_oam;
};

#endif // SPRITE_HPP_
