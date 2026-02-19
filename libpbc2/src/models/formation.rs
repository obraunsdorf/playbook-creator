/* models/formation.rs
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

use super::PBCPlayer;
use crate::types::FormationName;

/// A formation defines player positions on the field
///
/// Formations are templates that define where players line up. When copied
/// into a play, the players start without routes/motions assigned.
#[derive(Debug, Clone, PartialEq)]
pub struct PBCFormation {
    pub name: FormationName,
    pub players: Vec<PBCPlayer>,
}

impl PBCFormation {
    /// Creates a new empty formation
    pub fn new(name: impl Into<FormationName>) -> Self {
        Self {
            name: name.into(),
            players: Vec::new(),
        }
    }

    /// Creates a formation with players
    pub fn with_players(name: impl Into<FormationName>, players: Vec<PBCPlayer>) -> Self {
        Self {
            name: name.into(),
            players,
        }
    }

    /// Adds a player to the formation
    pub fn add_player(&mut self, player: PBCPlayer) {
        self.players.push(player);
    }

    /// Returns the number of players in the formation
    pub fn player_count(&self) -> usize {
        self.players.len()
    }

    /// Creates a copy of this formation without routes and motions
    ///
    /// This mimics the C++ copy constructor behavior, which copies player
    /// positions and roles but not their routes/motions.
    pub fn copy_without_routes(&self) -> Self {
        Self {
            name: self.name.clone(),
            players: self
                .players
                .iter()
                .map(|p| {
                    PBCPlayer::with_details(p.role.clone(), p.color, p.pos, p.name.clone(), p.nr)
                })
                .collect(),
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::models::PBCRole;
    use crate::types::{PBCColor, Point2D};

    fn make_test_player(nr: u32) -> PBCPlayer {
        PBCPlayer::with_details(
            PBCRole::from_strings("Test", "T   ").unwrap(),
            PBCColor::new(255, 0, 0),
            Point2D::new(nr as f64, 0.0),
            format!("Player {}", nr),
            nr,
        )
    }

    #[test]
    fn test_formation_creation() {
        let formation = PBCFormation::new("I Formation");
        assert_eq!(formation.name.as_str(), "I Formation");
        assert_eq!(formation.player_count(), 0);
    }

    #[test]
    fn test_formation_with_players() {
        let players = vec![make_test_player(1), make_test_player(2)];
        let formation = PBCFormation::with_players("Shotgun", players);

        assert_eq!(formation.player_count(), 2);
        assert_eq!(formation.players[0].nr, 1);
        assert_eq!(formation.players[1].nr, 2);
    }

    #[test]
    fn test_add_player() {
        let mut formation = PBCFormation::new("Pro Set");
        formation.add_player(make_test_player(1));
        formation.add_player(make_test_player(2));

        assert_eq!(formation.player_count(), 2);
    }

    #[test]
    fn test_copy_without_routes() {
        let mut formation = PBCFormation::new("Test Formation");

        // Add a player with a route
        let mut player = make_test_player(1);
        player.set_route(crate::models::PBCRoute::with_name("Post", "P"));
        player.set_motion(crate::models::PBCMotion::new());
        formation.add_player(player);

        // Copy without routes
        let copy = formation.copy_without_routes();

        assert_eq!(copy.name, formation.name);
        assert_eq!(copy.player_count(), 1);

        // Original has route, copy doesn't
        assert!(formation.players[0].route.is_some());
        assert!(copy.players[0].route.is_none());

        // Original has motion, copy doesn't
        assert!(formation.players[0].motion.is_some());
        assert!(copy.players[0].motion.is_none());

        // But positions/roles/names are preserved
        assert_eq!(copy.players[0].pos, formation.players[0].pos);
        assert_eq!(copy.players[0].nr, formation.players[0].nr);
        assert_eq!(copy.players[0].name, formation.players[0].name);
    }
}
