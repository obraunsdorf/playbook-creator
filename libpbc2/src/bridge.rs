/* bridge.rs
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

//! CXX bridge module - defines the interface between Rust and C++
//!
//! This module uses the `cxx` crate to generate type-safe bindings.
//! Simple data transfer objects (DTOs) are defined here for passing
//! data between Rust and C++.

use crate::controller::PBCController;
use crate::models::{PBCCategory, PBCFormation, PBCPlay, PBCRoute};
use crate::PBCColor;

#[cxx::bridge(namespace = "pbc2rust")]
mod ffi {
    // ========== Shared Simple Types ==========
    // Simple value types that can be freely copied between Rust and C++

    /// RGB color value
    #[derive(Debug, Clone, Copy)]
    pub struct Color {
        pub r: u8,
        pub g: u8,
        pub b: u8,
    }

    /// 2D point on the field
    #[derive(Debug, Clone, Copy)]
    pub struct Point2D {
        pub x: f64,
        pub y: f64,
    }

    // ========== Opaque Model Types ==========
    // Complex models that are owned by Rust - C++ only sees them through pointers

    extern "Rust" {
        // Opaque types for models
        type Play;
        type Formation;
        type Route;
        type Category;
        
        // Play methods
        fn play_name(play: &Play) -> &str;
        fn play_code_name(play: &Play) -> &str;
        fn play_comment(play: &Play) -> &str;
        fn play_formation_name(play: &Play) -> &str;
        fn play_player_count(play: &Play) -> usize;
        
        // Formation methods
        fn formation_name(formation: &Formation) -> &str;
        fn formation_player_count(formation: &Formation) -> usize;
        
        // Route methods
        fn route_name(route: &Route) -> &str;
        fn route_code_name(route: &Route) -> &str;
        
        // Category methods
        fn category_name(category: &Category) -> &str;

        // ========== Controller Operations ==========
        
        // Playbook operations
        fn pbc_get_playbook_name() -> String;
        fn pbc_set_playbook_name(name: String);
        fn pbc_reset_playbook(name: String, player_number: u32);

        // Play operations
        fn pbc_get_play_names() -> Vec<String>;
        fn pbc_get_play(name: &str) -> Result<Box<Play>>;
        fn pbc_create_new_play(
            name: String,
            code_name: String,
            formation_name: String,
        ) -> Result<()>;
        fn pbc_has_play(name: &str) -> bool;

        // Formation operations
        fn pbc_get_formation_names() -> Vec<String>;
        fn pbc_get_formation(name: &str) -> Result<Box<Formation>>;
        fn pbc_has_formation(name: &str) -> bool;

        // Route operations
        fn pbc_get_route_names() -> Vec<String>;
        fn pbc_get_route(name: &str) -> Result<Box<Route>>;
        fn pbc_has_route(name: &str) -> bool;

        // Category operations
        fn pbc_get_category_names() -> Vec<String>;
        fn pbc_get_category(name: &str) -> Result<Box<Category>>;
        fn pbc_has_category(name: &str) -> bool;
    }
}

// ========== Type Aliases for Opaque Types ==========
// These make the models accessible through the bridge

pub type Play = PBCPlay;
pub type Formation = PBCFormation;
pub type Route = PBCRoute;
pub type Category = PBCCategory;

// ========== Conversions for Simple Types ==========

impl From<PBCColor> for ffi::Color {
    fn from(c: PBCColor) -> Self {
        Self { r: c.r, g: c.g, b: c.b }
    }
}

impl From<ffi::Color> for PBCColor {
    fn from(c: ffi::Color) -> Self {
        Self::new(c.r, c.g, c.b)
    }
}

// ========== Getter Methods for Opaque Types ==========

// Play methods
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

// Formation methods
pub fn formation_name(formation: &Formation) -> &str {
    formation.name.as_str()
}

pub fn formation_player_count(formation: &Formation) -> usize {
    formation.player_count()
}

// Route methods
pub fn route_name(route: &Route) -> &str {
    &route.name
}

pub fn route_code_name(route: &Route) -> &str {
    &route.code_name
}

// Category methods
pub fn category_name(category: &Category) -> &str {
    category.name.as_str()
}

// ========== Implementation of Bridge Functions ==========
// All functions access the singleton controller internally

fn with_controller<F, R>(f: F) -> R
where
    F: FnOnce(&PBCController) -> R,
{
    PBCController::with(f)
}

fn with_controller_mut<F, R>(f: F) -> R
where
    F: FnOnce(&mut PBCController) -> R,
{
    PBCController::with_mut(f)
}

// Playbook operations

pub fn pbc_get_playbook_name() -> String {
    with_controller(|ctrl| ctrl.playbook().name.clone())
}

pub fn pbc_set_playbook_name(name: String) {
    with_controller_mut(|ctrl| ctrl.playbook_mut().name = name);
}

pub fn pbc_reset_playbook(name: String, player_number: u32) {
    with_controller_mut(|ctrl| ctrl.playbook_mut().reset(name, player_number));
}

// Play operations

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
    use crate::models::PBCPlay;

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

// Formation operations

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

// Route operations

pub fn pbc_get_route_names() -> Vec<String> {
    with_controller(|ctrl| {
        ctrl.playbook()
            .route_names()
            .into_iter()
            .map(|n| n.into_string())
            .collect()
    })
}

pub fn pbc_get_route(name: &str) -> Result<Box<Route>, String> {
    with_controller(|ctrl| {
        let route = ctrl
            .playbook()
            .get_route(&name.into())
            .map_err(|e| format!("Failed to get route: {}", e))?;

        Ok(Box::new(route.clone()))
    })
}

pub fn pbc_has_route(name: &str) -> bool {
    with_controller(|ctrl| ctrl.playbook().get_route(&name.into()).is_ok())
}

// Category operations

pub fn pbc_get_category_names() -> Vec<String> {
    with_controller(|ctrl| {
        ctrl.playbook()
            .category_names()
            .into_iter()
            .map(|n| n.into_string())
            .collect()
    })
}

pub fn pbc_get_category(name: &str) -> Result<Box<Category>, String> {
    with_controller(|ctrl| {
        let category = ctrl
            .playbook()
            .get_category(&name.into())
            .map_err(|e| format!("Failed to get category: {}", e))?;

        Ok(Box::new(category.clone()))
    })
}

pub fn pbc_has_category(name: &str) -> bool {
    with_controller(|ctrl| ctrl.playbook().get_category(&name.into()).is_ok())
}

