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

#ifndef _hCraft__COMMANDS__UNMUTE_H_
#define _hCraft__COMMANDS__UNMUTE_H_

#include "command.hpp"


namespace hCraft {
	namespace commands {
		
		/*
		 * /unmute
		 * 
		 * Unmutes a player.
		 * 
		 * Permissions:
		 *   - command.admin.unmute
		 *       Needed to execute the command.
		 */
		class c_unmute: public command
		{
		public:
			const char* get_name () { return "unmute"; }
			
			const char*
			get_summary ()
				{ return "Unmutes a player."; }
			
			const char*
			get_help ()
			{ return
				""
				;}
			
			const char* get_exec_permission () { return "command.admin.unmute"; }
			
		//----
			void execute (player *pl, command_reader& reader);
		};
	}
}

#endif

