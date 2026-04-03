/* bridge/controller.rs
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

//! Helper functions for controller access

use crate::controller::PBCController;

/// Executes a function with an immutable reference to the controller
pub(super) fn with_controller<F, R>(f: F) -> R
where
    F: FnOnce(&PBCController) -> R,
{
    PBCController::with(f)
}

/// Executes a function with a mutable reference to the controller
pub(super) fn with_controller_mut<F, R>(f: F) -> R
where
    F: FnOnce(&mut PBCController) -> R,
{
    PBCController::with_mut(f)
}
