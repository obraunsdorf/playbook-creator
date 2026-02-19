/* models/category.rs
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

use crate::types::CategoryName;

/// A category for organizing plays
///
/// Categories group related plays together (e.g., "Pass", "Run", "Play Action").
/// The relationship between categories and plays is stored in the Playbook,
/// not in this struct, to avoid circular references.
#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub struct PBCCategory {
    pub name: CategoryName,
}

impl PBCCategory {
    /// Creates a new category
    pub fn new(name: impl Into<CategoryName>) -> Self {
        Self { name: name.into() }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_category_creation() {
        let category = PBCCategory::new("Pass");
        assert_eq!(category.name.as_str(), "Pass");
    }

    #[test]
    fn test_category_from_string() {
        let category = PBCCategory::new(String::from("Run"));
        assert_eq!(category.name.as_str(), "Run");
    }

    #[test]
    fn test_category_equality() {
        let cat1 = PBCCategory::new("Play Action");
        let cat2 = PBCCategory::new("Play Action");
        let cat3 = PBCCategory::new("Pass");

        assert_eq!(cat1, cat2);
        assert_ne!(cat1, cat3);
    }

    #[test]
    fn test_category_hash() {
        use std::collections::HashSet;

        let mut set = HashSet::new();
        set.insert(PBCCategory::new("Pass"));
        set.insert(PBCCategory::new("Pass")); // Duplicate
        set.insert(PBCCategory::new("Run"));

        assert_eq!(set.len(), 2);
    }
}
