/* models/role.rs
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
use crate::error::{PbcError, Result};
use std::fmt;

/// A role represents a player's position on the field
///
/// Stores both the full name (e.g., "Quarterback") and a 4-character
/// short name (e.g., "QB")
#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub struct PBCRole {
    pub full_name: String,
    pub short_name: [char; 4],
}

impl PBCRole {
    /// Creates a new PBCRole
    ///
    /// # Arguments
    /// * `full_name` - The full name of the role (e.g., "Quarterback")
    /// * `short_name` - A 4-character array for the abbreviated name
    pub const fn new(full_name: String, short_name: [char; 4]) -> Self {
        Self {
            full_name,
            short_name,
        }
    }

    /// Creates a PBCRole from strings
    ///
    /// # Arguments
    /// * `full_name` - The full name of the role
    /// * `short_name_str` - A string that will be converted to a 4-char array
    ///
    /// # Errors
    /// Returns `PbcError::InvalidShortName` if short_name_str is not exactly 4 characters
    pub fn from_strings(full_name: impl Into<String>, short_name_str: &str) -> Result<Self> {
        let chars: Vec<char> = short_name_str.chars().collect();
        if chars.len() != 4 {
            return Err(PbcError::InvalidShortName);
        }

        let short_name = [chars[0], chars[1], chars[2], chars[3]];
        Ok(Self {
            full_name: full_name.into(),
            short_name,
        })
    }

    /// Returns the short name as a String
    pub fn short_name_string(&self) -> String {
        self.short_name.iter().collect()
    }
}

impl fmt::Display for PBCRole {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{} ({})", self.full_name, self.short_name_string())
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_role_creation() {
        let role = PBCRole::new("Quarterback".to_string(), ['Q', 'B', ' ', ' ']);

        assert_eq!(role.full_name, "Quarterback");
        assert_eq!(role.short_name, ['Q', 'B', ' ', ' ']);
        assert_eq!(role.short_name_string(), "QB  ");
    }

    #[test]
    fn test_role_from_strings() {
        let role = PBCRole::from_strings("Wide Receiver", "WR  ").unwrap();

        assert_eq!(role.full_name, "Wide Receiver");
        assert_eq!(role.short_name_string(), "WR  ");
    }

    #[test]
    fn test_role_from_strings_invalid_length() {
        let result = PBCRole::from_strings("Quarterback", "QB");
        assert!(result.is_err());

        let result = PBCRole::from_strings("Quarterback", "QBACK");
        assert!(result.is_err());
    }

    #[test]
    fn test_display() {
        let role = PBCRole::from_strings("Quarterback", "QB  ").unwrap();
        assert_eq!(format!("{}", role), "Quarterback (QB  )");
    }
}
