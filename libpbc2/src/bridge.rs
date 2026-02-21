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

#[cxx::bridge(namespace = "pbc2rust")]
mod ffi {
    // ========== Shared Types (DTOs) ==========
    // These structs can be used from both Rust and C++

    /// Simple color representation for FFI
    #[derive(Debug, Clone)]
    pub struct ColorDto {
        pub r: u8,
        pub g: u8,
        pub b: u8,
    }

    /// Position on the field
    #[derive(Debug, Clone)]
    pub struct Point2DDto {
        pub x: f64,
        pub y: f64,
    }

    /// Player information DTO
    #[derive(Debug, Clone)]
    pub struct PlayerInfoDto {
        pub name: String,
        pub nr: u32,
        pub color: ColorDto,
        pub pos: Point2DDto,
        pub route_name: String,
    }

    /// Play metadata DTO
    #[derive(Debug, Clone)]
    pub struct PlayInfoDto {
        pub name: String,
        pub code_name: String,
        pub comment: String,
        pub formation_name: String,
    }

    /// Formation metadata DTO
    #[derive(Debug, Clone)]
    pub struct FormationInfoDto {
        pub name: String,
    }

    /// Route metadata DTO
    #[derive(Debug, Clone)]
    pub struct RouteInfoDto {
        pub name: String,
        pub code_name: String,
    }

    /// Category metadata DTO
    #[derive(Debug, Clone)]
    pub struct CategoryInfoDto {
        pub name: String,
    }

    // ========== Rust Functions Exposed to C++ ==========
    // Controller functions access the singleton internally

    extern "Rust" {
        // Playbook operations
        fn pbc_get_playbook_name() -> String;
        fn pbc_set_playbook_name(name: String);
        fn pbc_reset_playbook(name: String, player_number: u32);

        // Play operations
        fn pbc_get_play_names() -> Vec<String>;
        fn pbc_get_play_info(name: &str) -> Result<PlayInfoDto>;
        fn pbc_create_new_play(
            name: String,
            code_name: String,
            formation_name: String,
        ) -> Result<()>;
        fn pbc_has_play(name: &str) -> bool;

        // Formation operations
        fn pbc_get_formation_names() -> Vec<String>;
        fn pbc_get_formation_info(name: &str) -> Result<FormationInfoDto>;
        fn pbc_has_formation(name: &str) -> bool;

        // Route operations
        fn pbc_get_route_names() -> Vec<String>;
        fn pbc_get_route_info(name: &str) -> Result<RouteInfoDto>;
        fn pbc_has_route(name: &str) -> bool;

        // Category operations
        fn pbc_get_category_names() -> Vec<String>;
        fn pbc_get_category_info(name: &str) -> Result<CategoryInfoDto>;
        fn pbc_has_category(name: &str) -> bool;
    }
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

pub fn pbc_get_play_info(name: &str) -> Result<ffi::PlayInfoDto, String> {
    with_controller(|ctrl| {
        let play = ctrl
            .playbook()
            .get_play(&name.into())
            .map_err(|e| format!("Failed to get play: {}", e))?;

        Ok(ffi::PlayInfoDto {
            name: play.name.to_string(),
            code_name: play.code_name.clone(),
            comment: play.comment.clone(),
            formation_name: play.formation.name.to_string(),
        })
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

pub fn pbc_get_formation_info(name: &str) -> Result<ffi::FormationInfoDto, String> {
    with_controller(|ctrl| {
        let formation = ctrl
            .playbook()
            .get_formation(&name.into())
            .map_err(|e| format!("Failed to get formation: {}", e))?;

        Ok(ffi::FormationInfoDto {
            name: formation.name.to_string(),
        })
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

pub fn pbc_get_route_info(name: &str) -> Result<ffi::RouteInfoDto, String> {
    with_controller(|ctrl| {
        let route = ctrl
            .playbook()
            .get_route(&name.into())
            .map_err(|e| format!("Failed to get route: {}", e))?;

        Ok(ffi::RouteInfoDto {
            name: route.name.clone(),
            code_name: route.code_name.clone(),
        })
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

pub fn pbc_get_category_info(name: &str) -> Result<ffi::CategoryInfoDto, String> {
    with_controller(|ctrl| {
        let category = ctrl
            .playbook()
            .get_category(&name.into())
            .map_err(|e| format!("Failed to get category: {}", e))?;

        Ok(ffi::CategoryInfoDto {
            name: category.name.to_string(),
        })
    })
}

pub fn pbc_has_category(name: &str) -> bool {
    with_controller(|ctrl| ctrl.playbook().get_category(&name.into()).is_ok())
}

// Re-export the FFI types for use in other modules
pub use ffi::*;
