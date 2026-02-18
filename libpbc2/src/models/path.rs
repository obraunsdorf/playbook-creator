/* models/path.rs
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
use crate::types::Point2D;

/// Represents a path segment with an endpoint and optional Bezier control point
///
/// A path defines a segment of movement. If bezier_control_point is Some,
/// the path will be drawn as a curve.
#[derive(Debug, Clone, PartialEq)]
pub struct PBCPath {
    pub endpoint: Point2D,
    pub bezier_control_point: Option<Point2D>,
}

impl PBCPath {
    /// Creates a new straight path with just an endpoint
    ///
    /// # Arguments
    /// * `endpoint` - The end position of this path segment
    pub const fn new(endpoint: Point2D) -> Self {
        Self {
            endpoint,
            bezier_control_point: None,
        }
    }

    /// Creates a new curved path with an endpoint and Bezier control point
    ///
    /// # Arguments
    /// * `endpoint` - The end position of this path segment
    /// * `control_point` - The Bezier control point for the curve
    pub const fn new_curved(endpoint: Point2D, control_point: Point2D) -> Self {
        Self {
            endpoint,
            bezier_control_point: Some(control_point),
        }
    }

    /// Creates a path with optional control point
    ///
    /// # Arguments
    /// * `endpoint` - The end position of this path segment
    /// * `control_point` - Optional Bezier control point for the curve
    pub const fn with_control(endpoint: Point2D, control_point: Option<Point2D>) -> Self {
        Self {
            endpoint,
            bezier_control_point: control_point,
        }
    }

    /// Checks if this path is curved (has a control point)
    pub fn is_curved(&self) -> bool {
        self.bezier_control_point.is_some()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_path_creation() {
        let path = PBCPath::new(Point2D::new(10.0, 20.0));
        assert_eq!(path.endpoint, Point2D::new(10.0, 20.0));
        assert_eq!(path.bezier_control_point, None);
        assert!(!path.is_curved());
    }

    #[test]
    fn test_path_curved() {
        let path = PBCPath::new_curved(Point2D::new(10.0, 20.0), Point2D::new(5.0, 15.0));
        assert_eq!(path.endpoint, Point2D::new(10.0, 20.0));
        assert_eq!(path.bezier_control_point, Some(Point2D::new(5.0, 15.0)));
        assert!(path.is_curved());
    }

    #[test]
    fn test_path_with_control() {
        let path = PBCPath::with_control(Point2D::new(10.0, 20.0), Some(Point2D::new(5.0, 15.0)));
        assert_eq!(path.endpoint, Point2D::new(10.0, 20.0));
        assert_eq!(path.bezier_control_point, Some(Point2D::new(5.0, 15.0)));
        assert!(path.is_curved());

        let straight = PBCPath::with_control(Point2D::new(10.0, 20.0), None);
        assert!(!straight.is_curved());
    }

    #[test]
    fn test_direct_field_access() {
        let mut path = PBCPath::new(Point2D::new(10.0, 20.0));

        // Direct mutation through public fields
        path.endpoint = Point2D::new(30.0, 40.0);
        path.bezier_control_point = Some(Point2D::new(20.0, 25.0));

        assert_eq!(path.endpoint, Point2D::new(30.0, 40.0));
        assert_eq!(path.bezier_control_point, Some(Point2D::new(20.0, 25.0)));
        assert!(path.is_curved());

        // Clear control point
        path.bezier_control_point = None;
        assert!(!path.is_curved());
    }

    #[test]
    fn test_from_tuple() {
        // Can still create from tuples thanks to From trait
        let path = PBCPath::new((10.0, 20.0).into());
        assert_eq!(path.endpoint.x, 10.0);
        assert_eq!(path.endpoint.y, 20.0);
    }
}
