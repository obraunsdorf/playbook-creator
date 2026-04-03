/* bridge/formation.rs
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

//! Formation-related bridge implementations

use super::controller::with_controller;
use super::types::Formation;

// ========== Opaque Type Methods ==========

pub fn formation_name(formation: &Formation) -> &str {
    formation.name.as_str()
}

pub fn formation_player_count(formation: &Formation) -> usize {
    formation.player_count()
}

// ========== Controller Operations ==========

pub fn pbc_get_formation_names() -> Vec<String> {
    with_controller(|ctrl| {
        ctrl.playbook()
            .formation_names()
            .into_iter()
            .map(|n| n.into_string())
            .collect()
    })
}

pub fn pbc_get_formation(name: &str) -> Result<Box<Formation>, String> {
    with_controller(|ctrl| {
        let formation = ctrl
            .playbook()
            .get_formation(&name.into())
            .map_err(|e| format!("Failed to get formation: {}", e))?;

        Ok(Box::new(formation.clone()))
    })
}

pub fn pbc_has_formation(name: &str) -> bool {
    with_controller(|ctrl| ctrl.playbook().has_formation(&name.into()))
}
