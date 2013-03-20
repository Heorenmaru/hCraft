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

#include "pickup.hpp"
#include "player.hpp"
#include "server.hpp"
#include <vector>
#include <utility>

#include <iostream> // DEBUG


namespace hCraft {
	
	/* 
	 * Class constructor.
	 */
	pickup_item::pickup_item (int eid, const slot_item& item)
		: entity (eid),
			data (item)
	{
		this->valid = true;
	}
	
	
	/* 
	 * Constructs metdata records according to the entity's type.
	 */
	void
	pickup_item::build_metadata (entity_metadata& dict) 
	{
		entity::build_metadata (dict);
		
		entity_metadata_record rec (10);
		rec.set_slot (this->data.id (), this->data.amount (), this->data.damage ());
		dict.add (rec);
	}
	
	
	
	/* 
	 * Spawns the entity to the specified player.
	 */
	void
	pickup_item::spawn_to (player *pl)
	{
		pl->send (
			packet::make_spawn_object (this->eid, 2, this->pos.x, this->pos.y,
				this->pos.z, 0.0f, 0.0f, 1, 0, 0, 0));
			
		entity_metadata dict;
		this->build_metadata (dict);
		pl->send (
			packet::make_entity_metadata (this->eid, dict));
	}
	
	
	
	/* 
	 * Half a second should pass after a pickup item spawns before it could be
	 * collected by a player. This method checks if enough time has passed.
	 */
	bool
	pickup_item::pickable ()
	{
		return ((std::chrono::steady_clock::now () - this->spawn_time)
			>= std::chrono::milliseconds (500));
	}
	
	
	static double
	calc_distance_squared (entity_pos origin, entity_pos other)
	{
		double dx = other.x - origin.x;
		double dy = other.y - origin.y;
		double dz = other.z - origin.z;
		return dx*dx + dy*dy + dz*dz;
	}

	/* 
	 * Called by the world that's holding the entity every tick (50ms).
	 * A return value of true will cause the world to destroy the entity.
	 */
	bool
	pickup_item::tick (world &w)
	{
		if (!valid)
			return false;
		
		// fall if possible
		if (w.get_id ((int)this->pos.x, (int)(this->pos.y - 0.1), (int)this->pos.z)
			== BT_AIR) // TODO: fall through any transparent block
			{
				this->pos.y -= 0.1;
			}
		
		if (!pickable ())
			return false;
		
		// fetch closest player
		std::pair<player *, double> closest;
		int i = 0;
		pickup_item *me = this;
		w.get_players ().all (
			[&i, me, &closest] (player *pl)
				{
					double dist = calc_distance_squared (me->pos, pl->pos);
					if (i == 0)
						closest = {pl, dist};
					else
						{
							if (dist < closest.second)
								closest = {pl, dist};
						}
					++ i;
				});
		if ((i == 0) || (closest.second > 2.25))
			return false;
		
		this->valid = false;
		player *pl = closest.first;
		pl->send (packet::make_collect_item (this->eid, pl->get_eid ()));
		pl->inv.add (this->data);
		return true;
	}
}

