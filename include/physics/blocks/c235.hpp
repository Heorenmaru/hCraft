/* 
 * hCraft - A custom Minecraft server.
 * Copyright (C) 2012	Jacob Zhitomirsky
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _hCraft__PHYSICS__C235_H_
#define _hCraft__PHYSICS__C235_H_

#include "physics/blocks/physics_block.hpp"


namespace hCraft {
	
	namespace physics {
		
		class c235: public physics_block
		{
		public:
			virtual int  id () override { return 45; }
			virtual blocki vanilla_block () override { return BT_BRICKS; }
			virtual int  tick_rate () override { return 40; }
			virtual const char* name () { return "c235"; }
			virtual bool affected_by_neighbours () { return true; }
		
			virtual void tick (world &w, int x, int y, int z, int extra,
				void *ptr, std::minstd_rand& rnd) override;
			virtual void on_neighbour_modified (world &w, int x, int y, int z,
				int nx, int ny, int nz) override;
		};
	}
}

#endif
