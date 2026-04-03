/* bridge/types.rs
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

//! Type aliases and conversions for the bridge

use crate::models::{PBCCategory, PBCFormation, PBCPlay, PBCRoute};
use crate::PBCColor;

// ========== Type Aliases for Opaque Types ==========
// These make the models accessible through the bridge

pub type Play = PBCPlay;
pub type Formation = PBCFormation;
pub type Route = PBCRoute;
pub type Category = PBCCategory;

// ========== Conversions for Simple Types ==========

impl From<PBCColor> for super::ffi::Color {
    fn from(c: PBCColor) -> Self {
        Self { r: c.r, g: c.g, b: c.b }
    }
}

impl From<super::ffi::Color> for PBCColor {
    fn from(c: super::ffi::Color) -> Self {
        Self::new(c.r, c.g, c.b)
    }
}
