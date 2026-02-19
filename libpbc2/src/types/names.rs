/* types/names.rs
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

/// Macro to generate strongly-typed name wrappers
///
/// This creates newtype wrappers around String for different entity names,
/// providing compile-time type safety to prevent mixing up play names,
/// formation names, etc.
macro_rules! name_type {
    ($name:ident) => {
        #[derive(Debug, Clone, PartialEq, Eq, Hash)]
        pub struct $name(String);

        impl $name {
            /// Creates a new name
            pub fn new(s: impl Into<String>) -> Self {
                Self(s.into())
            }

            /// Returns the name as a string slice
            pub fn as_str(&self) -> &str {
                &self.0
            }

            /// Consumes self and returns the inner String
            pub fn into_string(self) -> String {
                self.0
            }
        }

        impl From<&str> for $name {
            fn from(s: &str) -> Self {
                Self(s.to_string())
            }
        }

        impl From<String> for $name {
            fn from(s: String) -> Self {
                Self(s)
            }
        }

        impl AsRef<str> for $name {
            fn as_ref(&self) -> &str {
                &self.0
            }
        }

        impl std::fmt::Display for $name {
            fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
                write!(f, "{}", self.0)
            }
        }

        impl std::ops::Deref for $name {
            type Target = str;

            fn deref(&self) -> &Self::Target {
                &self.0
            }
        }
    };
}

// Define all name types
name_type!(PlayName);
name_type!(CategoryName);
name_type!(FormationName);
name_type!(RouteName);

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_play_name() {
        let name = PlayName::new("PA Post");
        assert_eq!(name.as_str(), "PA Post");
        assert_eq!(format!("{}", name), "PA Post");
    }

    #[test]
    fn test_name_from_str() {
        let name: PlayName = "Test Play".into();
        assert_eq!(name.as_str(), "Test Play");
    }

    #[test]
    fn test_name_from_string() {
        let name: FormationName = String::from("I Formation").into();
        assert_eq!(name.as_str(), "I Formation");
    }

    #[test]
    fn test_name_equality() {
        let name1 = RouteName::new("Post");
        let name2 = RouteName::new("Post");
        let name3 = RouteName::new("Slant");

        assert_eq!(name1, name2);
        assert_ne!(name1, name3);
    }

    #[test]
    fn test_name_hash() {
        use std::collections::HashSet;

        let mut set = HashSet::new();
        set.insert(CategoryName::new("Pass"));
        set.insert(CategoryName::new("Pass")); // Duplicate
        set.insert(CategoryName::new("Run"));

        assert_eq!(set.len(), 2);
    }

    #[test]
    fn test_deref() {
        let name = PlayName::new("Test");
        // Can use string methods via Deref
        assert_eq!(name.len(), 4);
        assert!(name.starts_with("Te"));
    }

    #[test]
    fn test_into_string() {
        let name = PlayName::new("Test");
        let string = name.into_string();
        assert_eq!(string, "Test");
    }
}
