fn main() {
    let mut cxx_build = cxx_build::bridge("src/lib.rs");

    cxx_build
        .compiler("clang")
        .flag_if_supported("-std=c++11")
        .static_crt(true); // link to MSVCRT statically on Windows (only used on windows)

    if std::env::var("CARGO_CFG_TARGET_OS").unwrap() == "windows" {
        cxx_build.static_flag(true);
    }

    cxx_build.compile("cxx-demo");
}