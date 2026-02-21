/* controller.rs
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

use crate::models::{PBCPlay, PBCPlaybook};
use crate::types::PlayName;
use std::sync::{Mutex, OnceLock};

/// PBCController is the central point for controlling the business logic.
///
/// It acts as a facade between the GUI and the domain models, managing:
/// - The currently loaded playbook
/// - The currently active play being edited
/// - High-level operations that coordinate multiple models
///
/// This is implemented as a singleton that can be safely accessed from C++.
pub struct PBCController {
    playbook: PBCPlaybook,
    current_play: Option<PBCPlay>,
}

impl PBCController {
    /// Create a new controller with a default playbook
    fn new() -> Self {
        Self {
            playbook: PBCPlaybook::default(),
            current_play: None,
        }
    }

    /// Get the singleton Mutex
    fn mutex() -> &'static Mutex<PBCController> {
        static INSTANCE: OnceLock<Mutex<PBCController>> = OnceLock::new();
        INSTANCE.get_or_init(|| Mutex::new(PBCController::new()))
    }

    /// Access the controller with a read closure
    pub fn with<F, R>(f: F) -> R
    where
        F: FnOnce(&PBCController) -> R,
    {
        let guard = Self::mutex().lock().unwrap();
        f(&guard)
    }

    /// Access the controller with a mutable closure
    pub fn with_mut<F, R>(f: F) -> R
    where
        F: FnOnce(&mut PBCController) -> R,
    {
        let mut guard = Self::mutex().lock().unwrap();
        f(&mut guard)
    }

    // ========== Playbook Access ==========

    /// Get an immutable reference to the playbook
    pub fn playbook(&self) -> &PBCPlaybook {
        &self.playbook
    }

    /// Get a mutable reference to the playbook
    pub fn playbook_mut(&mut self) -> &mut PBCPlaybook {
        &mut self.playbook
    }

    /// Replace the current playbook entirely
    pub fn set_playbook(&mut self, playbook: PBCPlaybook) {
        self.playbook = playbook;
        self.current_play = None; // Clear current play when loading new playbook
    }

    // ========== Current Play Management ==========

    /// Get the current play being edited (if any)
    pub fn current_play(&self) -> Option<&PBCPlay> {
        self.current_play.as_ref()
    }

    /// Get a mutable reference to the current play
    pub fn current_play_mut(&mut self) -> Option<&mut PBCPlay> {
        self.current_play.as_mut()
    }

    /// Load a play from the playbook as the current play
    pub fn load_play(&mut self, play_name: &PlayName) -> Result<(), String> {
        let play = self
            .playbook
            .get_play(play_name)
            .map_err(|e| format!("Failed to load play: {}", e))?;

        self.current_play = Some(play.clone());
        Ok(())
    }

    /// Save the current play back to the playbook
    pub fn save_current_play(&mut self, overwrite: bool) -> Result<(), String> {
        let play = self
            .current_play
            .clone()
            .ok_or_else(|| "No current play to save".to_string())?;

        self.playbook
            .add_play(play, overwrite)
            .map_err(|e| format!("Failed to save play: {}", e))?;

        Ok(())
    }

    /// Save the current play with a new name
    pub fn save_current_play_as(
        &mut self,
        new_name: String,
        new_code_name: String,
    ) -> Result<(), String> {
        let mut play = self
            .current_play
            .clone()
            .ok_or_else(|| "No current play to save".to_string())?;

        play.name = new_name.into();
        play.code_name = new_code_name;

        self.playbook
            .add_play(play.clone(), true) // Always overwrite when saving as
            .map_err(|e| format!("Failed to save play: {}", e))?;

        self.current_play = Some(play);
        Ok(())
    }

    /// Clear the current play
    pub fn clear_current_play(&mut self) {
        self.current_play = None;
    }

    /// Check if there is a current play loaded
    pub fn has_current_play(&self) -> bool {
        self.current_play.is_some()
    }

    // ========== High-Level Operations ==========

    /// Create a new play and set it as current
    pub fn create_and_load_new_play(
        &mut self,
        name: String,
        code_name: String,
        formation_name: &str,
    ) -> Result<(), String> {
        let formation = self
            .playbook
            .get_formation(&formation_name.into())
            .map_err(|e| format!("Formation not found: {}", e))?;

        let play = PBCPlay::new(name, code_name, formation);
        self.current_play = Some(play);
        Ok(())
    }

    /// Delete a play from the playbook
    pub fn delete_play(&mut self, play_name: &PlayName) -> Result<(), String> {
        // Clear current play if it's the one being deleted
        if let Some(ref current) = self.current_play
            && &current.name == play_name
        {
            self.current_play = None;
        }

        self.playbook
            .delete_play(play_name)
            .map_err(|e| format!("Failed to delete play: {}", e))?;

        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::models::PBCFormation;

    #[test]
    fn test_controller_singleton() {
        let controller = PBCController::instance();
        assert_eq!(controller.playbook().name, "");
    }

    #[test]
    fn test_current_play_workflow() {
        let controller = PBCController::instance();

        // Initially no current play
        assert!(!controller.has_current_play());

        // Create and load a new play
        controller
            .create_and_load_new_play(
                "Test Play".to_string(),
                "TP".to_string(),
                "Spread Right", // Default formation
            )
            .unwrap();

        assert!(controller.has_current_play());
        assert_eq!(controller.current_play().unwrap().name, "Test Play");

        // Save the current play
        controller.save_current_play(false).unwrap();

        // Verify it's in the playbook
        assert!(controller.playbook().has_play(&"Test Play".into()));

        // Clear current play
        controller.clear_current_play();
        assert!(!controller.has_current_play());

        // Load it back
        controller.load_play(&"Test Play".into()).unwrap();
        assert!(controller.has_current_play());
    }

    #[test]
    fn test_save_play_as() {
        let controller = PBCController::instance();

        controller
            .create_and_load_new_play("Original".to_string(), "ORG".to_string(), "Spread Right")
            .unwrap();

        controller
            .save_current_play_as("Modified".to_string(), "MOD".to_string())
            .unwrap();

        // Current play should have new name
        assert_eq!(controller.current_play().unwrap().name, "Modified");

        // Both plays should exist in playbook
        assert!(controller.playbook().has_play(&"Modified".into()));
    }
}
