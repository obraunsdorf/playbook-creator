/* models/movement.rs
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

use super::PBCPath;

/// Abstract movement structure that represents a series of path segments
///
/// This is used as the base for both routes and motions, which are
/// fundamentally sequences of path segments.
#[derive(Debug, Clone, PartialEq, Default)]
pub struct PBCAbstractMovement {
    pub paths: Vec<PBCPath>,
}

impl PBCAbstractMovement {
    /// Creates a new empty movement
    pub fn new() -> Self {
        Self { paths: Vec::new() }
    }

    /// Creates a movement with the given paths
    pub fn with_paths(paths: Vec<PBCPath>) -> Self {
        Self { paths }
    }

    /// Adds a path to the end of the movement
    pub fn add_path(&mut self, path: PBCPath) {
        self.paths.push(path);
    }

    /// Returns the number of path segments in this movement
    pub fn len(&self) -> usize {
        self.paths.len()
    }

    /// Returns true if the movement has no paths
    pub fn is_empty(&self) -> bool {
        self.paths.is_empty()
    }

    /// Clears all paths from the movement
    pub fn clear(&mut self) {
        self.paths.clear();
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::types::Point2D;

    #[test]
    fn test_movement_creation() {
        let movement = PBCAbstractMovement::new();
        assert!(movement.is_empty());
        assert_eq!(movement.len(), 0);
    }

    #[test]
    fn test_movement_with_paths() {
        let paths = vec![
            PBCPath::new(Point2D::new(10.0, 0.0)),
            PBCPath::new(Point2D::new(10.0, 10.0)),
        ];
        let movement = PBCAbstractMovement::with_paths(paths);
        assert_eq!(movement.len(), 2);
        assert!(!movement.is_empty());
    }

    #[test]
    fn test_add_path() {
        let mut movement = PBCAbstractMovement::new();
        movement.add_path(PBCPath::new(Point2D::new(5.0, 5.0)));
        movement.add_path(PBCPath::new(Point2D::new(10.0, 10.0)));

        assert_eq!(movement.len(), 2);
        assert_eq!(movement.paths[0].endpoint, Point2D::new(5.0, 5.0));
        assert_eq!(movement.paths[1].endpoint, Point2D::new(10.0, 10.0));
    }

    #[test]
    fn test_clear() {
        let mut movement =
            PBCAbstractMovement::with_paths(vec![PBCPath::new(Point2D::new(10.0, 0.0))]);
        assert!(!movement.is_empty());

        movement.clear();
        assert!(movement.is_empty());
    }
}
