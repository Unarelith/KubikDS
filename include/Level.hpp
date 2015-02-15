/*
 * =====================================================================================
 *
 *       Filename:  Level.hpp
 *
 *    Description:  
 *
 *        Created:  15/02/2015 16:04:56
 *       Compiler:  gcc
 *
 *         Author:  Quentin Bazin, <gnidmoo@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef LEVEL_HPP_
#define LEVEL_HPP_

#include "LevelsData.hpp"

class Level {
	public:
		Level(int id, const Map &map);
		
		void initializeBg(int bg, bool main = true);
		
		void scroll(s32 x, s32 y, bool absolute = false);
		
		void resetScrolling();
		
		int id() const { return m_id; }
		
		u16 length() const { return m_map.length; }
		u16 height() const { return m_map.height; }
		
		s32 scrollX() const { return m_scrollX; }
		s32 scrollY() const { return m_scrollY; }
		
		Map &map() { return m_map; }
		
		void setKube(s16 x, s16 y, u16 tile);
		
		bool isKubeAt(s16 x, s16 y);
		bool isFinishAt(s16 x, s16 y);
		
		static int nbLevels;
		
	private:
		int m_id;
		
		Map m_map;
		
		int m_bg;
		
		s16 m_scrollX;
		s16 m_scrollY;
};

#endif // LEVEL_HPP_
