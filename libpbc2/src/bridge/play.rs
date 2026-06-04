/* bridge/play.rs
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

//! Play-related bridge implementations

use super::controller::{with_controller, with_controller_mut};
use super::types::Play;
use crate::models::PBCPlay;

// ========== Opaque Type Methods ==========

pub fn play_name(play: &Play) -> &str {
    play.name.as_str()
}

pub fn play_code_name(play: &Play) -> &str {
    &play.code_name
}

pub fn play_comment(play: &Play) -> &str {
    &play.comment
}

pub fn play_formation_name(play: &Play) -> &str {
    play.formation.name.as_str()
}

pub fn play_player_count(play: &Play) -> usize {
    play.player_count()
}

// ========== Controller Operations ==========

pub fn pbc_get_play_names() -> Vec<String> {
    with_controller(|ctrl| {
        ctrl.playbook()
            .play_names()
            .into_iter()
            .map(|n| n.into_string())
            .collect()
    })
}

pub fn pbc_get_play(name: &str) -> Result<Box<Play>, String> {
    with_controller(|ctrl| {
        let play = ctrl
            .playbook()
            .get_play(&name.into())
            .map_err(|e| format!("Failed to get play: {}", e))?;

        Ok(Box::new(play.clone()))
    })
}

pub fn pbc_create_new_play(
    name: String,
    code_name: String,
    formation_name: String,
) -> Result<(), String> {
    with_controller_mut(|ctrl| {
        let formation = ctrl
            .playbook()
            .get_formation(&formation_name.into())
            .map_err(|e| format!("Formation not found: {}", e))?;

        let play = PBCPlay::new(name, code_name, formation);

        ctrl.playbook_mut()
            .add_play(play, false)
            .map_err(|e| format!("Failed to add play: {}", e))?;

        Ok(())
    })
}

pub fn pbc_has_play(name: &str) -> bool {
    with_controller(|ctrl| ctrl.playbook().get_play(&name.into()).is_ok())
}

// ========== Current Play Management ==========

pub fn pbc_load_play(name: &str) -> Result<(), String> {
    with_controller_mut(|ctrl| ctrl.load_play(&name.into()))
}

pub fn pbc_save_current_play() -> Result<(), String> {
    with_controller_mut(|ctrl| ctrl.save_current_play(true))
}

pub fn pbc_save_current_play_as(name: String, code_name: String) -> Result<(), String> {
    with_controller_mut(|ctrl| ctrl.save_current_play_as(name, code_name))
}

pub fn pbc_create_new_play_and_load(
    name: String,
    code_name: String,
    formation_name: String,
) -> Result<(), String> {
    with_controller_mut(|ctrl| ctrl.create_and_load_new_play(name, code_name, &formation_name))
}

pub fn pbc_clear_current_play() {
    with_controller_mut(|ctrl| ctrl.clear_current_play())
}

pub fn pbc_has_current_play() -> bool {
    with_controller(|ctrl| ctrl.has_current_play())
}

pub fn pbc_get_current_play() -> Result<Box<Play>, String> {
    with_controller(|ctrl| {
        let play = ctrl
            .current_play()
            .ok_or_else(|| "No current play loaded".to_string())?;
        Ok(Box::new(play.clone()))
    })
}

pub fn pbc_set_current_play_comment(comment: String) -> Result<(), String> {
    with_controller_mut(|ctrl| {
        let play = ctrl
            .current_play_mut()
            .ok_or_else(|| "No current play loaded".to_string())?;
        play.comment = comment;
        Ok(())
    })
}
