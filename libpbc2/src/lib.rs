/* lib.rs
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
pub mod bridge;
pub mod controller;
pub mod error;
pub mod models;
pub mod types;

// Re-export commonly used types
pub use error::{PbcError, Result};
pub use models::{
    PBCAbstractMovement, PBCCategory, PBCFormation, PBCMotion, PBCPath, PBCPlay, PBCPlaybook,
    PBCPlayer, PBCRole, PBCRoute,
};
pub use types::{CategoryName, FormationName, PBCColor, PlayName, Point2D, RouteName};

// Re-export bridge types for C++ integration
pub use bridge::*;
