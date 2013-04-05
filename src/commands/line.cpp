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

#include "drawc.hpp"
#include "player.hpp"
#include "server.hpp"
#include "stringutils.hpp"
#include "utils.hpp"
#include "drawops.hpp"
#include <sstream>


namespace hCraft {
	namespace commands {
		
		namespace {
			struct line_data {
				blocki bl;
			};
		}
		
		
		static bool
		on_blocks_marked (player *pl, block_pos marked[], int markedc)
		{
			line_data *data = static_cast<line_data *> (pl->get_data ("line"));
			if (!data) return true; // shouldn't happen
			
			pl->get_logger () (LT_DEBUG) << "Drawing line from [" << marked[0].x << ", " << marked[0].y << ", " << marked[0].z << "] to ["
				<< marked[1].x << ", " << marked[1].y << ", " << marked[1].z << "]" << std::endl;
			sparse_edit_stage es (pl->get_world ());
			draw_ops draw (es);
			draw.draw_line (marked[0], marked[1], data->bl);
			es.commit ();
			
			pl->delete_data ("line");
			pl->message ("§3Line complete");
			return true;
		}
		
		
		/* 
		 * /line -
		 * 
		 * Draws a line between two selected points.
		 * 
		 * Permissions:
		 *   - command.draw.line
		 *       Needed to execute the command.
		 */
		void
		c_line::execute (player *pl, command_reader& reader)
		{
			if (!pl->perm ("command.draw.line"))
					return;
		
			if (!reader.parse_args (this, pl))
					return;
			if (reader.no_args () || reader.arg_count () > 1)
				{ this->show_summary (pl); return; }
			
			std::string& str = reader.next ().as_str ();
			if (!sutils::is_block (str))
				{
					pl->message ("§c * §7Invalid block§f: §c" + str);
					return;
				}
			
			blocki bl = sutils::to_block (str);
			if (bl.id == BT_UNKNOWN)
				{
					pl->message ("§c * §7Unknown block§f: §c" + str);
					return;
				}
			
			line_data *data = new line_data {bl};
			pl->create_data ("line", data,
				[] (void *ptr) { delete static_cast<line_data *> (ptr); });
			pl->get_nth_marking_callback (2) += on_blocks_marked;
			
			pl->message ("§8Line §7(§8Block§7: §b" + str + "§7):");
			pl->message ("§8 * §7Please mark §btwo §7blocks§7.");
		}
	}
}
