/* models/motion.rs
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
use crate::types::Point2D;

/// A player's motion before the snap
///
/// Motions are pre-snap movements that reposition a player. They track
/// the final position after the motion is complete.
#[derive(Debug, Clone, PartialEq)]
pub struct PBCMotion {
    pub movement: PBCAbstractMovement,
    pub motion_end_point: Point2D,
}

impl PBCMotion {
    /// Creates a new empty motion
    pub fn new() -> Self {
        Self {
            movement: PBCAbstractMovement::new(),
            motion_end_point: Point2D::ZERO,
        }
    }

    /// Creates a motion with the given paths
    ///
    /// The motion_end_point is automatically set to the endpoint of the last path,
    /// or Point2D::ZERO if there are no paths.
    pub fn with_paths(paths: Vec<PBCPath>) -> Self {
        let motion_end_point = paths
            .last()
            .map(|p| p.endpoint)
            .unwrap_or(Point2D::ZERO);

        Self {
            movement: PBCAbstractMovement::with_paths(paths),
            motion_end_point,
        }
    }

    /// Adds a path to this motion
    ///
    /// This updates the motion_end_point to match the endpoint of the added path.
    pub fn add_path(&mut self, path: PBCPath) {
        self.motion_end_point = path.endpoint;
        self.movement.add_path(path);
    }

    /// Returns the paths of this motion
    pub fn paths(&self) -> &[PBCPath] {
        &self.movement.paths
    }
}

impl Default for PBCMotion {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_motion_creation() {
        let motion = PBCMotion::new();
        assert!(motion.movement.is_empty());
        assert_eq!(motion.motion_end_point, Point2D::ZERO);
    }

    #[test]
    fn test_motion_with_paths() {
        let paths = vec![
            PBCPath::new(Point2D::new(5.0, 0.0)),
            PBCPath::new(Point2D::new(10.0, 0.0)),
        ];
        let motion = PBCMotion::with_paths(paths);

        assert_eq!(motion.movement.len(), 2);
        assert_eq!(motion.motion_end_point, Point2D::new(10.0, 0.0));
    }

    #[test]
    fn test_add_path_updates_endpoint() {
        let mut motion = PBCMotion::new();

        motion.add_path(PBCPath::new(Point2D::new(5.0, 0.0)));
        assert_eq!(motion.motion_end_point, Point2D::new(5.0, 0.0));

        motion.add_path(PBCPath::new(Point2D::new(10.0, -2.0)));
        assert_eq!(motion.motion_end_point, Point2D::new(10.0, -2.0));

        assert_eq!(motion.paths().len(), 2);
    }

    #[test]
    fn test_with_paths_empty() {
        let motion = PBCMotion::with_paths(vec![]);
        assert_eq!(motion.motion_end_point, Point2D::ZERO);
        assert!(motion.movement.is_empty());
    }

    #[test]
    fn test_default() {
        let motion = PBCMotion::default();
        assert_eq!(motion.motion_end_point, Point2D::ZERO);
        assert!(motion.movement.is_empty());
    }
}
