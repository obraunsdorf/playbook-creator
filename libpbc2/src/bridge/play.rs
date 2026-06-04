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
use super::ffi::{BridgePath, BridgePlaySnapshot, BridgeRoute};
use super::types::Play;
use crate::models::{PBCFormation, PBCMotion, PBCPath, PBCPlay, PBCPlayer, PBCRole, PBCRoute};
use crate::types::{PBCColor, Point2D};

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

// ========== Play Snapshot (Phase 2 dual-write) ==========

fn decode_path(bp: &BridgePath) -> PBCPath {
    if bp.has_control {
        PBCPath::new_curved(
            Point2D {
                x: bp.x,
                y: bp.y,
            },
            Point2D {
                x: bp.cx,
                y: bp.cy,
            },
        )
    } else {
        PBCPath::new(Point2D { x: bp.x, y: bp.y })
    }
}

fn decode_route_opt(br: &BridgeRoute) -> Option<PBCRoute> {
    if br.name.is_empty() {
        return None;
    }
    let paths: Vec<PBCPath> = br.paths.iter().map(decode_path).collect();
    Some(PBCRoute::new(br.name.clone(), br.code_name.clone(), paths))
}

pub fn pbc_save_play_snapshot(snapshot: BridgePlaySnapshot) -> Result<(), String> {
    with_controller_mut(|ctrl| {
        let mut players: Vec<PBCPlayer> = Vec::new();

        for bp in snapshot.players.iter() {
            let role = PBCRole::from_strings(bp.role_full_name.clone(), "    ")
                .map_err(|e| format!("Invalid role: {}", e))?;

            let color = PBCColor {
                r: bp.r,
                g: bp.g,
                b: bp.b,
            };
            let pos = Point2D {
                x: bp.pos_x,
                y: bp.pos_y,
            };

            let mut player =
                PBCPlayer::with_details(role, color, pos, bp.name.clone(), bp.nr);

            player.route = decode_route_opt(&bp.route);
            player.alternative_route1 = decode_route_opt(&bp.alt_route1);
            player.alternative_route2 = decode_route_opt(&bp.alt_route2);

            for opt_r in bp.option_routes.iter() {
                if let Some(r) = decode_route_opt(opt_r) {
                    player.option_routes.push(r);
                }
            }

            if !bp.motion_paths.is_empty() {
                let motion_paths: Vec<PBCPath> =
                    bp.motion_paths.iter().map(decode_path).collect();
                player.motion = Some(PBCMotion::with_paths(motion_paths));
            }

            players.push(player);
        }

        // Formation name is left empty for a play-specific snapshot
        let formation = PBCFormation::with_players("", players);
        let play = PBCPlay::with_comment(
            snapshot.name,
            snapshot.code_name,
            formation,
            snapshot.comment,
        );

        ctrl.playbook_mut()
            .add_play(play, true)
            .map(|_| ())
            .map_err(|e| format!("Failed to save play snapshot: {}", e))
    })
}
