/* error.rs
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
use thiserror::Error;

/// Result type alias for libpbc2 operations
pub type Result<T> = std::result::Result<T, PbcError>;

/// Error types for Playbook Creator operations
#[derive(Error, Debug)]
pub enum PbcError {
    #[error("Item not found: {0}")]
    NotFound(String),

    #[error("Item already exists: {0}")]
    AlreadyExists(String),

    #[error("Invalid input: {0}")]
    InvalidInput(String),

    #[error("Invalid color value: {field} must be 0-255, got {value}")]
    InvalidColorValue { field: String, value: u32 },

    #[error("Invalid short name: must be exactly 4 characters")]
    InvalidShortName,

    #[error("Playbook operation failed: {0}")]
    PlaybookError(String),

    #[error("Category operation failed: {0}")]
    CategoryError(String),

    #[error("Formation operation failed: {0}")]
    FormationError(String),

    #[error("Play operation failed: {0}")]
    PlayError(String),
}
