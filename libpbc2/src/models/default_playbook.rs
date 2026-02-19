/* models/default_playbook.rs
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

use super::{PBCFormation, PBCPath, PBCPlayer, PBCRole, PBCRoute};
use crate::types::{PBCColor, Point2D};

/// Creates the default routes that come with every new playbook
///
/// These are standard football routes like Post, Slant, Fly, etc.
pub fn create_default_routes() -> Vec<PBCRoute> {
    vec![
        PBCRoute::new(
            "Hook",
            "",
            vec![
                PBCPath::new((0.0, 6.0).into()),
                PBCPath::new((1.0, 5.0).into()),
            ],
        ),
        PBCRoute::new(
            "Comeback",
            "",
            vec![
                PBCPath::new((0.0, 12.0).into()),
                PBCPath::new((-2.0, 10.0).into()),
            ],
        ),
        PBCRoute::new(
            "5 In",
            "",
            vec![
                PBCPath::new((0.0, 5.0).into()),
                PBCPath::new((10.0, 5.0).into()),
            ],
        ),
        PBCRoute::new(
            "10 In",
            "",
            vec![
                PBCPath::new((0.0, 10.0).into()),
                PBCPath::new((10.0, 10.0).into()),
            ],
        ),
        PBCRoute::new(
            "5 Out",
            "",
            vec![
                PBCPath::new((0.0, 5.0).into()),
                PBCPath::new((-10.0, 5.0).into()),
            ],
        ),
        PBCRoute::new(
            "10 Out",
            "",
            vec![
                PBCPath::new((0.0, 10.0).into()),
                PBCPath::new((-10.0, 10.0).into()),
            ],
        ),
        PBCRoute::new(
            "Slant",
            "",
            vec![
                PBCPath::new((0.0, 2.0).into()),
                PBCPath::new((9.0, 5.0).into()),
            ],
        ),
        PBCRoute::new(
            "Shallow",
            "",
            vec![
                PBCPath::new_curved((13.0, 2.0).into(), (2.0, 2.0).into()),
                PBCPath::new((15.0, 2.0).into()),
            ],
        ),
        PBCRoute::new(
            "Curl",
            "",
            vec![
                PBCPath::new((0.0, 12.0).into()),
                PBCPath::new((2.0, 10.0).into()),
            ],
        ),
        PBCRoute::new(
            "Post",
            "",
            vec![
                PBCPath::new((0.0, 7.0).into()),
                PBCPath::new((7.0, 14.0).into()),
            ],
        ),
        PBCRoute::new(
            "Corner",
            "",
            vec![
                PBCPath::new((0.0, 7.0).into()),
                PBCPath::new((-7.0, 14.0).into()),
            ],
        ),
        PBCRoute::new(
            "Fly",
            "",
            vec![
                PBCPath::new_curved((-1.0, 12.0).into(), (-0.7, 3.0).into()),
                PBCPath::new((-1.0, 14.0).into()),
            ],
        ),
        PBCRoute::new(
            "Seam",
            "",
            vec![
                PBCPath::new_curved((0.7, 12.0).into(), (-0.3, 3.0).into()),
                PBCPath::new((1.0, 14.0).into()),
            ],
        ),
        PBCRoute::new(
            "Fade",
            "",
            vec![
                PBCPath::new_curved((-1.0, 5.0).into(), (-0.7, 1.0).into()),
                PBCPath::new((-1.0, 7.0).into()),
            ],
        ),
    ]
}

/// Creates the default "Spread Right" formation
///
/// The formation scales based on the number of players:
/// - 5 players: C, QB, WRL, WRR, HB
/// - 7 players: adds LG, RG
/// - 9 players: adds FB, TE
/// - 11 players: adds LT, RT
pub fn create_default_formation(player_number: u32) -> PBCFormation {
    let mut formation = PBCFormation::new("Spread Right");

    // Core 5 players (always present)
    formation.add_player(PBCPlayer::new(
        PBCRole::from_strings("Center", "C   ").unwrap(),
        PBCColor::default(),
        Point2D::new(0.0, 0.0),
    ));

    formation.add_player(PBCPlayer::new(
        PBCRole::from_strings("Quarterback", "QB  ").unwrap(),
        PBCColor::default(),
        Point2D::new(0.0, -5.0),
    ));

    formation.add_player(PBCPlayer::new(
        PBCRole::from_strings("Wide Receiver Left", "WRL ").unwrap(),
        PBCColor::default(),
        Point2D::new(-10.0, 0.0),
    ));

    formation.add_player(PBCPlayer::new(
        PBCRole::from_strings("Wide Receiver Right", "WRR ").unwrap(),
        PBCColor::default(),
        Point2D::new(10.0, 0.0),
    ));

    formation.add_player(PBCPlayer::new(
        PBCRole::from_strings("Halfback", "HB  ").unwrap(),
        PBCColor::default(),
        Point2D::new(5.0, 0.0),
    ));

    // 7 players: add guards
    if player_number >= 7 {
        formation.add_player(PBCPlayer::new(
            PBCRole::from_strings("Left Guard", "LG  ").unwrap(),
            PBCColor::default(),
            Point2D::new(-1.0, 0.0),
        ));

        formation.add_player(PBCPlayer::new(
            PBCRole::from_strings("Right Guard", "RG  ").unwrap(),
            PBCColor::default(),
            Point2D::new(1.0, 0.0),
        ));
    }

    // 9 players: add fullback and tight end
    if player_number >= 9 {
        formation.add_player(PBCPlayer::new(
            PBCRole::from_strings("Fullback", "FB  ").unwrap(),
            PBCColor::default(),
            Point2D::new(0.0, -3.0),
        ));

        formation.add_player(PBCPlayer::new(
            PBCRole::from_strings("Tight End", "TE  ").unwrap(),
            PBCColor::default(),
            Point2D::new(3.0, -1.0),
        ));
    }

    // 11 players: add tackles
    if player_number == 11 {
        formation.add_player(PBCPlayer::new(
            PBCRole::from_strings("Left Tackle", "LT  ").unwrap(),
            PBCColor::default(),
            Point2D::new(-2.0, 0.0),
        ));

        formation.add_player(PBCPlayer::new(
            PBCRole::from_strings("Right Tackle", "RT  ").unwrap(),
            PBCColor::default(),
            Point2D::new(2.0, 0.0),
        ));
    }

    formation
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_default_routes_count() {
        let routes = create_default_routes();
        assert_eq!(routes.len(), 14);
    }

    #[test]
    fn test_default_routes_have_names() {
        let routes = create_default_routes();
        let names: Vec<&str> = routes.iter().map(|r| r.name.as_str()).collect();

        assert!(names.contains(&"Hook"));
        assert!(names.contains(&"Comeback"));
        assert!(names.contains(&"5 In"));
        assert!(names.contains(&"10 In"));
        assert!(names.contains(&"5 Out"));
        assert!(names.contains(&"10 Out"));
        assert!(names.contains(&"Slant"));
        assert!(names.contains(&"Shallow"));
        assert!(names.contains(&"Curl"));
        assert!(names.contains(&"Post"));
        assert!(names.contains(&"Corner"));
        assert!(names.contains(&"Fly"));
        assert!(names.contains(&"Seam"));
        assert!(names.contains(&"Fade"));
    }

    #[test]
    fn test_default_routes_have_paths() {
        let routes = create_default_routes();
        for route in &routes {
            assert!(
                !route.movement.is_empty(),
                "Route '{}' has no paths",
                route.name
            );
        }
    }

    #[test]
    fn test_default_formation_5_players() {
        let formation = create_default_formation(5);
        assert_eq!(formation.name.as_str(), "Spread Right");
        assert_eq!(formation.player_count(), 5);
    }

    #[test]
    fn test_default_formation_7_players() {
        let formation = create_default_formation(7);
        assert_eq!(formation.player_count(), 7);
    }

    #[test]
    fn test_default_formation_9_players() {
        let formation = create_default_formation(9);
        assert_eq!(formation.player_count(), 9);
    }

    #[test]
    fn test_default_formation_11_players() {
        let formation = create_default_formation(11);
        assert_eq!(formation.player_count(), 11);
    }

    #[test]
    fn test_default_formation_invalid_player_count() {
        // Should work with any number, just scales appropriately
        let formation = create_default_formation(3);
        assert_eq!(formation.player_count(), 5); // Still creates base 5

        let formation = create_default_formation(8);
        assert_eq!(formation.player_count(), 7); // 7 (not 9)
    }

    #[test]
    fn test_post_route() {
        let routes = create_default_routes();
        let post = routes.iter().find(|r| r.name == "Post").unwrap();

        // Post should have 2 paths
        assert_eq!(post.movement.len(), 2);
    }

    #[test]
    fn test_shallow_route_has_curve() {
        let routes = create_default_routes();
        let shallow = routes.iter().find(|r| r.name == "Shallow").unwrap();

        // Shallow's first path should be curved
        assert!(shallow.movement.paths[0].is_curved());
    }
}
