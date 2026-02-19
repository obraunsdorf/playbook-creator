/* models/play.rs
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

use super::PBCFormation;
use crate::types::PlayName;

/// A play in the playbook
///
/// A play consists of a formation with assigned routes and motions for each player.
/// The relationship between plays and categories is managed by the Playbook,
/// not stored directly in this struct.
#[derive(Debug, Clone, PartialEq)]
pub struct PBCPlay {
    pub name: PlayName,
    pub code_name: String,
    pub formation: PBCFormation,
    pub comment: String,
}

impl PBCPlay {
    /// Creates a new play with a formation
    ///
    /// # Arguments
    /// * `name` - The descriptive name of the play
    /// * `code_name` - A short code name for diagrams
    /// * `formation` - The formation for this play (will be owned)
    pub fn new(
        name: impl Into<PlayName>,
        code_name: impl Into<String>,
        formation: PBCFormation,
    ) -> Self {
        Self {
            name: name.into(),
            code_name: code_name.into(),
            formation,
            comment: String::new(),
        }
    }

    /// Creates a play with all fields
    pub fn with_comment(
        name: impl Into<PlayName>,
        code_name: impl Into<String>,
        formation: PBCFormation,
        comment: impl Into<String>,
    ) -> Self {
        Self {
            name: name.into(),
            code_name: code_name.into(),
            formation,
            comment: comment.into(),
        }
    }

    /// Returns the number of players in this play
    pub fn player_count(&self) -> usize {
        self.formation.player_count()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::models::{PBCPlayer, PBCRole};
    use crate::types::{PBCColor, Point2D};

    fn make_test_formation() -> PBCFormation {
        let player = PBCPlayer::with_details(
            PBCRole::from_strings("QB", "QB  ").unwrap(),
            PBCColor::new(255, 0, 0),
            Point2D::ZERO,
            "Test QB",
            1,
        );
        PBCFormation::with_players("I Formation", vec![player])
    }

    #[test]
    fn test_play_creation() {
        let formation = make_test_formation();
        let play = PBCPlay::new("PA Post", "PAP", formation);

        assert_eq!(play.name.as_str(), "PA Post");
        assert_eq!(play.code_name, "PAP");
        assert_eq!(play.comment, "");
        assert_eq!(play.player_count(), 1);
    }

    #[test]
    fn test_play_with_comment() {
        let formation = make_test_formation();
        let play = PBCPlay::with_comment("Deep Crossing", "DC", formation, "Good against Cover 2");

        assert_eq!(play.name.as_str(), "Deep Crossing");
        assert_eq!(play.code_name, "DC");
        assert_eq!(play.comment, "Good against Cover 2");
    }

    #[test]
    fn test_play_owns_formation() {
        let formation = make_test_formation();
        let formation_name = formation.name.clone();

        let play = PBCPlay::new("Test Play", "TP", formation);

        // Play owns the formation
        assert_eq!(play.formation.name, formation_name);
    }

    #[test]
    fn test_player_count() {
        let mut formation = PBCFormation::new("Test");
        formation.add_player(PBCPlayer::with_details(
            PBCRole::from_strings("WR", "WR  ").unwrap(),
            PBCColor::new(0, 255, 0),
            Point2D::new(5.0, 0.0),
            "WR1",
            11,
        ));
        formation.add_player(PBCPlayer::with_details(
            PBCRole::from_strings("WR", "WR  ").unwrap(),
            PBCColor::new(0, 255, 0),
            Point2D::new(-5.0, 0.0),
            "WR2",
            12,
        ));

        let play = PBCPlay::new("Test", "T", formation);
        assert_eq!(play.player_count(), 2);
    }
}
