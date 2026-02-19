/* models/player.rs
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

use super::{PBCMotion, PBCRole, PBCRoute};
use crate::types::{PBCColor, Point2D};

/// A player on the field with position, role, and assigned routes
#[derive(Debug, Clone, PartialEq)]
pub struct PBCPlayer {
    pub role: PBCRole,
    pub color: PBCColor,
    pub pos: Point2D,
    pub route: Option<PBCRoute>,
    pub option_routes: Vec<PBCRoute>,
    pub alternative_route1: Option<PBCRoute>,
    pub alternative_route2: Option<PBCRoute>,
    pub motion: Option<PBCMotion>,
    pub name: String,
    pub nr: u32,
}

impl PBCPlayer {
    /// Creates a new player with the minimum required fields
    ///
    /// # Arguments
    /// * `role` - The player's position role
    /// * `color` - The visual color for this player
    /// * `pos` - Starting position on the field
    pub fn new(role: PBCRole, color: PBCColor, pos: Point2D) -> Self {
        Self {
            role,
            color,
            pos,
            route: None,
            option_routes: Vec::new(),
            alternative_route1: None,
            alternative_route2: None,
            motion: None,
            name: String::new(),
            nr: 0,
        }
    }

    /// Creates a player with all fields specified
    pub fn with_details(
        role: PBCRole,
        color: PBCColor,
        pos: Point2D,
        name: impl Into<String>,
        nr: u32,
    ) -> Self {
        Self {
            role,
            color,
            pos,
            route: None,
            option_routes: Vec::new(),
            alternative_route1: None,
            alternative_route2: None,
            motion: None,
            name: name.into(),
            nr,
        }
    }

    /// Sets the player's primary route
    pub fn set_route(&mut self, route: PBCRoute) {
        self.route = Some(route);
    }

    /// Clears the player's primary route
    pub fn reset_route(&mut self) {
        self.route = None;
    }

    /// Adds an option route for this player
    pub fn add_option_route(&mut self, route: PBCRoute) {
        self.option_routes.push(route);
    }

    /// Clears all option routes
    pub fn reset_option_routes(&mut self) {
        self.option_routes.clear();
    }

    /// Sets an alternative route (version 1 or 2)
    ///
    /// # Arguments
    /// * `version` - Must be 1 or 2
    /// * `route` - The alternative route
    ///
    /// # Panics
    /// Panics if version is not 1 or 2
    pub fn set_alternative_route(&mut self, version: u32, route: PBCRoute) {
        match version {
            1 => self.alternative_route1 = Some(route),
            2 => self.alternative_route2 = Some(route),
            _ => panic!("Alternative route version must be 1 or 2"),
        }
    }

    /// Gets an alternative route by version
    pub fn alternative_route(&self, version: u32) -> Option<&PBCRoute> {
        match version {
            1 => self.alternative_route1.as_ref(),
            2 => self.alternative_route2.as_ref(),
            _ => None,
        }
    }

    /// Clears an alternative route
    pub fn reset_alternative_route(&mut self, version: u32) {
        match version {
            1 => self.alternative_route1 = None,
            2 => self.alternative_route2 = None,
            _ => {}
        }
    }

    /// Sets the player's pre-snap motion
    pub fn set_motion(&mut self, motion: PBCMotion) {
        self.motion = Some(motion);
    }

    /// Clears the player's motion
    pub fn reset_motion(&mut self) {
        self.motion = None;
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::types::Point2D;

    fn make_test_role() -> PBCRole {
        PBCRole::from_strings("Quarterback", "QB  ").unwrap()
    }

    #[test]
    fn test_player_creation() {
        let player = PBCPlayer::new(
            make_test_role(),
            PBCColor::new(255, 0, 0),
            Point2D::new(0.0, 0.0),
        );

        assert_eq!(player.pos, Point2D::ZERO);
        assert_eq!(player.name, "");
        assert_eq!(player.nr, 0);
        assert!(player.route.is_none());
        assert!(player.motion.is_none());
    }

    #[test]
    fn test_player_with_details() {
        let player = PBCPlayer::with_details(
            make_test_role(),
            PBCColor::new(255, 0, 0),
            Point2D::new(5.0, 0.0),
            "Tom Brady",
            12,
        );

        assert_eq!(player.name, "Tom Brady");
        assert_eq!(player.nr, 12);
        assert_eq!(player.pos, Point2D::new(5.0, 0.0));
    }

    #[test]
    fn test_route_management() {
        let mut player = PBCPlayer::new(make_test_role(), PBCColor::new(255, 0, 0), Point2D::ZERO);

        let route = PBCRoute::with_name("Post", "P");
        player.set_route(route.clone());
        assert!(player.route.is_some());

        player.reset_route();
        assert!(player.route.is_none());
    }

    #[test]
    fn test_option_routes() {
        let mut player = PBCPlayer::new(make_test_role(), PBCColor::new(255, 0, 0), Point2D::ZERO);

        player.add_option_route(PBCRoute::with_name("Slant", "S"));
        player.add_option_route(PBCRoute::with_name("Fade", "F"));

        assert_eq!(player.option_routes.len(), 2);

        player.reset_option_routes();
        assert_eq!(player.option_routes.len(), 0);
    }

    #[test]
    fn test_alternative_routes() {
        let mut player = PBCPlayer::new(make_test_role(), PBCColor::new(255, 0, 0), Point2D::ZERO);

        let route1 = PBCRoute::with_name("Go", "G");
        let route2 = PBCRoute::with_name("Comeback", "C");

        player.set_alternative_route(1, route1);
        player.set_alternative_route(2, route2);

        assert!(player.alternative_route(1).is_some());
        assert!(player.alternative_route(2).is_some());
        assert!(player.alternative_route(3).is_none());

        player.reset_alternative_route(1);
        assert!(player.alternative_route(1).is_none());
    }

    #[test]
    #[should_panic(expected = "Alternative route version must be 1 or 2")]
    fn test_invalid_alternative_route_version() {
        let mut player = PBCPlayer::new(make_test_role(), PBCColor::new(255, 0, 0), Point2D::ZERO);

        player.set_alternative_route(3, PBCRoute::with_name("Test", "T"));
    }

    #[test]
    fn test_motion() {
        let mut player = PBCPlayer::new(make_test_role(), PBCColor::new(255, 0, 0), Point2D::ZERO);

        let motion = PBCMotion::new();
        player.set_motion(motion);
        assert!(player.motion.is_some());

        player.reset_motion();
        assert!(player.motion.is_none());
    }
}
