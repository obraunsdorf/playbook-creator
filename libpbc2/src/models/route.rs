/* models/route.rs
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

use super::{PBCAbstractMovement, PBCPath};

/// A named route that a player can run
///
/// Routes have a descriptive name (e.g., "Post") and a code name for
/// play diagrams. They consist of a movement (series of paths).
#[derive(Debug, Clone, PartialEq)]
pub struct PBCRoute {
    pub name: String,
    pub code_name: String,
    pub movement: PBCAbstractMovement,
}

impl PBCRoute {
    /// Creates a new route
    ///
    /// # Arguments
    /// * `name` - The descriptive name of the route (e.g., "Post Route")
    /// * `code_name` - The code name for diagrams (e.g., "P")
    /// * `paths` - The path segments that make up this route
    pub fn new(name: impl Into<String>, code_name: impl Into<String>, paths: Vec<PBCPath>) -> Self {
        Self {
            name: name.into(),
            code_name: code_name.into(),
            movement: PBCAbstractMovement::with_paths(paths),
        }
    }

    /// Creates an empty route with a name and code name
    pub fn with_name(name: impl Into<String>, code_name: impl Into<String>) -> Self {
        Self {
            name: name.into(),
            code_name: code_name.into(),
            movement: PBCAbstractMovement::new(),
        }
    }

    /// Adds a path to this route
    pub fn add_path(&mut self, path: PBCPath) {
        self.movement.add_path(path);
    }

    /// Returns the paths of this route
    pub fn paths(&self) -> &[PBCPath] {
        &self.movement.paths
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::types::Point2D;

    #[test]
    fn test_route_creation() {
        let paths = vec![
            PBCPath::new(Point2D::new(0.0, 5.0)),
            PBCPath::new(Point2D::new(10.0, 15.0)),
        ];
        let route = PBCRoute::new("Post", "P", paths);

        assert_eq!(route.name, "Post");
        assert_eq!(route.code_name, "P");
        assert_eq!(route.movement.len(), 2);
    }

    #[test]
    fn test_route_with_name() {
        let route = PBCRoute::with_name("Slant", "S");
        assert_eq!(route.name, "Slant");
        assert_eq!(route.code_name, "S");
        assert!(route.movement.is_empty());
    }

    #[test]
    fn test_add_path() {
        let mut route = PBCRoute::with_name("Go", "G");
        route.add_path(PBCPath::new(Point2D::new(0.0, 10.0)));
        route.add_path(PBCPath::new(Point2D::new(0.0, 20.0)));

        assert_eq!(route.paths().len(), 2);
        assert_eq!(route.paths()[0].endpoint, Point2D::new(0.0, 10.0));
        assert_eq!(route.paths()[1].endpoint, Point2D::new(0.0, 20.0));
    }

    #[test]
    fn test_route_string_types() {
        // Test that both &str and String work
        let route1 = PBCRoute::with_name("Test", "T");
        let route2 = PBCRoute::with_name(String::from("Test"), String::from("T"));
        assert_eq!(route1.name, route2.name);
        assert_eq!(route1.code_name, route2.code_name);
    }
}
