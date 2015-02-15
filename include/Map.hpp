/*
 * =====================================================================================
 *
 *       Filename:  Map.hpp
 *
 *    Description:  
 *
 *        Created:  15/02/2015 17:11:45
 *       Compiler:  gcc
 *
 *         Author:  Quentin Bazin, <gnidmoo@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef MAP_HPP_
#define MAP_HPP_

#include <vector>

#include "Enemy.hpp"

class Map {
	public:
		Map(int _id, u16 _length, u16 _height, u16 _color, s32 _scrollX, s32 _scrollY, s16 _playerX, s16 _playerY)
			: id(_id), length(_length), height(_height), color(_color),
			  scrollX(_scrollX), scrollY(_scrollY), playerX(_playerX), playerY(_playerY) {}
		
		int id = -1;
		
		u16 length = 0;
		u16 height = 0;
		
		u16 color = 0xFFFF;
		
		s32 scrollX = 0;
		s32 scrollY = 0;
		
		s16 playerX = 0;
		s16 playerY = 0;
		
		std::vector<u16> map;
		
		std::vector<Enemy> enemies;
};

#endif // MAP_HPP_
