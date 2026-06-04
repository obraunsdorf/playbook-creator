/* bridge/mod.rs
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

//! CXX bridge module - defines the FFI interface between Rust and C++
//!
//! This module uses the `cxx` crate to generate type-safe bindings.
//! The bridge declarations must all be in this file (cxx requirement),
//! but implementations are organized in submodules.

// Implementation modules
mod category;
mod controller;
mod formation;
mod play;
mod playbook;
mod route;
mod types;

// Re-export all public functions
pub use category::*;
pub use formation::*;
pub use play::*;
pub use playbook::*;
pub use route::*;
pub use types::*;

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

    // ========== Play Snapshot Types ==========
    // Used to pass a full play (with all player routes/motions) across the FFI
    // for the Rust shadow-write in Phase 2.

    /// A single path segment with an optional Bezier control point.
    /// has_control=false means cx/cy are ignored.
    pub struct BridgePath {
        pub x: f64,
        pub y: f64,
        pub has_control: bool,
        pub cx: f64,
        pub cy: f64,
    }

    /// A named route (or motion expressed as a list of paths).
    /// An empty `name` field signals "no route assigned".
    pub struct BridgeRoute {
        pub name: String,
        pub code_name: String,
        pub paths: Vec<BridgePath>,
    }

    /// All data needed to reconstruct one player in the Rust playbook.
    pub struct BridgePlayer {
        pub name: String,
        pub nr: u32,
        pub r: u8,
        pub g: u8,
        pub b: u8,
        pub role_full_name: String,
        pub pos_x: f64,
        pub pos_y: f64,
        /// Empty name + empty paths ⇒ no route assigned
        pub route: BridgeRoute,
        /// Empty ⇒ no motion
        pub motion_paths: Vec<BridgePath>,
        pub option_routes: Vec<BridgeRoute>,
        /// Empty name ⇒ no alternative route 1
        pub alt_route1: BridgeRoute,
        /// Empty name ⇒ no alternative route 2
        pub alt_route2: BridgeRoute,
    }

    /// A full play snapshot passed from C++ to Rust on every save.
    pub struct BridgePlaySnapshot {
        pub name: String,
        pub code_name: String,
        pub comment: String,
        pub players: Vec<BridgePlayer>,
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

        // Current play management
        fn pbc_load_play(name: &str) -> Result<()>;
        fn pbc_save_current_play() -> Result<()>;
        fn pbc_save_current_play_as(name: String, code_name: String) -> Result<()>;
        fn pbc_create_new_play_and_load(
            name: String,
            code_name: String,
            formation_name: String,
        ) -> Result<()>;
        fn pbc_clear_current_play();
        fn pbc_has_current_play() -> bool;
        fn pbc_get_current_play() -> Result<Box<Play>>;
        fn pbc_set_current_play_comment(comment: String) -> Result<()>;
        fn pbc_save_play_snapshot(snapshot: BridgePlaySnapshot) -> Result<()>;

        // Formation operations
        fn pbc_get_formation_names() -> Vec<String>;
        fn pbc_get_formation(name: &str) -> Result<Box<Formation>>;
        fn pbc_has_formation(name: &str) -> bool;

        // Route operations
        fn pbc_get_route_names() -> Vec<String>;
        fn pbc_get_route_names_sorted() -> Vec<String>;
        fn pbc_get_route(name: &str) -> Result<Box<Route>>;
        fn pbc_has_route(name: &str) -> bool;

        // Category operations
        fn pbc_get_category_names() -> Vec<String>;
        fn pbc_get_category(name: &str) -> Result<Box<Category>>;
        fn pbc_has_category(name: &str) -> bool;
    }
}

// Re-export simple FFI types for external use
pub use ffi::{BridgePath, BridgePlayer, BridgePlaySnapshot, BridgeRoute, Color, Point2D};
