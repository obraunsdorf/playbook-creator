/* build.rs
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

use std::env;
use std::path::PathBuf;

fn main() {
    cxx_build::bridge("src/bridge.rs")
        .flag_if_supported("-std=c++14")
        .compile("libpbc2");

    // If CXXBRIDGE_DIR is set by CMake, copy the generated header there
    if let Ok(bridge_dir) = env::var("CXXBRIDGE_DIR") {
        let out_dir = env::var("OUT_DIR").unwrap();
        let generated_header_path = PathBuf::from(&out_dir)
            .join("cxxbridge")
            .join("include")
            .join("libpbc2")
            .join("src");

        std::fs::create_dir_all(&bridge_dir).expect("Failed to create CXXBRIDGE_DIR");
        // copy all headers from the generated header directory to the CXXBRIDGE_DIR
        let error_msg = format!(
            "Failed to read generated header directory: {}",
            generated_header_path.display()
        );
        for entry in std::fs::read_dir(&generated_header_path).expect(&error_msg) {
            let entry = entry.expect("Failed to read directory entry");
            let path = entry.path();
            if path.extension().and_then(|s| s.to_str()) == Some("h") {
                let file_name = path.file_name().unwrap();
                let dest_path = PathBuf::from(&bridge_dir).join(file_name);
                std::fs::copy(&path, &dest_path)
                    .expect("Failed to copy generated header to CXXBRIDGE_DIR");
                println!(
                    "Copied generated header {} to {}",
                    path.display(),
                    dest_path.display()
                );
            }
        }
    }

    println!("cargo:rerun-if-changed=src/bridge.rs");
}
