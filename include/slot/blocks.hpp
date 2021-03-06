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

#ifndef _hCraft__BLOCKS_H_
#define _hCraft__BLOCKS_H_

#include <functional>
#include <cstddef>


namespace hCraft {
	
	/* 
	 * Block IDs.
	 */
	enum block_type: unsigned short
	{
		BT_UNKNOWN = 0xFFF,
		
		BT_AIR = 0,
		BT_STONE,
		BT_GRASS,
		BT_DIRT,
		BT_COBBLE,
		BT_WOOD,
		BT_SAPLING,
		BT_BEDROCK,
		BT_WATER,
		BT_STILL_WATER,
		BT_LAVA,
		BT_STILL_LAVA,
		BT_SAND,
		BT_GRAVEL,
		BT_GOLD_ORE,
		BT_IRON_ORE,
		BT_COAL_ORE,
		BT_TRUNK,
		BT_LEAVES,
		BT_SPONGE,
		BT_GLASS,
		BT_LAPIS_ORE,
		BT_LAPIS_BLOCK,
		BT_DISPENSER,
		BT_SANDSTONE,
		BT_NOTE_BLOCK,
		BT_BED,
		BT_POWERED_RAIL,
		BT_DETECTOR_RAIL,
		BT_STICKY_PISTON,
		BT_COBWEB,
		BT_TALL_GRASS,
		BT_DEAD_BUSH,
		BT_PISTON,
		BT_PISTON_EXTENSION,
		BT_WOOL,
		BT_PISTON_MOVE,
		BT_DANDELION,
		BT_ROSE,
		BT_BROWN_MUSHROOM,
		BT_RED_MUSHROOM,
		BT_GOLD_BLOCK,
		BT_IRON_BLOCK,
		BT_DSLAB,
		BT_SLAB,
		BT_BRICKS,
		BT_TNT,
		BT_BOOKSHELF,
		BT_MOSSY_COBBLE,
		BT_OBSIDIAN,
		BT_TORCH,
		BT_FIRE,
		BT_MONSTER_SPAWNER,
		BT_OAK_STAIRS,
		BT_CHEST,
		BT_REDSTONE_WIRE,
		BT_DIAMOND_ORE,
		BT_DIAMOND_BLOCK,
		BT_WORKBENCH,
		BT_WHEAT_SEEDS,
		BT_FARMLAND,
		BT_FURNACE,
		BT_BRUNING_FURNACE,
		BT_SIGN_POST,
		BT_WOODEN_DOOR,
		BT_LADDER,
		BT_RAIL,
		BT_COBBLE_STAIRS,
		BT_WALL_SIGN,
		BT_LEVER,
		BT_STONE_PRESSURE_PLATE,
		BT_IRON_DOOR,
		BT_WOODEN_PRESSURE_PLATE,
		BT_REDSTONE_ORE,
		BT_GLOWING_REDSTONE_ORE,
		BT_INACTIVE_REDSTONE_TORCH,
		BT_REDSTONE_TORCH,
		BT_STONE_BUTTON,
		BT_SNOW_COVER,
		BT_ICE,
		BT_SNOW_BLOCK,
		BT_CACTUS,
		BT_CLAY_BLOCK,
		BT_SUGAR_CANE,
		BT_JUKEBOX,
		BT_FENCE,
		BT_PUMPKIN,
		BT_NETHERRACK,
		BT_SOULSAND,
		BT_GLOWSTONE_BLOCK,
		BT_NETHER_PORTAL,
		BT_JACK_O_LANTERN,
		BT_CAKE_BLOCK,
		BT_REDSTONE_REPEATER,
		BT_ACTIVE_REDSTONE_REPEATER,
		BT_STAINED_GLASS,
		BT_TRAPDOOR,
		BT_MONSTER_EGG,
		BT_STONE_BRICKS,
		BT_HUGE_BROWN_MUSHROOM,
		BT_HUGE_RED_MUSHROOM,
		BT_IRON_BARS,
		BT_GLASS_PANE,
		BT_MELON,
		BT_PUMPKIN_STEM,
		BT_MELON_STEM,
		BT_VINES,
		BT_FENCE_GATE,
		BT_BRICK_STAIRS,
		BT_STONE_BRICK_STAIRS,
		BT_MYCELIUM,
		BT_LILYPAD,
		BT_NETHER_BRICKS,
		BT_NETHER_BRICK_FENCE,
		BT_NETHER_BRICK_STAIRS,
		BT_NETHERWART,
		BT_ENCHANTMENT_TABLE,
		BT_BREWING_STAND,
		BT_CAULDRON,
		BT_END_PORTAL,
		BT_END_PORTAL_FRAME,
		BT_END_STONE,
		BT_DRAGON_EGG,
		BT_REDSTONE_LAMP,
		BT_ACTIVE_REDSTONE_LAMP,
		BT_WOODEN_DSLAB,
		BT_WOODEN_SLAB,
		BT_COCOA_POD,
		BT_SANDSTONE_STAIRS,
		BT_EMERALD_ORE,
		BT_ENDER_CHEST,
		BT_TRIPWIRE_HOOK,
		BT_TRIPWIRE,
		BT_EMERALD_BLOCK,
		BT_SPRUCE_STAIRS,
		BT_BIRCH_STAIRS,
		BT_JUNGLE_STAIRS,
		BT_COMMAND_BLOCK,
		BT_BEACON,
		BT_COBBLESTONE_WALL,
		BT_FLOWER_POT,
		BT_CARROTS,
		BT_POTATOES,
		BT_WOODEN_BUTTON,
		BT_HEAD,
		BT_ANVIL,
		BT_TRAPPED_CHEST,
		BT_LIGHT_WEIGHTED_PRESSURE_PLATE,
		BT_HEAVY_WEIGHTED_PRESSURE_PLATE,
		BT_INACTIVE_REDSTONE_COMPARATOR,
		BT_ACTIVE_REDSTONE_COMPARATOR,
		BT_DAYLIGHT_SENSOR,
		BT_REDSTONE_BLOCK,
		BT_NETHER_QUARTZ_ORE,
		BT_HOPPER,
		BT_QUARTZ_BLOCK,
		BT_QUARTZ_STAIRS,
		BT_ACTIVATOR_RAIL,
		BT_DROPPER,
		BT_STAINED_CLAY, // 0x9F
		BT_STAINED_GLASS_PANE,
		BT_LEAVES2,	// acacia/dark oak
		BT_TRUNK2,	// acacia/dark oak
		BT_ACACIA_STAIRS,
		BT_DARK_OAK_STAIRS,
		
		BT_HAY_BLOCK = 0xAA,
		BT_CARPET,
		BT_HARDENED_CLAY,
		BT_COAL_BLOCK,
		
		
		// custom blocks
		BT_ACTIVE_WATER = 0x7D0,
		BT_WATER_SPONGE,
		BT_WATER_SPONGE_AGENT,
		BT_FIREWORK,
		BT_FIREWORK_ROCKET,
		BT_WATER_FAUCET,
		BT_LANGTONS_ANT,
		BT_SHARK,
	};
	
	
	enum block_extra_type
	{
		BE_NORMAL = 0,
		BE_DOOR,
		BE_PORTAL,
	};
	
	
	
//----
	
	enum block_state
	{
		BS_SOLID,
		BS_FLUID,
	};
	
	// TODO: remove this structure
	struct block_data
	{
		unsigned short id;
		unsigned char meta;
		char bl; // block light
		char sl; // sky light
		unsigned char ex; // extra
		
		block_data (unsigned short id = 0, char meta = 0, char bl = 0, char sl = 15, unsigned char ex = 0)
		{
			this->id = id;
			this->meta = meta;
			this->bl = bl;
			this->sl = sl;
			this->ex = ex;
		}
	};
	
	struct blocki
	{
		unsigned short id;
		unsigned char  meta;
		unsigned char  ex;
		
		blocki (unsigned short i, unsigned char m = 0, unsigned char ex = 0)
			{ this->set (i, m, ex); }
		blocki (block_data bd)
			{ this->set (bd); }
		blocki () : blocki (0, 0, 0) { }
		
		void set (unsigned short i, unsigned char m = 0, unsigned char ex = 0)
			{ this->id = i; this->meta = m; this->ex = ex; }
		
		void set (block_data bd)
			{ this->set (bd.id, bd.meta, bd.ex); }
		
		bool valid ()
			{ return ((this->id != BT_UNKNOWN) && (this->meta <= 0xF)); }
			
	//---
		bool
		operator== (const blocki other) const
		{
			return (this->id == other.id) && ((other.meta == 0x10 || this->meta == 0x10) ||
				(this->meta == other.meta)) && (this->ex == other.ex);
		}
		
		bool
		operator!= (const blocki other) const
			{ return !(this->operator== (other)); }
	};
	
	
	/* 
	 * Basic information about a single block.
	 */
	struct block_info
	{
		unsigned short id;
		char name[33];
		block_state state;
		float blast_resistance;
		char opacity;
		char luminance;
		char max_stack;
		bool opaque;
		
	//----
		
		block_info ();
		block_info (unsigned short id, const char *name, float blast_resistance,
			char opacity, char luminance, char max_stack, bool opaque,
			block_state state);
			
	//----
		
		/* 
		 * Returns the block_info structure describing the block associated with the
		 * specified ID number.
		 */
		static block_info* from_id (unsigned short id);
		
		/* 
		 * Returns the block info structure that corresponds with the given block
		 * name.
		 */
		static block_info* from_name (const char *name);
		
		/* 
		 * Attempts to find the block info structure associated with the specified
		 * string. Inputs such as "cobble" and "4" will both return the same result.
		 */
		static block_info* from_id_or_name (const char *str);
		
		
		/* 
		 * Gets the block that should be dropped after destroying blocks of type 
		 * @{bl}.
		 */
		static blocki get_drop (blocki bl);
		
		
		/* 
		 * Checks whether the specified ID value is recognized by the vanilla client
		 * as a valid block.
		 */
		static inline bool
		is_vanilla_id (unsigned short id)
		{
			return ((id <= 0xA4) || (id >= 0xAA && id <= 0xAF));
		}
	};
	
	

//----
	/* 
	 * Hash types.
	 */
	
	class blocki_hash {
		std::hash<int> ih;
		
	public:
		size_t
		operator() (const blocki bl) const
		{
			// only the id+ex are hashed, and that's done for a reason.
			return ih (bl.id * ih (bl.ex));
		}
	};
}

#endif

