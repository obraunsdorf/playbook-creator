/* bridge/playbook.rs
   This file is part of Playbook Creator.

    Playbook Creator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Playbook Creator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Playbook Creator.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2025 Oliver Braunsdorf
*/

//! Playbook-related bridge implementations

use super::controller::{with_controller, with_controller_mut};

pub fn pbc_get_playbook_name() -> String {
    with_controller(|ctrl| ctrl.playbook().name.clone())
}

pub fn pbc_set_playbook_name(name: String) {
    with_controller_mut(|ctrl| ctrl.playbook_mut().name = name);
}

pub fn pbc_reset_playbook(name: String, player_number: u32) {
    with_controller_mut(|ctrl| ctrl.playbook_mut().reset(name, player_number));
}
