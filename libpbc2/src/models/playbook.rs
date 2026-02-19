/* models/playbook.rs
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

use super::default_playbook::{create_default_formation, create_default_routes};
use super::{PBCCategory, PBCFormation, PBCPlay, PBCRoute};
use crate::error::PbcError;
use crate::types::{CategoryName, FormationName, PlayName, RouteName};
use std::collections::{HashMap, HashSet};

/// The main playbook structure containing all plays, formations, routes, and categories
///
/// This struct manages the relationships between entities and provides
/// CRUD operations for all playbook components.
#[derive(Debug, Clone, PartialEq)]
pub struct PBCPlaybook {
    /// Version string of PBC that created this playbook
    pub built_with_pbc_version: String,
    /// Name of the playbook
    pub name: String,
    /// Number of players on the field (e.g., 5, 7, 11)
    pub player_number: u32,

    // Entity storage
    formations: HashMap<FormationName, PBCFormation>,
    routes: HashMap<RouteName, PBCRoute>,
    categories: HashMap<CategoryName, PBCCategory>,
    plays: HashMap<PlayName, PBCPlay>,

    // Relationship storage: play -> categories
    play_categories: HashMap<PlayName, HashSet<CategoryName>>,
}

impl PBCPlaybook {
    /// Creates a new playbook with default routes and formations
    ///
    /// # Arguments
    /// * `name` - The name of the playbook
    /// * `player_number` - Number of players on the field
    pub fn new(name: impl Into<String>, player_number: u32) -> Self {
        let mut pb = Self {
            built_with_pbc_version: env!("CARGO_PKG_VERSION").to_string(),
            name: name.into(),
            player_number,
            formations: HashMap::new(),
            routes: HashMap::new(),
            categories: HashMap::new(),
            plays: HashMap::new(),
            play_categories: HashMap::new(),
        };

        // Populate default routes and formations
        pb.populate_defaults();
        pb
    }

    /// Resets the playbook to a new state with default routes and formations
    pub fn reset(&mut self, name: impl Into<String>, player_number: u32) {
        self.built_with_pbc_version = env!("CARGO_PKG_VERSION").to_string();
        self.name = name.into();
        self.player_number = player_number;
        self.formations.clear();
        self.routes.clear();
        self.categories.clear();
        self.plays.clear();
        self.play_categories.clear();

        // Repopulate defaults
        self.populate_defaults();
    }

    /// Populates the playbook with default routes and formations
    fn populate_defaults(&mut self) {
        // Add default routes
        for route in create_default_routes() {
            let _ = self.add_route(route, false);
        }

        // Add default formation
        let formation = create_default_formation(self.player_number);
        let _ = self.add_formation(formation, false);
    }

    /// Reloads just the default formations (used if all formations were deleted)
    pub fn reload_default_formations(&mut self) {
        let formation = create_default_formation(self.player_number);
        let _ = self.add_formation(formation, false);
    }

    // ========== Formation Management ==========

    /// Adds a formation to the playbook
    ///
    /// # Arguments
    /// * `formation` - The formation to add
    /// * `overwrite` - If true, overwrites existing formation with same name
    ///
    /// # Returns
    /// `Ok(true)` if added, `Ok(false)` if not added (already exists and overwrite=false),
    /// or `Err` if there was an error
    pub fn add_formation(
        &mut self,
        formation: PBCFormation,
        overwrite: bool,
    ) -> Result<bool, PbcError> {
        let name = formation.name.clone();

        if overwrite {
            self.formations.insert(name, formation);
            Ok(true)
        } else {
            use std::collections::hash_map::Entry;
            match self.formations.entry(name) {
                Entry::Vacant(e) => {
                    e.insert(formation);
                    Ok(true)
                }
                Entry::Occupied(_) => Ok(false),
            }
        }
    }

    /// Removes a formation from the playbook
    pub fn delete_formation(&mut self, name: &FormationName) -> Result<(), PbcError> {
        self.formations.remove(name);
        Ok(())
    }

    /// Checks if a formation exists
    pub fn has_formation(&self, name: &FormationName) -> bool {
        self.formations.contains_key(name)
    }

    /// Gets a formation by name (returns a clone, matching C++ behavior)
    pub fn get_formation(&self, name: &FormationName) -> Result<PBCFormation, PbcError> {
        self.formations
            .get(name)
            .cloned()
            .ok_or_else(|| PbcError::NotFound(format!("Formation '{}' not found", name)))
    }

    /// Returns all formations
    pub fn formations(&self) -> Vec<PBCFormation> {
        self.formations.values().cloned().collect()
    }

    /// Returns all formation names
    pub fn formation_names(&self) -> Vec<FormationName> {
        self.formations.keys().cloned().collect()
    }

    // ========== Route Management ==========

    /// Adds a route to the playbook
    ///
    /// # Arguments
    /// * `route` - The route to add
    /// * `overwrite` - If true, overwrites existing route with same name
    pub fn add_route(&mut self, route: PBCRoute, overwrite: bool) -> Result<bool, PbcError> {
        let name: RouteName = route.name.clone().into();

        if overwrite {
            self.routes.insert(name, route);
            Ok(true)
        } else {
            use std::collections::hash_map::Entry;
            match self.routes.entry(name) {
                Entry::Vacant(e) => {
                    e.insert(route);
                    Ok(true)
                }
                Entry::Occupied(_) => Ok(false),
            }
        }
    }

    /// Removes a route from the playbook
    pub fn delete_route(&mut self, name: &RouteName) -> Result<(), PbcError> {
        self.routes.remove(name);
        Ok(())
    }

    /// Gets a route by name
    pub fn get_route(&self, name: &RouteName) -> Result<&PBCRoute, PbcError> {
        self.routes
            .get(name)
            .ok_or_else(|| PbcError::NotFound(format!("Route '{}' not found", name)))
    }

    /// Returns all routes
    pub fn routes(&self) -> Vec<PBCRoute> {
        self.routes.values().cloned().collect()
    }

    /// Returns all route names
    pub fn route_names(&self) -> Vec<RouteName> {
        self.routes.keys().cloned().collect()
    }

    // ========== Category Management ==========

    /// Adds a category to the playbook
    ///
    /// # Arguments
    /// * `category` - The category to add
    /// * `overwrite` - If true, overwrites existing category with same name
    pub fn add_category(
        &mut self,
        category: PBCCategory,
        overwrite: bool,
    ) -> Result<bool, PbcError> {
        let name = category.name.clone();

        if overwrite {
            self.categories.insert(name, category);
            Ok(true)
        } else {
            use std::collections::hash_map::Entry;
            match self.categories.entry(name) {
                Entry::Vacant(e) => {
                    e.insert(category);
                    Ok(true)
                }
                Entry::Occupied(_) => Ok(false),
            }
        }
    }

    /// Removes a category from the playbook and all its play associations
    pub fn delete_category(&mut self, name: &CategoryName) -> Result<(), PbcError> {
        // Remove this category from all plays
        for categories in self.play_categories.values_mut() {
            categories.remove(name);
        }

        // Remove the category itself
        self.categories.remove(name);
        Ok(())
    }

    /// Gets a category by name
    pub fn get_category(&self, name: &CategoryName) -> Result<&PBCCategory, PbcError> {
        self.categories
            .get(name)
            .ok_or_else(|| PbcError::NotFound(format!("Category '{}' not found", name)))
    }

    /// Returns all categories
    pub fn categories(&self) -> Vec<PBCCategory> {
        self.categories.values().cloned().collect()
    }

    /// Returns all category names
    pub fn category_names(&self) -> Vec<CategoryName> {
        self.categories.keys().cloned().collect()
    }

    // ========== Play Management ==========

    /// Adds a play to the playbook
    ///
    /// # Arguments
    /// * `play` - The play to add
    /// * `overwrite` - If true, overwrites existing play with same name
    pub fn add_play(&mut self, play: PBCPlay, overwrite: bool) -> Result<bool, PbcError> {
        let name = play.name.clone();

        if overwrite {
            self.plays.insert(name.clone(), play);
            // Keep existing category associations if overwriting
            Ok(true)
        } else {
            use std::collections::hash_map::Entry;
            match self.plays.entry(name.clone()) {
                Entry::Vacant(e) => {
                    e.insert(play);
                    // Initialize empty category set for new play
                    self.play_categories.insert(name, HashSet::new());
                    Ok(true)
                }
                Entry::Occupied(_) => Ok(false),
            }
        }
    }

    /// Removes a play from the playbook
    pub fn delete_play(&mut self, name: &PlayName) -> Result<(), PbcError> {
        self.plays.remove(name);
        self.play_categories.remove(name);
        Ok(())
    }

    /// Gets a play by name
    pub fn get_play(&self, name: &PlayName) -> Result<&PBCPlay, PbcError> {
        self.plays
            .get(name)
            .ok_or_else(|| PbcError::NotFound(format!("Play '{}' not found", name)))
    }

    /// Returns all plays
    pub fn plays(&self) -> Vec<PBCPlay> {
        self.plays.values().cloned().collect()
    }

    /// Returns all play names
    pub fn play_names(&self) -> Vec<PlayName> {
        self.plays.keys().cloned().collect()
    }

    // ========== Category-Play Relationship Management ==========

    /// Associates a play with a category
    pub fn add_play_to_category(
        &mut self,
        play_name: &PlayName,
        category_name: &CategoryName,
    ) -> Result<(), PbcError> {
        // Verify both exist
        if !self.plays.contains_key(play_name) {
            return Err(PbcError::NotFound(format!(
                "Play '{}' not found",
                play_name
            )));
        }
        if !self.categories.contains_key(category_name) {
            return Err(PbcError::NotFound(format!(
                "Category '{}' not found",
                category_name
            )));
        }

        // Add the association
        self.play_categories
            .entry(play_name.clone())
            .or_default()
            .insert(category_name.clone());

        Ok(())
    }

    /// Removes a play from a category
    pub fn remove_play_from_category(
        &mut self,
        play_name: &PlayName,
        category_name: &CategoryName,
    ) -> Result<(), PbcError> {
        if let Some(categories) = self.play_categories.get_mut(play_name) {
            categories.remove(category_name);
        }
        Ok(())
    }

    /// Gets all categories a play belongs to
    pub fn get_play_categories(&self, play_name: &PlayName) -> Vec<CategoryName> {
        self.play_categories
            .get(play_name)
            .map(|cats| cats.iter().cloned().collect())
            .unwrap_or_default()
    }

    /// Gets all plays in a category
    pub fn get_category_plays(&self, category_name: &CategoryName) -> Vec<PlayName> {
        self.play_categories
            .iter()
            .filter_map(|(play, cats)| {
                if cats.contains(category_name) {
                    Some(play.clone())
                } else {
                    None
                }
            })
            .collect()
    }
}

impl Default for PBCPlaybook {
    fn default() -> Self {
        Self::new("New Playbook", 5)
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::models::{PBCPlayer, PBCRole};
    use crate::types::{PBCColor, Point2D};

    fn make_test_player() -> PBCPlayer {
        PBCPlayer::with_details(
            PBCRole::from_strings("QB", "QB  ").unwrap(),
            PBCColor::new(255, 0, 0),
            Point2D::ZERO,
            "Test QB",
            1,
        )
    }

    #[test]
    fn test_playbook_creation() {
        let pb = PBCPlaybook::new("My Playbook", 11);
        assert_eq!(pb.name, "My Playbook");
        assert_eq!(pb.player_number, 11);

        // Should have default routes and formations
        assert_eq!(pb.routes().len(), 14); // 14 default routes
        assert_eq!(pb.formations().len(), 1); // 1 default formation (Spread Right)
        assert!(pb.categories().is_empty());
        assert!(pb.plays().is_empty());

        // Verify default formation exists
        assert!(pb.has_formation(&"Spread Right".into()));
    }

    #[test]
    fn test_add_formation() {
        let mut pb = PBCPlaybook::new("Test", 5);
        let formation = PBCFormation::with_players("I Formation", vec![make_test_player()]);

        let result = pb.add_formation(formation.clone(), false);
        assert!(result.unwrap());

        // Try adding again without overwrite
        let result = pb.add_formation(formation.clone(), false);
        assert!(!result.unwrap());

        // Try adding with overwrite
        let result = pb.add_formation(formation, true);
        assert!(result.unwrap());
    }

    #[test]
    fn test_get_formation_returns_clone() {
        let mut pb = PBCPlaybook::new("Test", 5);
        let formation = PBCFormation::with_players("Test", vec![make_test_player()]);
        pb.add_formation(formation, false).unwrap();

        let retrieved1 = pb.get_formation(&"Test".into()).unwrap();
        let retrieved2 = pb.get_formation(&"Test".into()).unwrap();

        // Should be equal but not the same instance (different allocations)
        assert_eq!(retrieved1, retrieved2);
    }

    #[test]
    fn test_has_formation() {
        let mut pb = PBCPlaybook::new("Test", 5);
        let formation = PBCFormation::new("Test");
        pb.add_formation(formation, false).unwrap();

        assert!(pb.has_formation(&"Test".into()));
        assert!(!pb.has_formation(&"NonExistent".into()));
    }

    #[test]
    fn test_delete_formation() {
        let mut pb = PBCPlaybook::new("Test", 5);
        let formation = PBCFormation::new("Test");
        pb.add_formation(formation, false).unwrap();

        assert!(pb.has_formation(&"Test".into()));
        pb.delete_formation(&"Test".into()).unwrap();
        assert!(!pb.has_formation(&"Test".into()));
    }

    #[test]
    fn test_category_play_relationship() {
        let mut pb = PBCPlaybook::new("Test", 5);

        // Add a category and play
        let category = PBCCategory::new("Offense");
        let formation = PBCFormation::new("Test");
        let play = PBCPlay::new("PA Post", "PAP", formation);

        pb.add_category(category, false).unwrap();
        pb.add_play(play, false).unwrap();

        // Associate them
        let play_name: PlayName = "PA Post".into();
        let cat_name: CategoryName = "Offense".into();

        pb.add_play_to_category(&play_name, &cat_name).unwrap();

        // Check relationships
        let play_cats = pb.get_play_categories(&play_name);
        assert_eq!(play_cats.len(), 1);
        assert_eq!(play_cats[0], cat_name);

        let cat_plays = pb.get_category_plays(&cat_name);
        assert_eq!(cat_plays.len(), 1);
        assert_eq!(cat_plays[0], play_name);
    }

    #[test]
    fn test_delete_category_removes_associations() {
        let mut pb = PBCPlaybook::new("Test", 5);

        let category = PBCCategory::new("Offense");
        let formation = PBCFormation::new("Test");
        let play = PBCPlay::new("PA Post", "PAP", formation);

        pb.add_category(category, false).unwrap();
        pb.add_play(play, false).unwrap();

        let play_name: PlayName = "PA Post".into();
        let cat_name: CategoryName = "Offense".into();

        pb.add_play_to_category(&play_name, &cat_name).unwrap();

        // Delete the category
        pb.delete_category(&cat_name).unwrap();

        // Play should no longer have this category
        let play_cats = pb.get_play_categories(&play_name);
        assert_eq!(play_cats.len(), 0);
    }

    #[test]
    fn test_delete_play_removes_associations() {
        let mut pb = PBCPlaybook::new("Test", 5);

        let category = PBCCategory::new("Offense");
        let formation = PBCFormation::new("Test");
        let play = PBCPlay::new("PA Post", "PAP", formation);

        pb.add_category(category, false).unwrap();
        pb.add_play(play, false).unwrap();

        let play_name: PlayName = "PA Post".into();
        let cat_name: CategoryName = "Offense".into();

        pb.add_play_to_category(&play_name, &cat_name).unwrap();

        // Delete the play
        pb.delete_play(&play_name).unwrap();

        // Category should no longer reference this play
        let cat_plays = pb.get_category_plays(&cat_name);
        assert_eq!(cat_plays.len(), 0);
    }

    #[test]
    fn test_multiple_categories_per_play() {
        let mut pb = PBCPlaybook::new("Test", 5);

        pb.add_category(PBCCategory::new("Offense"), false).unwrap();
        pb.add_category(PBCCategory::new("RedZone"), false).unwrap();

        let formation = PBCFormation::new("Test");
        let play = PBCPlay::new("PA Post", "PAP", formation);
        pb.add_play(play, false).unwrap();

        let play_name: PlayName = "PA Post".into();

        pb.add_play_to_category(&play_name, &"Offense".into())
            .unwrap();
        pb.add_play_to_category(&play_name, &"RedZone".into())
            .unwrap();

        let cats = pb.get_play_categories(&play_name);
        assert_eq!(cats.len(), 2);
        assert!(cats.contains(&"Offense".into()));
        assert!(cats.contains(&"RedZone".into()));
    }

    #[test]
    fn test_formation_names() {
        let mut pb = PBCPlaybook::new("Test", 5);
        pb.add_formation(PBCFormation::new("I Formation"), false)
            .unwrap();
        pb.add_formation(PBCFormation::new("Spread"), false)
            .unwrap();

        let names = pb.formation_names();
        assert_eq!(names.len(), 3); // Includes default "Spread Right"
        assert!(names.contains(&"I Formation".into()));
        assert!(names.contains(&"Spread".into()));
        assert!(names.contains(&"Spread Right".into())); // Default formation
    }
}
