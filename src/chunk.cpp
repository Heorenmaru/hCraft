/* 
 * hCraft - A custom Minecraft server.
 * Copyright (C) 2012-2013	Jacob Zhitomirsky (BizarreCake)
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

#include "chunk.hpp"
#include "world.hpp"
#include <cstring>

#include <iostream> // DEBUG


namespace hCraft {
	
	/* 
	 * Constructs a new empty subchunk, with all blocks set to air.
	 */
	subchunk::subchunk (bool init)
	{
		if (init)
			{
				std::memset (this->ids, 0x00, 4096);
				std::memset (this->meta, 0x00, 2048);
				std::memset (this->blight, 0x00, 2048);
				std::memset (this->slight, 0xFF, 2048);
				this->add = nullptr;
				std::memset (this->extra, 0x00, 4096);
		
				this->add_count = 0;
				this->air_count = 4096;
				
				for (int i = 0; i < 128; ++i)
					this->custom[i] = 0;
			}
	}
	
	/* 
	 * Copy constructor.
	 */
	subchunk::subchunk (const subchunk& sub)
	{
		std::memcpy (this->ids, sub.ids, 4096);
		std::memcpy (this->meta, sub.meta, 2048);
		std::memcpy (this->blight, sub.blight, 2048);
		std::memcpy (this->slight, sub.slight, 2048);
		if (sub.add)
			{
				this->add = new unsigned char[2048];
				std::memcpy (this->add, sub.add, 2048);
			}
		else
			this->add = nullptr;
		
		this->add_count = sub.add_count;
		this->air_count = sub.air_count;
		
		for (int i = 0; i < 128; ++i)
			this->custom[i] = sub.custom[i];;
	}
	
	/* 
	 * Class destructor.
	 */
	subchunk::~subchunk ()
	{
		delete[] this->add;
	}
	
	
	
//----
	
	void
	subchunk::set_id (int x, int y, int z, unsigned short id)
	{
		unsigned int index = (y << 8) | (z << 4) | x; 
		unsigned int half = index >> 1;
		
		unsigned short lo = id & 0xFF; // the lower 8 bits of the ID.
		unsigned short hi = id >> 8;   // the upper 4 bits of the ID (the "add").
		unsigned short prev_lo = this->ids[index];
		unsigned short prev_hi =
			(this->add_count > 0)
				? ((index & 1) ? (this->add[half] >> 4) : (this->add[half] & 0xF))
				: 0;
		unsigned short prev_id = (prev_hi << 8) | prev_lo;
		
		this->ids[index] = lo;
		if (hi != 0 && this->add_count == 0)
			{
				this->add = new unsigned char[2048];
				std::memset (this->add, 0x00, 2048);
			}
		
		if (this->add)
			{
				if (index & 1)
					{ this->add[half] &= 0x0F; this->add[half] |= (hi << 4); }
				else
					{ this->add[half] &= 0xF0; this->add[half] |= hi; }
			}
		
		if (prev_id && !id)
			++ this->air_count;
		else if (!prev_id && id)
			-- this->air_count;
		
		if (prev_hi && !hi)
			-- this->add_count;
		else if (!prev_hi && hi)
			++ this->add_count;
		
		// custom block stuff
		if (block_info::is_vanilla_id (id))
			{
				this->custom[index >> 5] &= ~(1 << (index & 0x1F));
			}
		else
			{
				this->custom[index >> 5] |=  (1 << (index & 0x1F));
			}
		
		this->extra[index] = 0; // TODO: modify?
	}
	
	unsigned short
	subchunk::get_id (int x, int y, int z)
	{
		unsigned int index = (y << 8) | (z << 4) | x;
		
		unsigned short id = this->ids[index];
		if (this->add_count > 0)
			{
				unsigned int half = index >> 1;
				id |=
					((index & 1) ? (this->add[half] >> 4) : (this->add[half] & 0xF))
					<< 8;
			}
		
		return id;
	}
	
	
	void
	subchunk::set_extra (int x, int y, int z, unsigned char e)
	{
		this->extra[(y << 8) | (z << 4) | x] = e;
	}
	
	unsigned char
	subchunk::get_extra (int x, int y, int z)
	{
		return this->extra[(y << 8) | (z << 4) | x];
	}
	
	
	void
	subchunk::set_meta (int x, int y, int z, unsigned char val)
	{
		unsigned int index = (y << 8) | (z << 4) | x; 
		unsigned int half = index >> 1;
		
		if (index & 1)
			{ this->meta[half] &= 0x0F; this->meta[half] |= (val << 4); }
		else
			{ this->meta[half] &= 0xF0; this->meta[half] |= val; }
	}
	
	unsigned char
	subchunk::get_meta (int x, int y, int z)
	{
		unsigned int index = (y << 8) | (z << 4) | x; 
		unsigned int half = index >> 1;
		
		return (index & 1)
			? (this->meta[half] >> 4)
			: (this->meta[half] & 0xF);
	}
	
	
	void
	subchunk::set_block_light (int x, int y, int z, unsigned char val)
	{
		unsigned int index = (y << 8) | (z << 4) | x; 
		unsigned int half = index >> 1;
		
		if (index & 1)
			{ this->blight[half] &= 0x0F; this->blight[half] |= (val << 4); }
		else
			{ this->blight[half] &= 0xF0; this->blight[half] |= val; }
	}
	
	unsigned char
	subchunk::get_block_light (int x, int y, int z)
	{
		unsigned int index = (y << 8) | (z << 4) | x; 
		unsigned int half = index >> 1;
		
		return (index & 1)
			? (this->blight[half] >> 4)
			: (this->blight[half] & 0xF);
	}
	
	
	void
	subchunk::set_sky_light (int x, int y, int z, unsigned char val)
	{
		unsigned int index = (y << 8) | (z << 4) | x; 
		unsigned int half = index >> 1;
		
		if (index & 1)
			{ this->slight[half] &= 0x0F; this->slight[half] |= (val << 4); }
		else
			{ this->slight[half] &= 0xF0; this->slight[half] |= val; }
	}
	
	unsigned char
	subchunk::get_sky_light (int x, int y, int z)
	{
		unsigned int index = (y << 8) | (z << 4) | x; 
		unsigned int half = index >> 1;
		
		return (index & 1)
			? (this->slight[half] >> 4)
			: (this->slight[half] & 0xF);
	}
	
	
	void
	subchunk::set_block (int x, int y, int z, unsigned short id, unsigned char meta, unsigned char ex)
	{
		unsigned int index = (y << 8) | (z << 4) | x; 
		unsigned int half = index >> 1;
		
		if (index & 1)
			{ this->meta[half] &= 0x0F; this->meta[half] |= (meta << 4); }
		else
			{ this->meta[half] &= 0xF0; this->meta[half] |= meta; }
		
		unsigned short lo = id & 0xFF; // the lower 8 bits of the ID.
		unsigned short hi = id >> 8;   // the upper 4 bits of the ID (the "add").
		unsigned short prev_lo = this->ids[index];
		unsigned short prev_hi =
			(this->add_count > 0)
				? ((index & 1) ? (this->add[half] >> 4) : (this->add[half] & 0xF))
				: 0;
		unsigned short prev_id = (prev_hi << 8) | prev_lo;

		this->ids[index] = lo;
		if (hi != 0 && this->add_count == 0)
			{
				this->add = new unsigned char[2048];
				std::memset (this->add, 0x00, 2048);
			}
		
		if (add)
			{
				if (index & 1)
					{ this->add[half] &= 0x0F; this->add[half] |= (hi << 4); }
				else
					{ this->add[half] &= 0xF0; this->add[half] |= hi; }
			}

		if (prev_id && !id)
			++ this->air_count;
		else if (!prev_id && id)
			-- this->air_count;

		if (prev_hi && !hi)
			-- this->add_count;
		else if (!prev_hi && hi)
			++ this->add_count;
		
		// custom block stuff
		if (block_info::is_vanilla_id (id))
			{
				this->custom[index >> 5] &= ~(1 << (index & 0x1F));
			}
		else
			{
				this->custom[index >> 5] |=  (1 << (index & 0x1F));
			}
		
		this->extra[index] = ex;
	}
	
	
	block_data
	subchunk::get_block (int x, int y, int z)
	{
		block_data data {};
		unsigned int index = (y << 8) | (z << 4) | x;
		unsigned int half = index >> 1;
		
		data.id = this->ids[index];
		if (this->add_count > 0)
			{
				data.id |=
					((index & 1) ? (this->add[half] >> 4) : (this->add[half] & 0xF))
					<< 8;
			}
		
		data.ex = this->extra[index];
		
		if (index & 1)
			{
				data.meta = this->meta[half] >> 4;
				data.bl = this->blight[half] >> 4;
				data.sl = this->slight[half] >> 4;
			}
		else
			{
				data.meta = this->meta[half] & 0xF;
				data.bl = this->blight[half] & 0xF;
				data.sl = this->slight[half] & 0xF;
			}
		
		return data;
	}
	
	
	
//----
	
	/* 
	 * Constructs a new empty chunk, with all blocks set to air.
	 */
	chunk::chunk ()
	{ 
		for (int i = 0; i < 16; ++i)
			{
				this->subs[i] = nullptr;
			}
		
		for (int i = 0; i < 256; ++i)
			this->heightmap[i] = 0;
		
		std::memset (this->biomes, BI_PLAINS, 256);
		this->modified = true;
		this->generated = false;
		
		this->north = this->south = this->east = this->west = nullptr;
	}
	
	/* 
	 * Class destructor.
	 */
	chunk::~chunk ()
	{
		for (int i = 0; i < 16; ++i)
			{
				if (this->subs[i])
					delete this->subs[i];
			}
	}
	
	
	
	/* 
	 * Creates (if does not already exist) and returns the sub-chunk located at
	 * the given vertical position.
	 */
	subchunk*
	chunk::create_sub (int index, bool init)
	{
		subchunk *sub = this->get_sub (index);
		if (sub) return sub;
		
		return (this->subs[index] = new subchunk (init));
	}
	
	
	
//----
	
	void
	chunk::set_id (int x, int y, int z, unsigned short id)
	{
		int sy = y >> 4;
		subchunk *sub = this->subs[sy];
		if (!sub)
			{
				if (id == 0)
					return;
				else
					sub = this->subs[sy] = new subchunk ();
			}
		
		this->modified = true;
		sub->set_id (x, y & 0xF, z, id);
	}
	
	unsigned short
	chunk::get_id (int x, int y, int z)
	{
		int sy = y >> 4;
		subchunk *sub = this->subs[sy];
		if (!sub)
			return 0;
		
		return sub->get_id (x, y & 0xF, z);
	}
	
	
	void
	chunk::set_extra (int x, int y, int z, unsigned char e)
	{
		int sy = y >> 4;
		subchunk *sub = this->subs[sy];
		if (!sub)
			{
				if (e == 0)
					return;
				else
					sub = this->subs[sy] = new subchunk ();
			}
		
		this->modified = true;
		sub->set_extra (x, y & 0xF, z, e);
	}
	
	unsigned char
	chunk::get_extra (int x, int y, int z)
	{
		int sy = y >> 4;
		subchunk *sub = this->subs[sy];
		if (!sub)
			return 0;
		
		return sub->get_extra (x, y & 0xF, z);
	}
	
	
	void
	chunk::set_meta (int x, int y, int z, unsigned char val)
	{
		int sy = y >> 4;
		subchunk *sub = this->subs[sy];
		if (!sub)
			{
				if (val == 0)
					return;
				else
					sub = this->subs[sy] = new subchunk ();
			}
		
		//if (sub->get_meta (x, y & 0xF, z) != val)
			this->modified = true;
		sub->set_meta (x, y & 0xF, z, val);
	}
	
	unsigned char
	chunk::get_meta (int x, int y, int z)
	{
		int sy = y >> 4;
		subchunk *sub = this->subs[sy];
		if (!sub)
			return 0;
		
		return sub->get_meta (x, y & 0xF, z);
	}
	
	
	void
	chunk::set_block_light (int x, int y, int z, unsigned char val)
	{
		int sy = y >> 4;
		subchunk *sub = this->subs[sy];
		if (!sub)
			{
				if (val == 0)
					return;
				else
					sub = this->subs[sy] = new subchunk ();
			}
		
		//if (sub->get_block_light (x, y & 0xF, z) != val)
			this->modified = true;
		sub->set_block_light (x, y & 0xF, z, val);
	}
	
	unsigned char
	chunk::get_block_light (int x, int y, int z)
	{
		int sy = y >> 4;
		subchunk *sub = this->subs[sy];
		if (!sub)
			return 0;
		
		return sub->get_block_light (x, y & 0xF, z);
	}
	
	
	void
	chunk::set_sky_light (int x, int y, int z, unsigned char val)
	{
		int sy = y >> 4;
		subchunk *sub = this->subs[sy];
		if (!sub)
			{
				if (val == 0xF)
					return;
				else
					sub = this->subs[sy] = new subchunk ();
			}
		
		//if (sub->get_sky_light (x, y & 0xF, z) != val)
			this->modified = true;
		sub->set_sky_light (x, y & 0xF, z, val);
	}
	
	unsigned char
	chunk::get_sky_light (int x, int y, int z)
	{
		int sy = y >> 4;
		subchunk *sub = this->subs[sy];
		if (!sub)
			return 0xF;
		
		return sub->get_sky_light (x, y & 0xF, z);
	}
	
	
	void
	chunk::set_block (int x, int y, int z, unsigned short id, unsigned char meta, unsigned char ex)
	{
		int sy = y >> 4;
		subchunk *sub = this->subs[sy];
		if (!sub)
			{
				if (id == 0 && ex == 0) // && meta == 0
					return;
				else
					sub = this->subs[sy] = new subchunk ();
			}
		
		this->modified = true;
		sub->set_block (x, y & 0xF, z, id, meta, ex);
	}
	
	
	block_data
	chunk::get_block (int x, int y, int z)
	{
		int sy = y >> 4;
		if (sy >= 16 || sy < 0)
			std::cout << "sy PROBLEM!" << std::endl;
		subchunk *sub = this->subs[sy];
		if (!sub)
			return block_data ();
		
		return sub->get_block (x, y & 0xF, z);
	}
	
	 
	
//----
	
	/* 
	 * Lighting (re)calculation.
	 */
	/*
	void
	chunk::relight ()
	{
		for (int x = 0; x < 16; ++x)
			for (int z = 0; z < 16; ++z)
				{
					// the top-most layer is always lit.
					this->set_sky_light (x, 255, z, 15);
		
					char curr_opacity = 15;
					for (int y = 254; y >= 0; --y)
						{
							if (curr_opacity > 0)
								curr_opacity -= block_info::from_id (this->get_id (x, y, z))->opacity;
							this->set_sky_light (x, y, z, (curr_opacity > 0) ? curr_opacity : 0);
						}
				}
	}*/
	
	
	
	/* 
	 * (Re)creates the chunk's heightmap.
	 */
	 
	short
	chunk::recalc_heightmap (int x, int z)
	{
		short h;
		for (h = 255; h >= 0; --h)
			{ 
				block_info *binf = block_info::from_id (this->get_id (x, h, z));
				if (binf->state == BS_SOLID && binf->opaque)
					break;
			}
		this->set_height (x, z, h + 1);
		return h + 1;
	}
	
	void
	chunk::recalc_heightmap ()
	{
		for (int x = 0; x < 16; ++x)
			for (int z = 0; z < 16; ++z)
				this->recalc_heightmap (x, z);
	}
	
	
	
//----
	
	/* 
	 * Adds the specified entity to the chunk's entity list.
	 */
	void
	chunk::add_entity (entity *e)
	{
		std::lock_guard<std::mutex> guard {this->entity_lock};
		this->entities.insert (e);
	}
	
	/* 
	 * Removes the given entity from the chunk's entity list.
	 */
	void
	chunk::remove_entity (entity *e)
	{
		std::lock_guard<std::mutex> guard {this->entity_lock};
		this->entities.erase (e);
	}
	
	/* 
	 * Calls the specified function on every entity in the chunk's entity list.
	 */
	void
	chunk::all_entities (std::function<void (entity *)> f)
	{
		std::lock_guard<std::mutex> guard {this->entity_lock};
		for (entity* e : this->entities)
			f (e);
	}
	
	
	
//----
	
	/* 
	 * Returns a copy of this chunk.
	 * NOTE: Only block data is copied.
	 */
	chunk*
	chunk::duplicate ()
	{
		chunk *ch = new chunk ();
		
		for (int i = 0; i < 16; ++i)
			{
				subchunk *sub = this->subs[i];
				if (sub)
					ch->subs[i] = new subchunk (*sub);
			}
		
		std::memcpy (ch->biomes, this->biomes, 256);
		//std::memcpy (ch->heightmap, this->heightmap, 1024);
		return ch;
	}
	
	
	
//------------------------------------------------------------------------------
	
	chunk_link_map::chunk_link_map (world &wr, chunk *center, int cx, int cz)
		: wr (wr)
	{
		this->last = this->center = {cx, cz, center};
	}
	
	
	
	chunk*
	chunk_link_map::follow (int cx, int cz)
	{
		if (!this->wr.chunk_in_bounds (cx, cz))
			return nullptr;
		
		int lx = this->last.x, lz = this->last.z;
		chunk *ch = this->last.ch;
		if (cx != lx || cz != lz)
			{
				// follow through chunk links
				while (lx != cx || lz != cz)
					{
						if (lx < cx) {
							++ lx;
							ch = ch->east;
						} else if (lx > cx) {
							-- lx;
							ch = ch->west;
						}
						if (!ch) break;
						 
						if (lz < cz) {
							++ lz;
							ch = ch->south;
						} else if (lz > cz) {
							-- lz;
							ch = ch->north;
						}
						if (!ch) break;
					}
				
				if (!ch)
					{
						ch = this->wr.get_chunk (cx, cz);
						if (!ch)
							{
								// place empty chunk
								ch = new chunk ();
								this->wr.put_chunk (cx, cz, ch);
							}
					}

				this->last.ch = ch;
				this->last.x = cx;
				this->last.z = cz;
			}
		
		if (ch == this->wr.get_edge_chunk ())
			return nullptr; // shouldn't happen...
		return ch;
	}
	
	void
	chunk_link_map::set (int x, int y, int z, unsigned char id, unsigned char meta, unsigned char ex)
	{
		chunk *ch = this->follow (x >> 4, z >> 4);
		if (ch)
			ch->set_block (x & 0xF, y, z & 0xF, id, meta, ex);
	}
	 
	unsigned short
	chunk_link_map::get_id (int x, int y, int z)
	{
		chunk *ch = this->follow (x >> 4, z >> 4);
		if (ch)
			return ch->get_id (x & 0xF, y, z & 0xF);
		return 0;
	}
	
	unsigned char
	chunk_link_map::get_meta (int x, int y, int z)
	{
		chunk *ch = this->follow (x >> 4, z >> 4);
		if (ch)
			return ch->get_meta (x & 0xF, y, z & 0xF);
		return 0;
	}
	
	unsigned char
	chunk_link_map::get_extra (int x, int y, int z)
	{
		chunk *ch = this->follow (x >> 4, z >> 4);
		if (ch)
			return ch->get_extra (x & 0xF, y, z & 0xF);
		return 0;
	}
	
	blocki
	chunk_link_map::get (int x, int y, int z)
	{
		chunk *ch = this->follow (x >> 4, z >> 4);
		if (ch)
			{
				block_data bd = ch->get_block (x & 0xF, y, z & 0xF);
				return {bd.id, bd.meta, bd.ex};
			}
		return {};
	}
}

