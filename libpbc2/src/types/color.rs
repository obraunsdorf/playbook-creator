/* types/color.rs
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

/// Represents an RGB color
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash, Default)]
pub struct PBCColor {
    pub r: u8,
    pub g: u8,
    pub b: u8,
}

impl PBCColor {
    /// Creates a new PBCColor
    ///
    /// # Arguments
    /// * `r` - Red component (0-255)
    /// * `g` - Green component (0-255)
    /// * `b` - Blue component (0-255)
    pub const fn new(r: u8, g: u8, b: u8) -> Self {
        Self { r, g, b }
    }

    /// Commonly used colors
    pub const BLACK: Self = Self::new(0, 0, 0);
    pub const WHITE: Self = Self::new(255, 255, 255);

    /// Calculates the contrast color for better visibility
    ///
    /// Uses luminance calculation to determine if black or white
    /// provides better contrast
    pub fn contrast_color(&self) -> Self {
        // Calculate relative luminance using standard coefficients
        let luminance = 0.299 * self.r as f64 + 0.587 * self.g as f64 + 0.114 * self.b as f64;

        // Use white text for dark colors, black for bright colors
        if luminance > 128.0 {
            Self::BLACK
        } else {
            Self::WHITE
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_color_creation() {
        let color = PBCColor::new(100, 150, 200);
        assert_eq!(color.r, 100);
        assert_eq!(color.g, 150);
        assert_eq!(color.b, 200);
    }

    #[test]
    fn test_contrast_color_dark() {
        let dark = PBCColor::new(50, 50, 50);
        let contrast = dark.contrast_color();
        assert_eq!(contrast, PBCColor::WHITE);
    }

    #[test]
    fn test_contrast_color_bright() {
        let bright = PBCColor::new(200, 200, 200);
        let contrast = bright.contrast_color();
        assert_eq!(contrast, PBCColor::BLACK);
    }

    #[test]
    fn test_default() {
        let color = PBCColor::default();
        assert_eq!(color, PBCColor::BLACK);
    }

    #[test]
    fn test_const_colors() {
        assert_eq!(PBCColor::BLACK, PBCColor::new(0, 0, 0));
        assert_eq!(PBCColor::WHITE, PBCColor::new(255, 255, 255));
    }
}
