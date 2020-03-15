extern crate cbindgen;
use std::env;
use cbindgen::Language;

fn main() {
    let crate_dir = env::var("CARGO_MANIFEST_DIR").unwrap();

    cbindgen::Builder::new()
        .with_crate(crate_dir)
        .with_language(Language::Cxx)
        .with_include_version(true)
        .generate()
        .expect("Unable to generate bindings")
        .write_to_file("pbc_updater_bindings.h");
}
